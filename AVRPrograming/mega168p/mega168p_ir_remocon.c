//----------------------------------------------------------------------------------------
//  赤外線学習リモコン by neko_Java 2010.11.10
//                                  2010.11.18 連続押し時、リピートフレーム出力対応
//----------------------------------------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include "../lib/ir_ctrl.h"			// http://elm-chan.org/fsw/irctrl/00index.html

#define SYSCLK  10000000UL		// CPU clock 10Mhz
#define TEN_US    400000UL		// 4サイクルのループで10μsを作る
#define LOOPCNT   (SYSCLK / TEN_US)	// ループカウント

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
#define PROG_KEY 6	//プログラムコマンド用キー番号
#define ROM_BUFF_SIZE 30 //2+28 (fmt + len + IrCtrlバッファ)但しROMには16バイトだけ書く

//ir_ctrl.cより転載
#define IR_CAPT_ENA()	TIFR1 = _BV(ICF1); TIMSK1 = _BV(ICIE1)	/* Rx: Enable captureing interrupt */
#define IR_CAPT_DIS()	TIMSK1 &= ~_BV(ICIE1)	/* Tx && Rx: Disable captureing interrupt */
#define IR_TX_OFF()	TCCR0A &= ~_BV(COM0B1)	/* Tx: Stop IR burst */


//ウエイト　10 us-------------------------------------------
static void wait_10us( uint8_t t)
{ uint8_t cnt;
  while( t--)
  { for( cnt = 0; cnt<LOOPCNT; cnt++)
     asm volatile("nop");
  }
}
//ウェイト　1 ms
static void wait_ms(uint16_t t)
{ while(t--)
   wait_10us(100);
}
//----------------------------------------------------------

volatile uint8_t key=0, old_key=0, key_cd=0, old_key_cd=0;
volatile uint8_t mode_sts=NORM_MOD;
volatile uint16_t power_off_counter=0;


ISR(PCINT1_vect) //キー操作によりウェイクアップ-------------
{
	set_sleep_mode(SLEEP_MODE_IDLE);
}

ISR(PCINT2_vect)
{
	set_sleep_mode(SLEEP_MODE_IDLE);
}


ISR(TIMER2_OVF_vect) //キースキャン-------------------------
{

	uint8_t col, j=1, k, row;

	key = 0;

	//スキャン開始---------------------------------------
	for(col=0; col<4; col++){
		DDRC &=  0xF0;
		DDRC |= j; wait_10us(1);
		k = ~((PIND & ~_BV(PD5)) | (PINC & _BV(PC5))); //bit5だけCポートを使用
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
	DDRC = 0b00011111; //スキャン終了--------------------

	if(key == old_key){
		key_cd = key;
	}

	old_key = key;

	if(key == 0 && old_key == 0){ //キー入力が無ければパワーダウン
		if(mode_sts==NORM_MOD){
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		}
		if(mode_sts==PROG_MOD){ //プログラムモードの場合は少し待ってから
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


//通常（送信）モード------------------------------------------
void norm_sub(void)
{
	uint8_t rom_buff[ROM_BUFF_SIZE];
	uint16_t rom_addr;
	uint8_t len;

	rom_addr = (key_cd - 1) * 16; //16バイトだけ割当
	eeprom_busy_wait();
	eeprom_read_block(rom_buff, (void *)rom_addr, 16); 
	eeprom_busy_wait();

	if(key_cd == old_key_cd && rom_buff[0] != SONY){ //連続押下時はリピートフレームを送信
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


//プログラム（受信）モード------------------------------------
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
			return; // タイムアウト
		}
	}
	IR_CAPT_DIS();
	RCV_POW_OFF();
	IrCtrl.state = IR_IDLE;
	rom_buff[0] = IrCtrl.fmt;
	rom_buff[1] = IrCtrl.len;
	for(i=0; i<15; i++){ //14バイトだけ。fmt + len + 14 = 16
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

	//キースキャン:タイマーオーバーフロー割込設定
	TCCR2B = 6; //prescaler 1/256
	TIMSK2 = _BV(TOIE2); //Timer2 Overflow Interrupt 8bit 1/256

	//キー操作:ピン変化割込設定
	PCICR = _BV(PCIE1) | _BV(PCIE2);//PCINT8..14, PCINT16..23 enable
	PCMSK1 = _BV(PC5);//PCINT13 enable
	PCMSK2 = ~_BV(PD5);//PCINT16..20, 22,23  enable. IRout(PD5) disable.
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); //パワーダウンモード

	// 赤外線機能の初期化
	IR_initialize();

	sei();

	uint8_t long_push_count=0;//長押しカウンタ

	for(;;)
	{
		if(key_cd != 0 ){
			if(key_cd == PROG_KEY){ //プログラムキーの場合 長押し処理
				wait_ms(10);//遅延処理
				mode_sts=NORM_MOD; G_OFF(); R_OFF();//ノーマル(送信)モード
				long_push_count++;
				if(long_push_count>100){
					power_off_counter = 0;
					mode_sts=PROG_MOD; G_ON();//プログラム(受信)モード
				}
			}else{ //各処理
				if(mode_sts==NORM_MOD){ //送信モード
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
				if(mode_sts==PROG_MOD){ //受信モード
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
