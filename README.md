## AVRprogramming
ATMEL STUDIOの設定方法。

#### CKDIV8   
Device ProgrammingのFusesにCKDIV8のチェックボックスがある。
ここからMakeFileを修正できる。
ATMega168は、初期状態で８MHｚなので：
- チェックが入っている（さらに8分の一。1MHz）： F_CPU=1000000UL(1x10^6)
- チェックが入ってない（そのまま。8MHz）： F_CPU=8000000UL(8x10^6)

#### 外部クロックから16MHzを供給する場合
１）CKDIV8のチェックをはずす。
２）SUT_CKSELをEXTXOSC_8MHZ_XX_16KCK_14CK_65MS

#### atmega168pの場合の※デフォルト値は以下：
- Int.RC Osc 8MHz; Start-up Time PWRDWN/RESET; 6CK/14CK+0ms;
-  表記は「INTRCOSC_8MHZ_6CK_14CK_0MS」

#### 実行の方法
各機能を持ったprogramがAVRタイプ名のかかれたフォルダ内にあるので使用時にexec内にaddしてmain.cppで起動する