//-----------------------------------------------------------
// TIMER CONTROL
// by takuya matsubara
// http://nicotak.com

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#ifndef F_CPU
#define   F_CPU   8000000    //CPU�N���b�N���g��
#endif

#define TMR_PS   8        //�v���X�P�[���l(������)
#define TMR_PSCR 2        //�v���X�P�[���ݒ�l

//-----------------------------------------------------------
// �^�C�}������
void timer_init(void)
{
	TCCR0A = 0;	// Normal port operation
	TCCR0B = TMR_PSCR;	// Clock select(div8)
}

//-----------------------------------------------------------
//�@�^�C�}�E�G�C�g
//����:�E�G�C�g���� 1�`255[usec]
void timer_waitusec(unsigned char usec)
{
    TCNT0 = (unsigned char)(0x100-usec);
    TIFR0 |= (1 << TOV0);  // TOVx�r�b�g��0�N���A

    while(!(TIFR0 & (1 << TOV0)));
    // TOVx�r�b�g��1�ɂȂ�܂�
}

//-----------------------------------------------------------
//�@�^�C�}�E�G�C�g
//�����F�E�G�C�g���ԁ@1�`6553[msec]
void timer_waitmsec(unsigned int i)
{
	i*=10;

	while(i--){
		timer_waitusec(100);
	}
}

