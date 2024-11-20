/**
 * Project: PID Controller
 *
 * @file pid_controller.c
 *
 * @brief test code for dual output PID controller
 *
 * @author _art_of_electronics_
 * @date 2024.11.20
 * @version v1.0
 *
 * @copyright 2024 _art_of_electronics_
*/

/*****************************************************************************
                                    INCLUDES
*****************************************************************************/

#include <stdint.h>
#include "pid_controller.h"


/*****************************************************************************
                          PRIVATE DEFINES / MACROS
*****************************************************************************/

#define MIN_OUTPUT_VALUE  (0U)
#define MAX_OUTPUT_VALUE  (100U)
#define TIME_INTERVAL     (1000.0f)


/*****************************************************************************
                     PRIVATE STRUCTS / ENUMS / VARIABLES
*****************************************************************************/

static float sPidIntegralSum = 0.0f;
static float sPidPreviousError = 0.0f;
static float sPidTimeElapsed = 0.0f;


/*****************************************************************************
                         PRIVATE FUNCTION DECLARATION
*****************************************************************************/


/*****************************************************************************
                           INTERFACE IMPLEMENTATION
*****************************************************************************/

void PID_Update(float kp, float ki, float kd, float timeout,                  \
                float currentTemp, float targetTemp, uint32_t timeDelta_ms,  \
                uint8_t *outputHeater, uint8_t *outputFan)
{

  float error = targetTemp - currentTemp;
  sPidTimeElapsed += timeDelta_ms / TIME_INTERVAL;

  ///< Check timeout
  if (sPidTimeElapsed > timeout)
  {
    *outputHeater = MIN_OUTPUT_VALUE;
    *outputFan = MIN_OUTPUT_VALUE;
    return;
  }

  ///< Integral term
  sPidIntegralSum += error * (timeDelta_ms / TIME_INTERVAL);

  ///< Derivative term
  float derivative = (error - sPidPreviousError) / (timeDelta_ms / TIME_INTERVAL);

  ///< PID output
  float output = (kp * error) + (ki * sPidIntegralSum) + (kd * derivative);

  ///< Determine heater and fan output
  if (output > MIN_OUTPUT_VALUE)
  {
    *outputHeater = (output > MAX_OUTPUT_VALUE) ? MAX_OUTPUT_VALUE : (uint8_t)output;
    *outputFan = MIN_OUTPUT_VALUE;
  }
  else
  {
    *outputFan = (-output > MAX_OUTPUT_VALUE) ? MAX_OUTPUT_VALUE : (uint8_t)(-output);
    *outputHeater = MIN_OUTPUT_VALUE;
  }

  ///< Update previous error
  sPidPreviousError = error;
}


/*****************************************************************************
                        PRIVATE FUNCTION IMPLEMENTATION
*****************************************************************************/


/******************************** END OF FILE *******************************/
