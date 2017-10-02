#include "i2c.h"
#include <util/delay.h>

void i2cError(){
	while(1);
}


void initI2C(void) {
  TWBR = 32;                               /* set bit rate, see p. 242 */
                                     /* 8MHz / (16+2*TWBR*1) ~= 100kHz */
  TWCR |= (1 << TWEN);                                       /* enable */
}

void i2cWaitForComplete(void) {
  loop_until_bit_is_set(TWCR, TWINT);
}

void i2cStart(void) {
  TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTA));
  i2cWaitForComplete();
}

void i2cStop(void) {
  TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTO));
}

uint8_t i2cReadAck(void) {
  TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWEA));
  i2cWaitForComplete();
  return (TWDR);
}

void i2cSendAck(uint8_t data) {
	TWDR = data;
	TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWEA));
	i2cWaitForComplete();
}

void i2cSendNoAck(uint8_t data) {
	TWDR = data;
	TWCR = (_BV(TWINT) | _BV(TWEN));
	i2cWaitForComplete();
}



uint8_t i2cReadNoAck(void) {
  TWCR = (_BV(TWINT) | _BV(TWEN));
  i2cWaitForComplete();
  return (TWDR);
}


void i2cSend(uint8_t data) {
  TWDR = data;
  TWCR = (_BV(TWINT) | _BV(TWEN));                  /* init and enable */
  i2cWaitForComplete();
}

uint8_t I2C_LCD_SendControls(uint8_t data[], uint8_t num)
{
	uint8_t i;

	i2cStart();
	i2cSend(0x7c);
	/*
	if (TW_STATUS != TW_MT_SLA_ACK) {
		return -1;
	}
	*/
	i2cSend(0x00); // Co:0(continuous) RS:0
	/*
	if (TW_STATUS != TW_MT_DATA_ACK) {
		return -1;
	}
	*/

	for (i = 0; i < num; i++) {
		i2cSend(data[i]);
		/*
		if (TW_STATUS != TW_MT_DATA_ACK) {
			return -1;
		}
		*/
		_delay_us(27);
	}

	i2cStop();

	return 0;
}

uint8_t I2C_LCD_SendByte(uint8_t addr, uint8_t byte){
	uint8_t i;

	i2cStart();

	i2cSend(0x7c);

	i2cSend(0x80); // Co:1(one shot) RS:0

	i2cSend(0x80 | addr); // Set DDRAM address

	i2cSend(0x40); // Co:0(continuous) RS:1
	
	  i2cSend('0' + (byte / 100));                        /* Hundreds */
	  _delay_us(27);
	  i2cSend('0' + ((byte / 10) % 10));                      /* Tens */
	  _delay_us(27);
	  i2cSend('0' + (byte % 10));                             /* Ones */
	  _delay_us(27);
	//i2cSend(byte);
	i2cStop();
	return 0;
}


uint8_t I2C_LCD_SendString(uint8_t addr, char *data, uint8_t num)
{
	uint8_t i;

	i2cStart();

	i2cSend(0x7c);
	/*
	if (TW_STATUS != TW_MT_SLA_ACK) {
		return -1;
	}
	*/

	i2cSend(0x80); // Co:1(one shot) RS:0
	/*
	if (TW_STATUS != TW_MT_DATA_ACK) {
		return -1;
	}*/
	

	i2cSend(0x80 | addr); // Set DDRAM address
	/*
	if (TW_STATUS != TW_MT_DATA_ACK) {
		return -1;
	}
	*/

	i2cSend(0x40); // Co:0(continuous) RS:1
	/*
	if (TW_STATUS != TW_MT_DATA_ACK) {
		return -1;
	}*/

	for (i = 0; i < num; i++) {
		i2cSend(data[i]);
		/*
		if (TW_STATUS != TW_MT_DATA_ACK) {
			return -1;
		}
		*/
		_delay_us(27);
	}
	i2cStop();
	return 0;
}


uint8_t I2C_LCD_SendString_Rep(uint8_t addr, char *data, uint8_t num)
{
	uint8_t i;

	//i2cStart();

	i2cSend(0x7c);
	/*
	if (TW_STATUS != TW_MT_SLA_ACK) {
		return -1;
	}
	*/

	i2cSend(0x80); // Co:1(one shot) RS:0
	/*
	if (TW_STATUS != TW_MT_DATA_ACK) {
		return -1;
	}*/
	

	i2cSend(0x80 | addr); // Set DDRAM address
	/*
	if (TW_STATUS != TW_MT_DATA_ACK) {
		return -1;
	}
	*/

	i2cSend(0x40); // Co:0(continuous) RS:1
	/*
	if (TW_STATUS != TW_MT_DATA_ACK) {
		return -1;
	}*/

	for (i = 0; i < num; i++) {
		i2cSend(data[i]);
		/*
		if (TW_STATUS != TW_MT_DATA_ACK) {
			return -1;
		}
		*/
		_delay_us(27);
	}
	return 0;
}


uint8_t I2C_LCD_Init()
{
	uint8_t init_data1[] = {0x38, 0x39, 0x14, 0x70, 0x56, 0x6c};
	uint8_t init_data2[] = {0x38, 0x0C, 0x01};
	uint8_t st;

	_delay_ms(40);

	initI2C();

	st = I2C_LCD_SendControls(init_data1, sizeof(init_data1));
	if (st != 0) {
		return st;
	}
	_delay_ms(200);

	st = I2C_LCD_SendControls(init_data2, sizeof(init_data2));
	if (st != 0) {
		return st;
	}
	_delay_ms(2);

	return 0;
}