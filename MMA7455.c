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

void JSC_MMA7455_Init_2G()
{
	uint8_t init_byte = JSC_MMA7455_2g_init_byte;
	HAL_I2C_Mem_Write(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_init_reg, 1, &init_byte, 1, 10000);
}
void JSC_MMA7455_Init_4G()
{
	uint8_t init_byte = JSC_MMA7455_4g_init_byte;
	HAL_I2C_Mem_Write(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_init_reg, 1, &init_byte, 1, 10000);
}
void JSC_MMA7455_Init_8G()
{
	uint8_t init_byte = JSC_MMA7455_8g_init_byte;
	HAL_I2C_Mem_Write(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_init_reg, 1, &init_byte, 1, 10000);
}
void JSC_MMA7455_Init_Freefall()
{
	uint8_t init_byte = JSC_MMA7455_freefall_init_byte;
	uint8_t control1_byte = JSC_MMA7455_freefall_control1_byte;
	uint8_t control2_byte = JSC_MMA7455_freefall_control2_byte;
	uint8_t freefall_limit_byte = JSC_MMA7455_freefall_limit_byte;
	HAL_I2C_Mem_Write(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_init_reg, 1, &init_byte, 1, 10000);
	HAL_I2C_Mem_Write(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_control1_reg, 1, &control1_byte, 1, 10000);
	HAL_I2C_Mem_Write(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_control2_reg, 1, &control2_byte, 1, 10000);
	HAL_I2C_Mem_Write(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_freefall_limit_reg, 1, &freefall_limit_byte, 1, 10000);

}

/* This function needs to be called at the end of your ISR in order to clear the interrupt flags in register $17.
 * If you don't call this after the interrupt, the freefall detection will only work once during each power cycle.
 */

void JSC_MMA7455_Interrupt_Reset()
{
	uint8_t clear_byte = JSC_MMA7455_interrupt_clear_byte;
	uint8_t reset_byte = JSC_MMA7455_interrupt_reset_byte;
	HAL_I2C_Mem_Write(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_interrupt_reg, 1, &clear_byte, 1, 10000);
	HAL_I2C_Mem_Write(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_interrupt_reg, 1, &reset_byte, 1, 10000);
}

/* These three functions read the 8-bit registers for the three axes.  This is not as accurate as the 10-bit mode,
 * but faster since they each only read one register as opposed to two.
 */

int8_t JSC_MMA7455_Read_8Bit_X(void)
{
	uint8_t JSC_8bit_xval;
	HAL_I2C_Mem_Read(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_8bit_x_reg, 1, &JSC_8bit_xval, 1, 10000);
	int8_t JSC_signed_xval = JSC_8bit_xval;
	return JSC_signed_xval;
}
int8_t JSC_MMA7455_Read_8Bit_Y(void)
{
	uint8_t JSC_8bit_yval;
	HAL_I2C_Mem_Read(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_8bit_y_reg, 1, &JSC_8bit_yval, 1, 10000);
	int8_t JSC_signed_yval = JSC_8bit_yval;
	return JSC_signed_yval;
}
int8_t JSC_MMA7455_Read_8Bit_Z(void)
{
	uint8_t JSC_8bit_zval;
	HAL_I2C_Mem_Read(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_8bit_z_reg, 1, &JSC_8bit_zval, 1, 10000);
	int8_t JSC_signed_zval = JSC_8bit_zval;
	return JSC_signed_zval;
}

/* These three functions call the two registers that supply the 10-bit values for the three axes.  Since it reads two
 * registers per axis, it is slower but more accurate.
 */

int16_t JSC_MMA7455_Read_10Bit_X(void)
{
	uint8_t JSC_10bit_xval_low;
	uint8_t JSC_10bit_xval_high;
	HAL_I2C_Mem_Read(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_10bit_x_reg_low, 1, &JSC_10bit_xval_low, 1, 10000);
	HAL_I2C_Mem_Read(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_10bit_x_reg_low, 1, &JSC_10bit_xval_high, 1, 10000);
	int16_t JSC_10bit_xval = ((int16_t)JSC_10bit_xval_high << 8) | JSC_10bit_xval_low;
	return JSC_10bit_xval;
}
int16_t JSC_MMA7455_Read_10Bit_Y(void)
{
	uint8_t JSC_10bit_yval_low;
	uint8_t JSC_10bit_yval_high;
	HAL_I2C_Mem_Read(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_10bit_y_reg_low, 1, &JSC_10bit_yval_low, 1, 10000);
	HAL_I2C_Mem_Read(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_10bit_y_reg_low, 1, &JSC_10bit_yval_high, 1, 10000);
	int16_t JSC_10bit_yval = ((int16_t)JSC_10bit_yval_high << 8) | JSC_10bit_yval_low;
	return JSC_10bit_yval;
}
int16_t JSC_MMA7455_Read_10Bit_Z(void)
{
	uint8_t JSC_10bit_zval_low;
	uint8_t JSC_10bit_zval_high;
	HAL_I2C_Mem_Read(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_10bit_z_reg_low, 1, &JSC_10bit_zval_low, 1, 10000);
	HAL_I2C_Mem_Read(i2c_instance, JSC_MMA7455_I2C_ADDRESS, JSC_MMA7455_10bit_z_reg_low, 1, &JSC_10bit_zval_high, 1, 10000);
	int16_t JSC_10bit_zval = ((int16_t)JSC_10bit_zval_high << 8) | JSC_10bit_zval_low;
	return JSC_10bit_zval;
}

// The pitch and roll functions are based off of the 8-bit reads.

float JSC_MMA7455_Pitch(void)
{
	float JSC_MMA7455_pitch_denom;
	float JSC_MMA7455_pitch_sqrt;
	float JSC_MMA7455_pitch;
	float JSC_MMA7455_pitch_num;
	int8_t JSC_MMA7455_sign = 1;
	if (JSC_MMA7455_Read_8Bit_Z() < 0)
	{
		JSC_MMA7455_sign = -1;
	}
	JSC_MMA7455_pitch_denom = JSC_MMA7455_Read_8Bit_Y() * JSC_MMA7455_Read_8Bit_Y() + JSC_MMA7455_Read_8Bit_Z() * JSC_MMA7455_Read_8Bit_Z();
	JSC_MMA7455_pitch_sqrt = sqrt(JSC_MMA7455_pitch_denom);
	JSC_MMA7455_pitch_num = JSC_MMA7455_Read_8Bit_X();
	JSC_MMA7455_pitch = atan2 (JSC_MMA7455_pitch_num, JSC_MMA7455_sign * JSC_MMA7455_pitch_sqrt) * 180 / PI;
	return JSC_MMA7455_pitch;

}
float JSC_MMA7455_Roll(void)
{
	float JSC_MMA7455_roll_denom;
	float JSC_MMA7455_roll_sqrt;
	float JSC_MMA7455_roll;
	float JSC_MMA7455_roll_num;
	int8_t JSC_MMA7455_sign = 1;
	if (JSC_MMA7455_Read_8Bit_Z() < 0)
	{
		JSC_MMA7455_sign = -1;
	}
	JSC_MMA7455_roll_denom = JSC_MMA7455_Read_8Bit_X() * JSC_MMA7455_Read_8Bit_X() + JSC_MMA7455_Read_8Bit_Z() * JSC_MMA7455_Read_8Bit_Z();
	JSC_MMA7455_roll_sqrt = sqrt(JSC_MMA7455_roll_denom);
	JSC_MMA7455_roll_num = JSC_MMA7455_Read_8Bit_Y();
	JSC_MMA7455_roll = atan2 (JSC_MMA7455_roll_num, JSC_MMA7455_sign * JSC_MMA7455_roll_sqrt) * 180 / PI;
	return JSC_MMA7455_roll;

}
