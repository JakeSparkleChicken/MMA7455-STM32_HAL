# MMA7455-STM32_HAL
MMA7455 library for the STM32 HAL environment

The I2C bus that you are using needs to be modified in both MMA7455.h and MMA7455.c unless you are using hi2c1.

It provides for both 8-bit and 10-bit value reads, freefall detection mode, and roll and pitch readings.
