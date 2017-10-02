//---------------------------------------------------------------------
//(8x8Dot)LED Game for ATmega168  by Takuya Matsubara
//---------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/sleep.h>
#include "../lib/sw.h"

#define LEDWIDTH 8       // LED�h�b�g��
#define BLOCKWIDTH 3     // �}�C�u���b�N�̃h�b�g��

#define LEVELMAX 20      // ����Q�[�����x��
#define LEVELMIN 3       // �ō����x�Q�[�����x��
#define SWCHKCNT 160     // �X�C�b�`���o���[�v��
#define WAITLOOP 500     // �E�G�C�g���[�v��

unsigned char ledvram[LEDWIDTH];    // LED Bitmap Buffer
unsigned char block[LEDWIDTH];      // 
unsigned char myblock[BLOCKWIDTH];  //�}�C�u���b�N���
char row;
char level;              // �Q�[�����x��
char score;              // �X�R�A

#define LEDCOL   PORTB   // LED(��)�p�|�[�g
#define LEDCOLOE DDRB
#define LEDROW   PORTD   // LED(�s)�p�|�[�g
#define LEDROWOE DDRD

#define RANDOM TCNT1

#define SW_UP     (1<<0)
#define SW_DOWN   (1<<1)
#define SW_LEFT   (1<<2)
#define SW_RIGHT  (1<<3)
#define SW_TURN   (1<<4)
#define SW_ALL    (SW_UP|SW_DOWN|SW_LEFT|SW_RIGHT|SW_TURN)

#define KABE    0b10000001
#define LINE    0b01111110

void block2vram(char tx,char ty);
void turnblock(void);
void newblock(void);
unsigned char hitcheck(char cx, char cy);
void fixblock(char tx ,char ty);
void led_init(void);
void led_putch(char tx ,char ty, char ch);
void led_disp(void);

//---------------------------------------------------------------------
// �s���`�F���W���荞��
SIGNAL (SIG_PIN_CHANGE1)
{
}

//---------------------------------------------------------------------
// LED������
void led_init(void)
{
    LEDCOLOE = 0xff;    //output
    LEDROWOE = 0x00;    //input

    row = 0;
}

//---------------------------------------------------------------------
// �t�H���g�f�[�^ 0�`9
const unsigned char ledfont[]={
    0x02,0x55,0x52,
    0x02,0x62,0x22,
    0x06,0x12,0x47,
    0x06,0x12,0x16,
    0x05,0x57,0x11,
    0x07,0x46,0x16,
    0x03,0x46,0x52,
    0x07,0x12,0x44,
    0x02,0x52,0x52,
    0x02,0x53,0x16
};

//---------------------------------------------------------------------
//���l�L�����N�^��VRAM�]��
void led_putch(char tx,char ty,char ch)
{
    unsigned char i;
    unsigned char bitdata;

    ch *= 3;
    for(i=0 ;i<6 ;i++) {
        ledvram[ty] &= ~(0xf << (4-tx));
        bitdata = ledfont[(int)ch];
        if(i % 2)
            ch++;
        else
            bitdata >>= 4;

        bitdata &= 0xf;
        bitdata <<= (4-tx);
        ledvram[ty] |= bitdata;
        ty++;
    }
}

//---------------------------------------------------------------------
// �u���b�N��VRAM�ɓ]��
void block2vram(char tx, char ty)
{
    unsigned char    i;

    // -------�u���b�N����VRAM�փR�s�[
    for(i=0; i<LEDWIDTH; i++){
        ledvram[i] = block[i];
    }
    // ---------�}�C�u���b�N��VRAM�ɃR�s�[
    for(i=0; i<BLOCKWIDTH; i++){
        ledvram[i+ty] |= (myblock[i]>>tx);
    }
}

//---------------------------------------------------------------------
// �}�C�u���b�N��]
void turnblock(void)
{
    unsigned char col,col2;
    unsigned char blockbak[BLOCKWIDTH];

    for( col=0; col<BLOCKWIDTH; col++){
        blockbak[col] = myblock[col];
        myblock[col]=0;
    }
    for( col=0; col<BLOCKWIDTH; col++){
        for( col2=0; col2<BLOCKWIDTH; col2++){
            myblock[col] <<= 1;
            myblock[col] |= ((blockbak[col2] >> col) & (1<<(LEDWIDTH-BLOCKWIDTH)));
        }
    }
}

//---------------------------------------------------------------------
// �V�K�}�C�u���b�N
void newblock(void)
{
    int i;

    switch(RANDOM % 7){
    case 0:
        myblock[0] = 0b00000000;
        myblock[1] = 0b11100000;
        myblock[2] = 0b00000000;
        break;
    case 1:
        myblock[0] = 0b01000000;
        myblock[1] = 0b11100000;
        myblock[2] = 0b00000000;
        break;
    case 2:
        myblock[0] = 0b10000000;
        myblock[1] = 0b11100000;
        myblock[2] = 0b00000000;
        break;
    case 3:
        myblock[0] = 0b00100000;
        myblock[1] = 0b11100000;
        myblock[2] = 0b00000000;
        break;
    case 4:
        myblock[0] = 0b11000000;
        myblock[1] = 0b01100000;
        myblock[2] = 0b00000000;
        break;
    case 5:
        myblock[0] = 0b01100000;
        myblock[1] = 0b11000000;
        myblock[2] = 0b00000000;
        break;
    case 6:
        myblock[0] = 0b11000000;
        myblock[1] = 0b11000000;
        myblock[2] = 0b00000000;
        break;
    }
    for(i=0; i<(RANDOM % 4); i++){
        turnblock();    // �}�C�u���b�N��]
    }
}

//---------------------------------------------------------------------
// �u���b�N�Փ˃`�F�b�N
unsigned char hitcheck(char cx, char cy)
{
    unsigned char i;

    for(i=0; i<BLOCKWIDTH; i++){
        if(block[cy+i] & (myblock[i] >> cx))
            return(1);
    }

    return(0);
}

//---------------------------------------------------------------------
// �u���b�N��蒅�A���������̏���
void fixblock(char tx, char ty)
{
    unsigned char col,i;

    for(col=0; col<BLOCKWIDTH; col++){
        block[col+ty] |= (myblock[col]>>tx);
    }

    //---------���������̃u���b�N������
    for( col = 0; col<(LEDWIDTH-1); col++)
    {
        if( ledvram[col] == (LINE | KABE) ){
            for( i=col ;i>=1 ;i--){
                block[i] = block[i-1];
            }
            block[0] = KABE;
            if( level > LEVELMIN ) level --;
            score++;
        }
    }
}

//---------------------------------------------------------------------
// LED����
void led_disp(void)
{
    LEDCOL = 0x00;              // LED����
    LEDROWOE = (1<<row);        // 1�s�̂�Low�o��
    LEDROW = 0;
    LEDCOL = ledvram[(int)row]; //LED�_��

    row = (row+1) % LEDWIDTH;
}

//---------------------------------------------------------------------
// ���C������
int mega168p_ledgame(void)
{
    unsigned int  keycnt;
    char dropcnt;    // �����J�E���^
    unsigned char i;
    int t;
    char seq;
    char x,y;        // �}�C�u���b�N���W

    sw_init(SW_ALL);
    led_init();
    seq = 0;

    TCNT1 = 0;       // �^�C�}1�̏����l�ݒ�   
    TCCR1A= 0;       // �^�C�}1 ���[�h 
    TCCR1B= 1;       // �^�C�}1 �v���X�P�[�� 

    while(1)
    {
        led_disp();

        switch(seq)
        {
        case 0:        //�u���b�N������
            for( i=0; i<LEDWIDTH; i++){
                block[i]=KABE;
            }
            block[LEDWIDTH-1] = KABE|LINE;
            newblock();        //�V�K�}�C�u���b�N
            x = 3;
            y = 0;
            block2vram(x,y);
            dropcnt=1;
            level = LEVELMAX;  //�Q�[�����x��
            score=0;
            seq++;
            break;

        case 1:        //�Q�[���v���C
            keycnt=(keycnt+1) % SWCHKCNT;
            if(keycnt != 0) break;

            if (sw_get(SW_DOWN)) {   // ���{�^��
                dropcnt = 0;         // �u���b�N����
            }else{
                if((sw_get(SW_LEFT)) && (x>0)){             // �E�{�^��
                    if(hitcheck(x-1,y)==0) x--;
                    block2vram(x,y);
                }
                if((sw_get(SW_RIGHT)) &&(x<(LEDWIDTH-1))) { // ���{�^��
                    if(hitcheck(x+1,y)==0) x++;
                    block2vram(x,y);
                }
            }

            if (sw_get(SW_TURN) ) {  // --------��]�{�^��
                turnblock();         // �}�C�u���b�N��]
                if(hitcheck(x,y)) {  // �u���b�N�Փ˂Ȃ�A
                    turnblock();     // ��]�����ɖ߂�
                    turnblock();
                    turnblock();
                }else{
                    block2vram(x,y);
                }
            }

            if(dropcnt==0) {         //---------�}�C�u���b�N�̗���
                dropcnt = level;
                if( (y>=(LEDWIDTH-BLOCKWIDTH))||(hitcheck(x,y+1)) ){ // �u���b�N�Փ�
                    fixblock(x,y);   // �u���b�N��蒅
                    newblock();      //�V�K�}�C�u���b�N
                    x = 3;
                    y = 0;
                    if( block[0] & ~KABE) // �ŏ�i�Ƀu���b�N����̏ꍇ�A
                        seq++;            // �Q�[���I�[�o�[
                }else{
                    y++;
                }
                block2vram(x,y);
            }
            dropcnt--;
            break;
        case 2:        // �X�R�A�\��
            for(i=0; i<LEDWIDTH; i++){
                ledvram[i] = 0;
            }
            led_putch(0 ,0 ,score / 10 );
            led_putch(4 ,0 ,score % 10 );
            keycnt = 0;
            seq++;    
            break;
        case 3:
            keycnt = keycnt+1;
            if(keycnt > SWCHKCNT*20) seq++;
            break;
        case 4:
            PCMSK1 |= ((1<<PCINT8)|(1<<PCINT9)|(1<<PCINT10)|(1<<PCINT11)|(1<<PCINT12));
            PCICR = (1<<PCIE1);    // �s���`�F���W���荞�݋���
            PCIFR = (1<<PCIF1);    // �s���`�F���W���荞�݃t���O
            sei();
            LEDCOL = 0x00;            // LED����
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_mode();    //�X���[�v
            seq=0;
            break;
        }
        for(t=0; t<WAITLOOP; t++);
    }
}

