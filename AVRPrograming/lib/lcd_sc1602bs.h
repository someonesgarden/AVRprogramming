#include <avr/io.h>
#include <util/delay.h>

#define LCD_PORT	PORTB	// LCD表示に使用する出力レジスタ
#define LCD_PIN		PINB	// LCD表示に使用する入力レジスタ
#define LCD_DDR		DDRB	// LCD表示に使用する方向レジスタ

#define LCD_D4	(1<<0)	// D4ビット位置
#define LCD_D5	(1<<1)	// D5ビット位置
#define LCD_D6	(1<<2)	// D6ビット位置
#define LCD_D7	(1<<3)	// D7ビット位置
#define LCD_E	(1<<4)	// Eビット位置
#define LCD_RW	(1<<5)	// R/Wビット位置
#define LCD_RS	(1<<6)	// RSビット位置
#define LCD_DATMASK	0x0F	// データビットマスク

// RSモード列挙型定義
typedef enum _tagRSMode{
	RS_MODE_COMMAND,	// コマンドモード
	RS_MODE_DATA,		// データモード
} RSMode;

void delay_ms( int time );

void Lcd_init( void );
void lcd_setcmd4( RSMode mode, uint8_t data );
void Lcd_setchar( char data );
void Lcd_setstr( char *str );
void lcd_busywait( void );
void Lcd_setcmd( uint8_t cmd );
void Lcd_setpos( int8_t x, int8_t y );