/*
 * MMA7455.c
 *
 *  Created on: May 19, 2018
 *      Author: Jake Moomaw
 */

#include "stm32l4xx_hal.h"
#include "MMA7455.h"
#define PI  (3.14159265F);


I2C_HandleTypeDef *i2c_instance = &hi2c1;			//Change this to the I2C bus that you are using

/* One of these four init functions needs to be called after the MX_I2Cx_Init() in the board initialization
 * portion of your program.  When in freefall detection mode, it will not provide other measurement information.
 */

void MMA7455_Init_2G()
{
	uint8_t init_byte = MMA7455_2g_init_byte;
	HAL_I2C_Mem_Write(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_init_reg, 1, &init_byte, 1, 10000);
}
void MMA7455_Init_4G()
{
	uint8_t init_byte = MMA7455_4g_init_byte;
	HAL_I2C_Mem_Write(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_init_reg, 1, &init_byte, 1, 10000);
}
void MMA7455_Init_8G()
{
	uint8_t init_byte = MMA7455_8g_init_byte;
	HAL_I2C_Mem_Write(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_init_reg, 1, &init_byte, 1, 10000);
}
void MMA7455_Init_Freefall()
{
	uint8_t init_byte = MMA7455_freefall_init_byte;
	uint8_t control1_byte = MMA7455_freefall_control1_byte;
	uint8_t control2_byte = MMA7455_freefall_control2_byte;
	uint8_t freefall_limit_byte = MMA7455_freefall_limit_byte;
	HAL_I2C_Mem_Write(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_init_reg, 1, &init_byte, 1, 10000);
	HAL_I2C_Mem_Write(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_control1_reg, 1, &control1_byte, 1, 10000);
	HAL_I2C_Mem_Write(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_control2_reg, 1, &control2_byte, 1, 10000);
	HAL_I2C_Mem_Write(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_freefall_limit_reg, 1, &freefall_limit_byte, 1, 10000);

}

/* This function needs to be called at the end of your ISR in order to clear the interrupt flags in register $17.
 * If you don't call this after the interrupt, the freefall detection will only work once during each power cycle.
 */

void MMA7455_IOCtl_Interrupt_Reset()
{
	uint8_t clear_byte = MMA7455_interrupt_clear_byte;
	uint8_t reset_byte = MMA7455_interrupt_reset_byte;
	HAL_I2C_Mem_Write(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_interrupt_reg, 1, &clear_byte, 1, 10000);
	HAL_I2C_Mem_Write(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_interrupt_reg, 1, &reset_byte, 1, 10000);
}

/* These three functions read the 8-bit registers for the three axes.  This is not as accurate as the 10-bit mode,
 * but faster since they each only read one register as opposed to two.
 */

int8_t MMA7455_Read_8Bit_X(void)
{
	uint8_t xval_8bit;
	HAL_I2C_Mem_Read(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_8bit_x_reg, 1, &xval_8bit, 1, 10000);
	int8_t signed_xval = xval_8bit;
	return signed_xval;
}
int8_t MMA7455_Read_8Bit_Y(void)
{
	uint8_t yval_8bit;
	HAL_I2C_Mem_Read(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_8bit_y_reg, 1, &yval_8bit, 1, 10000);
	int8_t signed_yval = yval_8bit;
	return signed_yval;
}
int8_t MMA7455_Read_8Bit_Z(void)
{
	uint8_t zval_8bit;
	HAL_I2C_Mem_Read(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_8bit_z_reg, 1, &zval_8bit, 1, 10000);
	int8_t signed_zval = zval_8bit;
	return signed_zval;
}

/* These three functions call the two registers that supply the 10-bit values for the three axes.  Since it reads two
 * registers per axis, it is slower but more accurate.
 */

int16_t MMA7455_Read_10Bit_X(void)
{
	uint8_t xval_10bit_low;
	uint8_t xval_10bit_high;
	HAL_I2C_Mem_Read(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_10bit_x_reg_low, 1, &xval_10bit_low, 1, 10000);
	HAL_I2C_Mem_Read(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_10bit_x_reg_low, 1, &xval_10bit_high, 1, 10000);
	int16_t xval_10bit = ((int16_t)xval_10bit_high << 8) | xval_10bit_low;
	return xval_10bit;
}
int16_t MMA7455_Read_10Bit_Y(void)
{
	uint8_t yval_10bit_low;
	uint8_t yval_10bit_high;
	HAL_I2C_Mem_Read(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_10bit_y_reg_low, 1, &yval_10bit_low, 1, 10000);
	HAL_I2C_Mem_Read(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_10bit_y_reg_low, 1, &yval_10bit_high, 1, 10000);
	int16_t yval_10bit = ((int16_t)yval_10bit_high << 8) | yval_10bit_low;
	return yval_10bit;
}
int16_t MMA7455_Read_10Bit_Z(void)
{
	uint8_t zval_10bit_low;
	uint8_t zval_10bit_high;
	HAL_I2C_Mem_Read(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_10bit_z_reg_low, 1, &zval_10bit_low, 1, 10000);
	HAL_I2C_Mem_Read(i2c_instance, MMA7455_I2C_ADDRESS, MMA7455_10bit_z_reg_low, 1, &zval_10bit_high, 1, 10000);
	int16_t zval_10bit = ((int16_t)zval_10bit_high << 8) | zval_10bit_low;
	return zval_10bit;
}

// The pitch and roll functions are based off of the 8-bit reads.

float MMA7455_Read_Pitch(void)
{
	float MMA7455_pitch_denom;
	float MMA7455_pitch_sqrt;
	float MMA7455_pitch;
	float MMA7455_pitch_num;
	int8_t MMA7455_sign = 1;
	if (MMA7455_Read_8Bit_Z() < 0)
	{
		MMA7455_sign = -1;
	}
	MMA7455_pitch_denom = MMA7455_Read_8Bit_Y() * MMA7455_Read_8Bit_Y() + MMA7455_Read_8Bit_Z() * MMA7455_Read_8Bit_Z();
	MMA7455_pitch_sqrt = sqrt(MMA7455_pitch_denom);
	MMA7455_pitch_num = MMA7455_Read_8Bit_X();
	MMA7455_pitch = atan2 (MMA7455_pitch_num, MMA7455_sign * MMA7455_pitch_sqrt) * 180 / PI;
	return MMA7455_pitch;

}
float MMA7455_Read_Roll(void)
{
	float MMA7455_roll_denom;
	float MMA7455_roll_sqrt;
	float MMA7455_roll;
	float MMA7455_roll_num;
	int8_t MMA7455_sign = 1;
	if (MMA7455_Read_8Bit_Z() < 0)
	{
		MMA7455_sign = -1;
	}
	MMA7455_roll_denom = MMA7455_Read_8Bit_X() * MMA7455_Read_8Bit_X() + MMA7455_Read_8Bit_Z() * MMA7455_Read_8Bit_Z();
	MMA7455_roll_sqrt = sqrt(MMA7455_roll_denom);
	MMA7455_roll_num = MMA7455_Read_8Bit_Y();
	MMA7455_roll = atan2 (MMA7455_roll_num, MMA7455_sign * MMA7455_roll_sqrt) * 180 / PI;
	return MMA7455_roll;

}
