/**
 * Project: PID Controller
 *
 * @file pid_controller.h
 *
 * @brief headers for dual output PID controller code
 *
 * @author _art_of_electronics_
 * @date 2024.11.20
 * @version v1.0
 *
 * @copyright 2024 _art_of_electronics_
*/

#pragma once 

/*****************************************************************************
                                    INCLUDES
*****************************************************************************/

// #include "main.h"

/*****************************************************************************
                       PUBLIC DEFINES / MACROS / ENUMS
*****************************************************************************/


/*****************************************************************************
                         PUBLIC INTERFACE DECLARATION
*****************************************************************************/


/**@brief Calculates NTC temperature based on ADC value
 *
 * @param [in] kp - proportional coefficient of PID controller
 * @param [in] ki - integral coefficient of PID controller
 * @param [in] kp - derivative coefficient of PID controller
 * @param [in] timeout - time value after which PID controller shuts off oututs
 * @param [in] currentTemp - temperature measured from NTC
 * @param [in] targetTemp - target temperature
 * @param [in/out] outputHeater - pointer to variable for driving PWM output of heater
 * @param [in/out] outputFan - pointer to variable for driving PWM output of fan
 */
void PID_Update(float kp, float ki, float kd, float timeout,                  \
                float currentTemp, float targetTemp, uint32_t timeDelta_ms,  \
                uint8_t *outputHeater, uint8_t *outputFan);

/******************************** END OF FILE *******************************/
