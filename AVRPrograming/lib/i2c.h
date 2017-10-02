// Functions for i2c communication
#include <avr/io.h>
#include "pinDefines.h"

void initI2C(void);
    /* Sets pullups and initializes bus speed to 100kHz (at FCPU=8MHz) */

void i2cWaitForComplete(void);   /* Waits until the hardware sets the TWINT flag */

void i2cStart(void);       /* Sends a start condition (sets TWSTA) */
void i2cStop(void);        /* Sends a stop condition (sets TWSTO) */

void i2cSend(uint8_t data);		/* Loads data, sends it out, waiting for completion */
void i2cSendAck(uint8_t data);
void i2cSendNoAck(uint8_t data);

uint8_t i2cReadAck(void);    /* Read in from slave, sending ACK when done (sets TWEA) */
uint8_t i2cReadNoAck(void);  /* Read in from slave, sending NOACK when done (no TWEA) */
			  


uint8_t I2C_LCD_SendControls(uint8_t data[], uint8_t num);

uint8_t I2C_LCD_SendString(uint8_t addr, char *data, uint8_t num);
uint8_t I2C_LCD_SendString_Rep(uint8_t addr, char *data, uint8_t num);

uint8_t I2C_LCD_SendByte(uint8_t addr, uint8_t byte);

uint8_t I2C_LCD_Init();

void i2cError();
