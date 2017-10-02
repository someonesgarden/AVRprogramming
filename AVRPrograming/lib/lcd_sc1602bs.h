#include <avr/io.h>
#include <util/delay.h>

#define LCD_PORT	PORTB	// LCD�\���Ɏg�p����o�̓��W�X�^
#define LCD_PIN		PINB	// LCD�\���Ɏg�p������̓��W�X�^
#define LCD_DDR		DDRB	// LCD�\���Ɏg�p����������W�X�^

#define LCD_D4	(1<<0)	// D4�r�b�g�ʒu
#define LCD_D5	(1<<1)	// D5�r�b�g�ʒu
#define LCD_D6	(1<<2)	// D6�r�b�g�ʒu
#define LCD_D7	(1<<3)	// D7�r�b�g�ʒu
#define LCD_E	(1<<4)	// E�r�b�g�ʒu
#define LCD_RW	(1<<5)	// R/W�r�b�g�ʒu
#define LCD_RS	(1<<6)	// RS�r�b�g�ʒu
#define LCD_DATMASK	0x0F	// �f�[�^�r�b�g�}�X�N

// RS���[�h�񋓌^��`
typedef enum _tagRSMode{
	RS_MODE_COMMAND,	// �R�}���h���[�h
	RS_MODE_DATA,		// �f�[�^���[�h
} RSMode;

void delay_ms( int time );

void Lcd_init( void );
void lcd_setcmd4( RSMode mode, uint8_t data );
void Lcd_setchar( char data );
void Lcd_setstr( char *str );
void lcd_busywait( void );
void Lcd_setcmd( uint8_t cmd );
void Lcd_setpos( int8_t x, int8_t y );