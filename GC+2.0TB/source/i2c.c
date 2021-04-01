#include <gccore.h>
#include <ogc/lwp_watchdog.h>
#include <ogc/machine/processor.h>
#include <stdio.h>
#include "gpio.h"
#include "i2c.h"

#define GPIO_SCL (1<<14)
#define GPIO_SDA (1<<15)
#define GPIO_I2C (GPIO_SCL | GPIO_SDA)

void i2c_udelay(int us) {
	u64 now = gettime();
	while(ticks_to_microsecs(gettime() - now) < us);
}

inline void i2c_setSCL(u8 s) {
	u32 val = HW_GPIOB_OUT & (~GPIO_SCL);
	val |= (s << 14);
	HW_GPIOB_OUT = val;
}

inline void i2c_setSDA(u8 s) {
	u32 val = HW_GPIOB_OUT & (~GPIO_SDA);
	val |= (s << 15);
	HW_GPIOB_OUT = val;
}

inline u8 i2c_getSDA(void) {
	if (HW_GPIOB_IN & GPIO_SDA) return 1;
	return 0;
}

inline u8 i2c_getSCL(void) {
	if (HW_GPIOB_IN & GPIO_SCL) return 1;
	return 0;
}

inline void i2c_setSCLDir(u8 dir) {
	u32 timeout = 100000;
	u32 val = HW_GPIOB_DIR & (~GPIO_SCL);
	val |= (dir << 14);
	HW_GPIOB_DIR = val;

	if (dir == 0) {
		do {
			i2c_udelay(10);
			//timeout--;
		} while(!i2c_getSCL() && timeout);
	}
}

inline void i2c_setSDADir(u8 dir) {
	u32 val = HW_GPIOB_DIR & (~GPIO_SDA);
	val |= (dir << 15);
	HW_GPIOB_DIR = val;
}

void i2c_init(void) {
	//Set both scl and sda as input
	i2c_setSCL(0);
	i2c_setSDA(0);

	i2c_setSDADir(0); //Let SDA high
	i2c_udelay(10);
	i2c_setSCLDir(0);
	i2c_udelay(10);
}

void i2c_start(void) {
	i2c_setSDADir(1); //Set SDA low
	i2c_udelay(10);

	i2c_setSCLDir(1); //Set SCL low
	i2c_udelay(10);
}

void i2c_stop(void) {
	i2c_setSDADir(1); //Set SDA low
	i2c_udelay(10);

	i2c_setSCLDir(0);
	i2c_udelay(10);

	i2c_setSDADir(0); //Let SDA high
	i2c_udelay(10);
}

void i2c_restart(void) {
	i2c_setSDADir(0); //Let SDA high
	i2c_udelay(10);

	i2c_setSCLDir(0);
	i2c_udelay(10);

	i2c_start();
}

void i2c_ack(void) {
	i2c_setSDADir(1); //Set SDA low
	i2c_udelay(10);

	i2c_setSCLDir(0);
	i2c_udelay(10);

	i2c_setSCLDir(1);
	i2c_udelay(10);
}

void i2c_nack(void) {
	i2c_setSDADir(0); //Let SDA high
	i2c_udelay(10);

	i2c_setSCLDir(0);
	i2c_udelay(10);

	i2c_setSCLDir(1);
	i2c_udelay(10);
}

u8 i2c_sendByte(u8 byte) {
	int i;

	for (i = 0; i < 8; i++)
	{
		if (byte & 0x80) i2c_setSDADir(0); //Let SDA high
		else i2c_setSDADir(1); //Set SDA low
		i2c_udelay(10);

		i2c_setSCLDir(0);
		i2c_udelay(10);
		
		i2c_setSCLDir(1);
		i2c_udelay(10);

		byte <<= 1;
	}

	i2c_setSDADir(0); //Let SDA high
	i2c_udelay(10);

	i2c_setSCLDir(0);
	i2c_udelay(10);

	if (i2c_getSDA())
	{
		return 0; //No ACK
	}

	i2c_setSCLDir(1);
	i2c_udelay(10);

	return 1;
}

u8 i2c_getByte(void) {
	int i;

	u8 byte = 0;

	for (i = 0; i < 8; i++)
	{
		byte <<= 1;
		
		i2c_setSDADir(0); //Let SDA high
		i2c_udelay(10);

		i2c_setSCLDir(0);
		i2c_udelay(10);

		if (i2c_getSDA()) byte |= 1;
		i2c_udelay(10);

		i2c_setSCLDir(1);
		i2c_udelay(10);

	}

	return byte;
}

u8 i2c_read8(u8 addr, u8 reg, u8* error) {
	u8 output;
	u8 ret;
	if (error != NULL) *error = 0;

	i2c_start();

	ret = i2c_sendByte(addr); //Write
	if (!ret)
	{
		printf("Error at line: %d\n", __LINE__);
		if (error != NULL) *error = 1;
		return 0;
	}

	ret = i2c_sendByte(reg);
	if (!ret)
	{
		printf("Error at line: %d\n", __LINE__);
		if (error != NULL) *error = 1;
		return 0;
	}

	i2c_restart();

	ret = i2c_sendByte(addr | 1); //Read
	if (!ret)
	{
		printf("Error at line: %d\n", __LINE__);
		if (error != NULL) *error = 1;
		return 0;
	}

	output = i2c_getByte();

	i2c_nack();

	i2c_stop();

	return output;
}

u16 i2c_read16(u8 addr, u8 reg, u8* error) {
	u16 output;
	u8 ret;
	if (error != NULL) *error = 0;

	i2c_start();

	ret = i2c_sendByte(addr); //Write
	if (!ret)
	{
		if (error != NULL) *error = 1;
		return 0;
	}

	ret = i2c_sendByte(reg);
	if (!ret)
	{
		if (error != NULL) *error = 1;
		return 0;
	}

	i2c_restart();

	ret = i2c_sendByte(addr | 1); //Read
	if (!ret)
	{
		if (error != NULL) *error = 1;
		return 0;
	}

	output = i2c_getByte();
	i2c_ack();
	output |= (i2c_getByte() << 8);

	i2c_nack();

	i2c_stop();

	return output;
}

void i2c_readBuffer(u8 addr, u8 reg, u8* error, u8* buffer, u16 len) {
	u16 output;
	u8 ret;
	int i;
	if (error != NULL) *error = 0;

	i2c_start();

	ret = i2c_sendByte(addr); //Write
	if (!ret)
	{
		printf("Error at line: %d\n", __LINE__);
		if (error != NULL) *error = 1;
		return;
	}

	ret = i2c_sendByte(reg);
	if (!ret)
	{
		printf("Error at line: %d\n", __LINE__);
		if (error != NULL) *error = 1;
		return;
	}

	i2c_restart();

	ret = i2c_sendByte(addr | 1); //Read
	if (!ret)
	{
		printf("Error at line: %d\n", __LINE__);
		if (error != NULL) *error = 1;
		return;
	}
	for (i = 0; i < len; i++) {
		buffer[i] = i2c_getByte();
		if (i == len - 1) {
			i2c_nack();
		} else {
			i2c_ack();
		}
	}

	i2c_stop();
}

void i2c_write8(u8 addr, u8 reg, u8 value, u8* error) {
	u8 ret;

	if (error != NULL) *error = 0;

	i2c_start();

	ret = i2c_sendByte(addr);
	if (!ret)
	{
		printf("Error at line: %d\n", __LINE__);
		if (error != NULL) *error = 1;
		return;
	}

	ret = i2c_sendByte(reg);
	if (!ret)
	{
		printf("Error at line: %d\n", __LINE__);
		if (error != NULL) *error = 1;
		return;
	}

	ret = i2c_sendByte(value);
	if (!ret)
	{
		printf("Error at line: %d\n", __LINE__);
		if (error != NULL) *error = 1;
		return;
	}

	i2c_stop();
}

void i2c_write16(u8 addr, u8 reg, u16 value, u8* error) {
	u8 ret;

	if (error != NULL) *error = 0;

	i2c_start();

	ret = i2c_sendByte(addr);
	if (!ret)
	{
		if (error != NULL) *error = 1;
		return;
	}

	ret = i2c_sendByte(reg);
	if (!ret)
	{
		if (error != NULL) *error = 1;
		return;
	}

	ret = i2c_sendByte(value & 0xFF);
	if (!ret)
	{
		if (error != NULL) *error = 1;
		return;
	}

	ret = i2c_sendByte((value >> 8) & 0xFF);
	if (!ret)
	{
		if (error != NULL) *error = 1;
		return;
	}

	i2c_stop();
}
