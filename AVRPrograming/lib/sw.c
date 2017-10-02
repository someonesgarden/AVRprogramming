//---------------------------------------------------------------
// スイッチ用関数
#include <avr/io.h>

#define SW_PORT  PORTC		// プルアップ用ポート
#define SW_DDR   DDRC       // ポート入力設定
#define SW_PIN   PINC       // 入力ポート

//---------------------------------------------------------------
// スイッチ初期化
//   引数:初期化するポートのマスク
void sw_init(unsigned char bitmask)
{
 	SW_DDR  &= ~bitmask; 	//input
	SW_PORT |= bitmask; 	//pullup
}


//---------------------------------------------------------------
// スイッチ読み取り
//   引数:検出するポートのマスク
//   戻り値：0=スイッチオフ / 1=スイッチオン
char sw_get(unsigned char bitmask)
{
	if(SW_PIN & bitmask)
		return(0);  // スイッチ押されてない
	else
		return(1);  // スイッチ押されている
}

