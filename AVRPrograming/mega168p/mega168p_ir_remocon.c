//----------------------------------------------------------------------------------------
//  �ԊO���w�K�����R�� by neko_Java 2010.11.10
//                                  2010.11.18 �A���������A���s�[�g�t���[���o�͑Ή�
//----------------------------------------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include "../lib/ir_ctrl.h"			// http://elm-chan.org/fsw/irctrl/00index.html

#define SYSCLK  10000000UL		// CPU clock 10Mhz
#define TEN_US    400000UL		// 4�T�C�N���̃��[�v��10��s�����
#define LOOPCNT   (SYSCLK / TEN_US)	// ���[�v�J�E���g

#define R_ON()	PORTB &= ~_BV(PB3)
#define R_OFF()	PORTB |= _BV(PB3)
#define G_ON()	PORTB &= ~_BV(PB2)
#define G_OFF()	PORTB |= _BV(PB2)
#define B_ON()	PORTC &= ~_BV(PC4)
#define B_OFF()	PORTC |= _BV(PC4)
#define RCV_POW_ON()	DDRB |= _BV(PB1); PORTB |= _BV(PB1); wait_10us(1)
#define RCV_POW_OFF()	PORTB &= ~_BV(PB1); DDRB &= ~_BV(PB1)
#define NORM_MOD 0
#define PROG_MOD 1
#define PROG_KEY 6	//�v���O�����R�}���h�p�L�[�ԍ�
#define ROM_BUFF_SIZE 30 //2+28 (fmt + len + IrCtrl�o�b�t�@)�A��ROM�ɂ�16�o�C�g��������

//ir_ctrl.c���]��
#define IR_CAPT_ENA()	TIFR1 = _BV(ICF1); TIMSK1 = _BV(ICIE1)	/* Rx: Enable captureing interrupt */
#define IR_CAPT_DIS()	TIMSK1 &= ~_BV(ICIE1)	/* Tx && Rx: Disable captureing interrupt */
#define IR_TX_OFF()	TCCR0A &= ~_BV(COM0B1)	/* Tx: Stop IR burst */


//�E�G�C�g�@10 us-------------------------------------------
static void wait_10us( uint8_t t)
{ uint8_t cnt;
  while( t--)
  { for( cnt = 0; cnt<LOOPCNT; cnt++)
     asm volatile("nop");
  }
}
//�E�F�C�g�@1 ms
static void wait_ms(uint16_t t)
{ while(t--)
   wait_10us(100);
}
//----------------------------------------------------------

volatile uint8_t key=0, old_key=0, key_cd=0, old_key_cd=0;
volatile uint8_t mode_sts=NORM_MOD;
volatile uint16_t power_off_counter=0;


ISR(PCINT1_vect) //�L�[����ɂ��E�F�C�N�A�b�v-------------
{
	set_sleep_mode(SLEEP_MODE_IDLE);
}

ISR(PCINT2_vect)
{
	set_sleep_mode(SLEEP_MODE_IDLE);
}


ISR(TIMER2_OVF_vect) //�L�[�X�L����-------------------------
{

	uint8_t col, j=1, k, row;

	key = 0;

	//�X�L�����J�n---------------------------------------
	for(col=0; col<4; col++){
		DDRC &=  0xF0;
		DDRC |= j; wait_10us(1);
		k = ~((PIND & ~_BV(PD5)) | (PINC & _BV(PC5))); //bit5����C�|�[�g���g�p
		if(k!=0) break;
		j <<= 1;
	}
	for(row=1; row<9; row++){
		if((k&1)==1) break;
		k >>= 1;
	}
	if(k==0){
		key =0;
	}else{
		key = row + (col<<3); //row+col*8 (1..32)
	}
	DDRC = 0b00011111; //�X�L�����I��--------------------

	if(key == old_key){
		key_cd = key;
	}

	old_key = key;

	if(key == 0 && old_key == 0){ //�L�[���͂�������΃p���[�_�E��
		if(mode_sts==NORM_MOD){
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		}
		if(mode_sts==PROG_MOD){ //�v���O�������[�h�̏ꍇ�͏����҂��Ă���
			power_off_counter++;
			if(power_off_counter > 2500){
				G_OFF();
				mode_sts = NORM_MOD;
				power_off_counter = 0;
				set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			//}else{
			//	set_sleep_mode(SLEEP_MODE_IDLE);
			}
		}
	}else{
		power_off_counter = 0;
	}
}


//�ʏ�i���M�j���[�h------------------------------------------
void norm_sub(void)
{
	uint8_t rom_buff[ROM_BUFF_SIZE];
	uint16_t rom_addr;
	uint8_t len;

	rom_addr = (key_cd - 1) * 16; //16�o�C�g��������
	eeprom_busy_wait();
	eeprom_read_block(rom_buff, (void *)rom_addr, 16); 
	eeprom_busy_wait();

	if(key_cd == old_key_cd && rom_buff[0] != SONY){ //�A���������̓��s�[�g�t���[���𑗐M
		len = 0;
	}else{
		len = rom_buff[1];
	}

	if(IR_xmit(rom_buff[0], rom_buff+2, len)){
		B_ON();
	}
	while(IrCtrl.state != IR_IDLE){}
	IR_CAPT_DIS();

}


//�v���O�����i��M�j���[�h------------------------------------
void prog_sub(void)
{
	uint8_t rom_buff[ROM_BUFF_SIZE];
	uint16_t rom_addr;
	uint16_t i=0;

	rom_addr = (key_cd - 1) * 16;
	IrCtrl.state = IR_IDLE;
	RCV_POW_ON();
	IR_CAPT_ENA();
	while(IrCtrl.state != IR_RECVED){
		wait_ms(10);
		i++;
		if(i==1000){
			IR_CAPT_DIS();
			RCV_POW_OFF();
			IrCtrl.state = IR_IDLE;
			return; // �^�C���A�E�g
		}
	}
	IR_CAPT_DIS();
	RCV_POW_OFF();
	IrCtrl.state = IR_IDLE;
	rom_buff[0] = IrCtrl.fmt;
	rom_buff[1] = IrCtrl.len;
	for(i=0; i<15; i++){ //14�o�C�g�����Bfmt + len + 14 = 16
		rom_buff[i+2] = IrCtrl.buff[i];
	}

	eeprom_busy_wait();
	eeprom_write_block(rom_buff, (void *)rom_addr, 16);
	eeprom_busy_wait();

}


int mega168p_ir_remocon(void)
{
	DDRB = 0b00001100;	//LED RED(PB3) GREEN(PB2) output. IRcapture(PB0) input. RCV_POW(PB1)input.
	PORTB= 0b00001100;	//LED RED(PB3) GREEN(PB2) high.RCV_POW(PB1)low.

	DDRC = 0b00011111;	//key scan(PC0-PC3) output. LED BLUE(PC4) output. key input(PC5) input.
	PORTC= 0b00110000;	//LED BLUE(PC4) high. key input(PC5) pullup.

	DDRD = 0b00100000;	//key input(PD0-PD4,PD6,PD7) input. IRout(PD5) output. 
	PORTD= 0b11011111;	//key input(PD0-PD4,PD6,PD7) pullup.

	//�L�[�X�L����:�^�C�}�[�I�[�o�[�t���[�����ݒ�
	TCCR2B = 6; //prescaler 1/256
	TIMSK2 = _BV(TOIE2); //Timer2 Overflow Interrupt 8bit 1/256

	//�L�[����:�s���ω������ݒ�
	PCICR = _BV(PCIE1) | _BV(PCIE2);//PCINT8..14, PCINT16..23 enable
	PCMSK1 = _BV(PC5);//PCINT13 enable
	PCMSK2 = ~_BV(PD5);//PCINT16..20, 22,23  enable. IRout(PD5) disable.
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); //�p���[�_�E�����[�h

	// �ԊO���@�\�̏�����
	IR_initialize();

	sei();

	uint8_t long_push_count=0;//�������J�E���^

	for(;;)
	{
		if(key_cd != 0 ){
			if(key_cd == PROG_KEY){ //�v���O�����L�[�̏ꍇ ����������
				wait_ms(10);//�x������
				mode_sts=NORM_MOD; G_OFF(); R_OFF();//�m�[�}��(���M)���[�h
				long_push_count++;
				if(long_push_count>100){
					power_off_counter = 0;
					mode_sts=PROG_MOD; G_ON();//�v���O����(��M)���[�h
				}
			}else{ //�e����
				if(mode_sts==NORM_MOD){ //���M���[�h
					norm_sub();
					switch (IrCtrl.fmt) {
						case NEC:
							wait_ms(108); break;
						case AEHA:
							wait_ms(130); break;
						case SONY:
							wait_ms(20);
					}
					B_OFF();
					old_key_cd = key_cd;
				}
				if(mode_sts==PROG_MOD){ //��M���[�h
					G_OFF(); R_ON();
					prog_sub();
					power_off_counter = 0;
					G_ON(); R_OFF();
				}
			}
			key_cd = 0;
		}else{
			long_push_count=0;
			old_key_cd = 0;
		}
		sleep_mode();
	}
}
