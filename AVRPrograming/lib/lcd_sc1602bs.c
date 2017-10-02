/*
	== lcd.c ==
	SC1602BS Charactor LCD (HD44780 compatible) driver library
	written by mojo@uec-koken 2009-5
*/

#include "lcd_sc1602bs.h"


void delay_ms( int time )
{
	// �w��ms�����[�v
	while( time-- ){
		_delay_ms( 1 );
	}
}

void Lcd_init( void )
{
	// �ŏ��͑S�Ẵ|�[�g���o�͂ɐݒ�
	LCD_DDR	|= ( LCD_E | LCD_RW | LCD_RS | LCD_DATMASK );
	// �M����Low�ɂ��Ă���
	LCD_PORT &= ~( LCD_E | LCD_RW | LCD_RS | LCD_DATMASK );

	// �ŏ���Busy�`�F�b�N�������Ȃ��̂ŁA���ԑ҂�����
	delay_ms( 15 );		// 15ms�҂�
	lcd_setcmd4( RS_MODE_COMMAND, 0x3 );	// 8bit���[�h�ݒ�
	delay_ms( 5 );		// 4.1ms�ȏ�҂��Ȃ̂ŁA5ms�҂�
	lcd_setcmd4( RS_MODE_COMMAND, 0x3 );	// 8bit���[�h�ݒ�
	delay_ms( 1 );		// 100��s�ȏ�҂��Ȃ̂ŁA�Ƃ肠����1ms�҂��Ă���
	lcd_setcmd4( RS_MODE_COMMAND, 0x3 );	// 8bit���[�h�ݒ�
	delay_ms( 1 );		// �t�@���N�V�����Z�b�g��39��s�ȏ�҂��Ȃ̂ŁA�Ƃ肠����1ms�҂��Ă���

	// �������V�[�P���X�ɏ����Ă���ʂ�A4bit�̐ݒ��4bit�̂ݓn����ΗL���Ȃ̂ŁA1��̂�
	lcd_setcmd4( RS_MODE_COMMAND, 0x2 );	// 4bit���[�h�ݒ�
	delay_ms( 1 );		// �t�@���N�V�����Z�b�g��39��s�ȏ�҂��Ȃ̂ŁA�Ƃ肠����1ms�҂��Ă���

	// ���������4bit���[�h����Ȃ̂ŁA8bit�̖��߂�4bit����2��ɕ����đ����Ă��
	lcd_setcmd4( RS_MODE_COMMAND, 0x2 );	// �t�@���N�V�����Z�b�g�A4bit���[�h
	lcd_setcmd4( RS_MODE_COMMAND, 0x8 );	// 2Line�\���A5*8�h�b�g�t�H���g
	delay_ms( 1 );		// �t�@���N�V�����Z�b�g��39��s�ȏ�҂��Ȃ̂ŁA�Ƃ肠����1ms�҂��Ă���
	lcd_setcmd4( RS_MODE_COMMAND, 0x0 );	// Display ON/OFF�ݒ�
	lcd_setcmd4( RS_MODE_COMMAND, 0xF );	// Display ON / Cursor ON / Blinking ON
	delay_ms( 1 );		// Display ON/OFF�ݒ��39��s�ȏ�҂��Ȃ̂ŁA�Ƃ肠����1ms�҂��Ă���
	lcd_setcmd4( RS_MODE_COMMAND, 0x0 );	// Clear Display�R�}���h
	lcd_setcmd4( RS_MODE_COMMAND, 0x1 );	// �X�y�[�X��DDRAM�����߂��A�A�h���X�J�E���^��DDRAM��00H(�擪)�Ɉړ�
	delay_ms( 2 );		// Clear Display�R�}���h��1.53ms�ȏ�҂��Ȃ̂ŁA2ms�҂�
	lcd_setcmd4( RS_MODE_COMMAND, 0x0 );	// Entry Mode
	lcd_setcmd4( RS_MODE_COMMAND, 0x6 );	// �C���N�������g���[�h / �f�B�X�v���C�V�t�g�Ȃ�
	delay_ms( 1 );		// Entry Mode�R�}���h��39��s�ȏ�҂��Ȃ̂ŁA�Ƃ肠����1ms�҂��Ă���
}

void lcd_setcmd4( RSMode mode, uint8_t data )
{
	uint8_t	tmpval = LCD_PORT;	// �|�[�g��Ԃ���U�ϐ��ɓ����

	// �������ރ|�[�g����U���Ƃ�
	tmpval &= ~( LCD_RW | LCD_RS | LCD_DATMASK );

	// RS�r�b�g�R�}���h���[�h�ł���΃N���A�̂܂�
	if( RS_MODE_DATA == mode ){
		tmpval |= LCD_RS;	// RS�r�b�g�̓f�[�^���[�h�ł���Η��Ă�
	}

	// RW�r�b�g�̓N���A��Write���[�h�Ȃ̂ł��̂܂�

	// �f�[�^�r�b�g�Z�b�g(����4bit�̂݃Z�b�g����̃}�X�N)
	tmpval |= ( LCD_DATMASK & data );

	// �|�[�g�ɏ�������
	LCD_PORT = tmpval;

	// �C�l�[�u���p���X�o��
	LCD_PORT |= LCD_E;
	// �����쓮���͂����ŃC�l�[�u���p���X���ȏ�҂K�v������͂�
	LCD_PORT &= ~LCD_E;
	// �����쓮���͂����ŃC�l�[�u���T�C�N�����Ԃ��҂��K�v������͂�
}

void Lcd_setchar( char data )
{
	// ���4bit���M
	lcd_setcmd4( RS_MODE_DATA, LCD_DATMASK & ( data >> 4 ));
	// ����4bit���M
	lcd_setcmd4( RS_MODE_DATA, LCD_DATMASK & data );
	// Busy�`�F�b�N
	lcd_busywait();
}

void Lcd_setstr( char *str )
{
	// �I��(NULL�����܂Ń��[�v)
	while( *str ){
		// 1�������ݒ�
		Lcd_setchar( *str );
		str++;
	}
}

void lcd_busywait( void )
{
	uint8_t	bf = LCD_D7;	// Busy�t���O����������Ԃ̕ϐ���������

	// RS�r�b�g�̓N���A�ŃR�}���h���[�h
	LCD_PORT &= ~( LCD_RS );

	// Read���[�h��RW�r�b�g�𗧂Ă�
	LCD_PORT |= ( LCD_RW );

	// �f�[�^�r�b�g����͂ɐݒ�
	LCD_DDR	&= ~( LCD_DATMASK );

	// Busy�t���O��������܂Ń��[�v
	while( bf ){
		// �C�l�[�u���p���X�o��(���4bit)
		LCD_PORT |= LCD_E;
		// �����쓮���͂����ŃC�l�[�u���p���X���ȏ�҂K�v������͂�
		bf = LCD_PIN & ( 1 << LCD_D7 );	// Busy�t���O�̓ǂݏo��
		LCD_PORT &= ~LCD_E;
		// �����쓮���͂����ŃC�l�[�u���T�C�N�����Ԃ��҂��K�v������͂�

		// �C�l�[�u���p���X�o��(����4bit/4bit���[�h�Ȃ̂�2�񑗂�K�v�����邪�����ł͓��ɂ��邱�Ƃ͂Ȃ�)
		LCD_PORT |= LCD_E;
		// �����쓮���͂����ŃC�l�[�u���p���X���ȏ�҂K�v������͂�
		LCD_PORT &= ~LCD_E;
		// �����쓮���͂����ŃC�l�[�u���T�C�N�����Ԃ��҂��K�v������͂�
	}

	// �f�[�^�r�b�g���o�͂ɖ߂�
	LCD_DDR	|= ( LCD_DATMASK );
}

void Lcd_setcmd( uint8_t cmd )
{
	// ���4bit���M
	lcd_setcmd4( RS_MODE_COMMAND, LCD_DATMASK & ( cmd >> 4 ));
	// ����4bit���M
	lcd_setcmd4( RS_MODE_COMMAND, LCD_DATMASK & cmd );
	// Busy�`�F�b�N
	lcd_busywait();
}

void Lcd_setpos( int8_t x, int8_t y )
{
	int8_t val = 0x80;	// �R�}���h�R�[�h�ݒ�

	if( x ) val |= 0x40;	// �s�w�肪0(1�s��)����Ȃ�������A2�s��bit�𗧂Ă�
	val |= ( y & 0x0f );	// ��w���4bit��ݒ肷��

	// �R�}���h���M
	Lcd_setcmd( val );
}
