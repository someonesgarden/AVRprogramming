/*
	== lcd.c ==
	SC1602BS Charactor LCD (HD44780 compatible) driver library
	written by mojo@uec-koken 2009-5
*/

#include "lcd_sc1602bs.h"


void delay_ms( int time )
{
	// 指定ms分ループ
	while( time-- ){
		_delay_ms( 1 );
	}
}

void Lcd_init( void )
{
	// 最初は全てのポートを出力に設定
	LCD_DDR	|= ( LCD_E | LCD_RW | LCD_RS | LCD_DATMASK );
	// 信号もLowにしておく
	LCD_PORT &= ~( LCD_E | LCD_RW | LCD_RS | LCD_DATMASK );

	// 最初はBusyチェックが効かないので、時間待ちする
	delay_ms( 15 );		// 15ms待ち
	lcd_setcmd4( RS_MODE_COMMAND, 0x3 );	// 8bitモード設定
	delay_ms( 5 );		// 4.1ms以上待ちなので、5ms待ち
	lcd_setcmd4( RS_MODE_COMMAND, 0x3 );	// 8bitモード設定
	delay_ms( 1 );		// 100μs以上待ちなので、とりあえず1ms待っておく
	lcd_setcmd4( RS_MODE_COMMAND, 0x3 );	// 8bitモード設定
	delay_ms( 1 );		// ファンクションセットは39μs以上待ちなので、とりあえず1ms待っておく

	// 初期化シーケンスに書いてある通り、4bitの設定は4bitのみ渡せれば有効なので、1回のみ
	lcd_setcmd4( RS_MODE_COMMAND, 0x2 );	// 4bitモード設定
	delay_ms( 1 );		// ファンクションセットは39μs以上待ちなので、とりあえず1ms待っておく

	// ここからは4bitモード動作なので、8bitの命令を4bitずつ2回に分けて送ってやる
	lcd_setcmd4( RS_MODE_COMMAND, 0x2 );	// ファンクションセット、4bitモード
	lcd_setcmd4( RS_MODE_COMMAND, 0x8 );	// 2Line表示、5*8ドットフォント
	delay_ms( 1 );		// ファンクションセットは39μs以上待ちなので、とりあえず1ms待っておく
	lcd_setcmd4( RS_MODE_COMMAND, 0x0 );	// Display ON/OFF設定
	lcd_setcmd4( RS_MODE_COMMAND, 0xF );	// Display ON / Cursor ON / Blinking ON
	delay_ms( 1 );		// Display ON/OFF設定は39μs以上待ちなので、とりあえず1ms待っておく
	lcd_setcmd4( RS_MODE_COMMAND, 0x0 );	// Clear Displayコマンド
	lcd_setcmd4( RS_MODE_COMMAND, 0x1 );	// スペースでDDRAMが埋められ、アドレスカウンタがDDRAMの00H(先頭)に移動
	delay_ms( 2 );		// Clear Displayコマンドは1.53ms以上待ちなので、2ms待ち
	lcd_setcmd4( RS_MODE_COMMAND, 0x0 );	// Entry Mode
	lcd_setcmd4( RS_MODE_COMMAND, 0x6 );	// インクリメントモード / ディスプレイシフトなし
	delay_ms( 1 );		// Entry Modeコマンドは39μs以上待ちなので、とりあえず1ms待っておく
}

void lcd_setcmd4( RSMode mode, uint8_t data )
{
	uint8_t	tmpval = LCD_PORT;	// ポート状態を一旦変数に入れる

	// 書き込むポートを一旦落とす
	tmpval &= ~( LCD_RW | LCD_RS | LCD_DATMASK );

	// RSビットコマンドモードであればクリアのまま
	if( RS_MODE_DATA == mode ){
		tmpval |= LCD_RS;	// RSビットはデータモードであれば立てる
	}

	// RWビットはクリアでWriteモードなのでそのまま

	// データビットセット(下位4bitのみセットするのマスク)
	tmpval |= ( LCD_DATMASK & data );

	// ポートに書き込み
	LCD_PORT = tmpval;

	// イネーブルパルス出力
	LCD_PORT |= LCD_E;
	// 高速駆動時はここでイネーブルパルス幅以上待つ必要があるはず
	LCD_PORT &= ~LCD_E;
	// 高速駆動時はここでイネーブルサイクル時間を稼ぐ必要があるはず
}

void Lcd_setchar( char data )
{
	// 上位4bit送信
	lcd_setcmd4( RS_MODE_DATA, LCD_DATMASK & ( data >> 4 ));
	// 下位4bit送信
	lcd_setcmd4( RS_MODE_DATA, LCD_DATMASK & data );
	// Busyチェック
	lcd_busywait();
}

void Lcd_setstr( char *str )
{
	// 終了(NULL文字までループ)
	while( *str ){
		// 1文字ずつ設定
		Lcd_setchar( *str );
		str++;
	}
}

void lcd_busywait( void )
{
	uint8_t	bf = LCD_D7;	// Busyフラグが立った状態の変数を初期化

	// RSビットはクリアでコマンドモード
	LCD_PORT &= ~( LCD_RS );

	// ReadモードはRWビットを立てる
	LCD_PORT |= ( LCD_RW );

	// データビットを入力に設定
	LCD_DDR	&= ~( LCD_DATMASK );

	// Busyフラグが落ちるまでループ
	while( bf ){
		// イネーブルパルス出力(上位4bit)
		LCD_PORT |= LCD_E;
		// 高速駆動時はここでイネーブルパルス幅以上待つ必要があるはず
		bf = LCD_PIN & ( 1 << LCD_D7 );	// Busyフラグの読み出し
		LCD_PORT &= ~LCD_E;
		// 高速駆動時はここでイネーブルサイクル時間を稼ぐ必要があるはず

		// イネーブルパルス出力(下位4bit/4bitモードなので2回送る必要があるがここでは特にすることはない)
		LCD_PORT |= LCD_E;
		// 高速駆動時はここでイネーブルパルス幅以上待つ必要があるはず
		LCD_PORT &= ~LCD_E;
		// 高速駆動時はここでイネーブルサイクル時間を稼ぐ必要があるはず
	}

	// データビットを出力に戻す
	LCD_DDR	|= ( LCD_DATMASK );
}

void Lcd_setcmd( uint8_t cmd )
{
	// 上位4bit送信
	lcd_setcmd4( RS_MODE_COMMAND, LCD_DATMASK & ( cmd >> 4 ));
	// 下位4bit送信
	lcd_setcmd4( RS_MODE_COMMAND, LCD_DATMASK & cmd );
	// Busyチェック
	lcd_busywait();
}

void Lcd_setpos( int8_t x, int8_t y )
{
	int8_t val = 0x80;	// コマンドコード設定

	if( x ) val |= 0x40;	// 行指定が0(1行目)じゃなかったら、2行目bitを立てる
	val |= ( y & 0x0f );	// 列指定の4bitを設定する

	// コマンド送信
	Lcd_setcmd( val );
}
