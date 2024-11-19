#include "stdint.h"
#include "stdbool.h"

// PID structure
typedef struct {
    float kp;        // Proportional gain
    float ki;        // Integral gain
    float kd;        // Derivative gain
    float integral;  // Integral sum
    float prev_error;// Previous error value
    float timeout;   // Timeout value in seconds
    uint32_t elapsed_time; // Elapsed time in ms
} PID_Controller;

// Initialize PID controller
void PID_Init(PID_Controller *pid, float kp, float ki, float kd, float timeout) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
    pid->timeout = timeout * 1000; // Convert to ms
    pid->elapsed_time = 0;
}

// PID control function
void PID_Update(PID_Controller *pid, float current_temp, float target_temp, 
                uint8_t *heater_output, uint8_t *fan_output, uint32_t delta_time_ms) {
    float error = target_temp - current_temp;

    // Timeout check
    if (pid->elapsed_time >= pid->timeout) {
        *heater_output = 0;
        *fan_output = 100; // Activate full cooling
        return;
    }

    // Proportional term
    float proportional = pid->kp * error;

    // Integral term
    pid->integral += error * (delta_time_ms / 1000.0f);
    float integral = pid->ki * pid->integral;

    // Derivative term
    float derivative = pid->kd * (error - pid->prev_error) / (delta_time_ms / 1000.0f);

    // PID calculation
    float output = proportional + integral + derivative;

    // Update previous error
    pid->prev_error = error;

    // Control outputs
    if (output > 0) {
        *heater_output = (output > 100) ? 100 : (uint8_t)output;
        *fan_output = 0;
    } else {
        *fan_output = (-output > 100) ? 100 : (uint8_t)(-output);
        *heater_output = 0;
    }

    // Increment elapsed time
    pid->elapsed_time += delta_time_ms;
}





#include "stm32f4xx_hal.h"

PID_Controller pid;
uint8_t heater_output = 0;
uint8_t fan_output = 0;

// Initialize PID in main function
void System_Init() {
    PID_Init(&pid, 2.5f, 1.0f, 0.1f, 100.0f); // Example gains and timeout
}

// SysTick callback every 100ms
void HAL_SYSTICK_Callback(void) {
    static uint32_t last_time = 0;
    uint32_t current_time = HAL_GetTick();
    uint32_t delta_time = current_time - last_time;

    if (delta_time >= 100) { // Execute every 100ms
        float measured_temp = movingAvgBuffer; // Use your moving average value
        float target_temp = 25.0f; // Example target temperature

        PID_Update(&pid, measured_temp, target_temp, &heater_output, &fan_output, delta_time);

        // Use heater_output and fan_output to adjust PWM or GPIO
        last_time = current_time;
    }
}

