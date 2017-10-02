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
    ２）LOW.SUT_CKSELを「EXTXOSC_8MHZ_XX_16KCK_14CK_65MS」に設定。

#### atmega168pの場合の※デフォルト値は以下：
    - Int.RC Osc 8MHz; Start-up Time PWRDWN/RESET; 6CK/14CK+0ms;
    -  LOW.SUT_CKSELは「INTRCOSC_8MHZ_6CK_14CK_0MS」に設定。

#### 実行の方法
    各機能を持ったprogramがAVRタイプ名のかかれたフォルダ内にあるので使用時にexec内にaddしてmain.cppで起動する

### 各種プログラムについて

#### ATMega168p用プログラム
    mega168p_ADC_LightSensor.c
    mega168p_IR.c
    mega168p_IR_Remote.c
    mega168p_POVDisplay.c
    mega168p_POVTOY.cpp
    mega168p_POVTOY.h
    mega168p_PWM.c
    mega168p_RFID_reader.c
    mega168p_SD1602HUOB.c
    mega168p_SPI_LCD.c
    mega168p_SPI_LCD2.c
    mega168p_SerialIO2.c
    mega168p_USART.c
    mega168p_USART.cpp
    mega168p_amRadio.c
    mega168p_bitTwiddling.c
    mega168p_blinkLED.c
    mega168p_cylonEyes.cpp
    mega168p_dcMotorWorkout.c
    mega168p_dds.c
    mega168p_dds_adsr.c
    mega168p_dds_fatSaw.c
    mega168p_digitalInput.c
    mega168p_footStepDetector.c
    mega168p_hBridge.c
    mega168p_helloInterrupt.c
    mega168p_i2c-masterReceiver_8MHz.c
    mega168p_i2cThermoMeter.c
    mega168p_i2c_keypad4x4_master_sender.c
    mega168p_i2c_slaveTransmitter_8MHz.c
    mega168p_i2c_to_LCD.c
    mega168p_ir_remocon.c
    mega168p_joyStick2D.c
    mega168p_keypad4x4.c
    mega168p_ledgame.c
    mega168p_pinChangeInterrupt.c
    mega168p_progMem.c
    mega168p_pwmOnAnyPin.c
    mega168p_pwmTimer.c
    mega168p_reactionTimer.c
    mega168p_serialIO.c
    mega168p_serialOrgan.c
    mega168p_servoSundial.c
    mega168p_servoSundial.h
    mega168p_servoWorkout.c
    mega168p_showingOffBits.cpp
    mega168p_slowScope.c
    mega168p_spiEEPROM.c
    mega168p_stopWatch.c
    mega168p_talkingVoltmeter.c
    mega168p_talkingVoltmeter.h
    mega168p_timerAudio.c
    mega168p_voltMeter.c

#### ATTiny2313用プログラム
    tiny2313_LEDtest.c
    tiny2313_PWM.c
    tiny2313_simpleTimer.c
    tiny2313_switchCounter.c