/*
 * MMA7455.h
 *
 *  Created on: May 17, 2018
 *      Author: Jake Moomaw
 */

#include "stm32l4xx_hal.h"
#include <math.h>

#ifndef MMA7455_H_
#define MMA7455_H_

#define JSC_MMA7455_I2C_ADDRESS					((uint8_t)0x1d<<1)		//Address is 7-bit so it needs to be shifted
#define JSC_MMA7455_I2C_PORT					hi2c1					//Change this to the i2c bus that you are using
#define JSC_MMA7455_10bit_x_reg_low				((uint8_t)0x00)
#define JSC_MMA7455_10bit_x_reg_high			((uint8_t)0x01)
#define JSC_MMA7455_10bit_y_reg_low				((uint8_t)0x02)
#define JSC_MMA7455_10bit_y_reg_high			((uint8_t)0x03)
#define JSC_MMA7455_10bit_z_reg_low				((uint8_t)0x04)
#define JSC_MMA7455_10bit_z_reg_high			((uint8_t)0x05)
#define JSC_MMA7455_8bit_x_reg					((uint8_t)0x06)
#define JSC_MMA7455_8bit_y_reg					((uint8_t)0x07)
#define JSC_MMA7455_8bit_z_reg					((uint8_t)0x08)
#define JSC_MMA7455_init_reg					((uint8_t)0x16)
#define JSC_MMA7455_interrupt_reg				((uint8_t)0x17)
#define JSC_MMA7455_control1_reg				((uint8_t)0x18)
#define JSC_MMA7455_control2_reg				((uint8_t)0x19)
#define JSC_MMA7455_freefall_limit_reg			((uint8_t)0x1a)

#define JSC_MMA7455_2g_init_byte				((uint8_t)0x15)			//2G saturation measurement mode
#define JSC_MMA7455_4g_init_byte				((uint8_t)0x19)			//4G saturation measurement mode
#define JSC_MMA7455_8g_init_byte				((uint8_t)0x11)			//8G saturation measurement mode
#define JSC_MMA7455_freefall_init_byte			((uint8_t)0x12)			//8G saturation level detection mode for freefall interrupt
#define JSC_MMA7455_freefall_control1_byte		((uint8_t)0x40)			//Integer mode freefall detection
#define JSC_MMA7455_freefall_control2_byte		((uint8_t)0x01)			//Freefall instead of level detection
#define JSC_MMA7455_freefall_limit_byte			((uint8_t)0x07)			//Freefall trips at .5g
#define JSC_MMA7455_interrupt_clear_byte		((uint8_t)0x03)			//Clears both interrupts even though only INT1 is used
#define JSC_MMA7455_interrupt_reset_byte		((uint8_t)0x00)			//Reactivates both interrupts

void JSC_MMA7455_Init_2G();												//These three init functions set the saturation
void JSC_MMA7455_Init_4G();												//level for the MMA7455. 2G saturates easiest
void JSC_MMA7455_Init_8G();												//but 8G is less accurate for angle measurement
void JSC_MMA7455_Init_Freefall();
void JSC_MMA7455_Interrupt_Reset();										//Must be called after ISR to clear flags
int8_t JSC_MMA7455_Read_8Bit_X();
int8_t JSC_MMA7455_Read_8Bit_Y();
int8_t JSC_MMA7455_Read_8Bit_Z();
int16_t JSC_MMA7455_Read_10Bit_X();
int16_t JSC_MMA7455_Read_10Bit_Y();
int16_t JSC_MMA7455_Read_10Bit_Z();
float JSC_MMA7455_Pitch();
float JSC_MMA7455_Roll();
extern I2C_HandleTypeDef JSC_MMA7455_I2C_PORT;

#endif /* MMA7455_H_ */
