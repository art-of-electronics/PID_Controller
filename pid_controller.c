#include <stdint.h>

void PID_Update(float kp, float ki, float kd, float timeout,
                float current_temp, float target_temp,
                uint32_t delta_time_ms,
                uint8_t *heater_output, uint8_t *fan_output) {
    static float integral_sum = 0.0f;
    static float prev_error = 0.0f;
    static float elapsed_time = 0.0f; // Accumulate elapsed time for timeout handling

    float error = target_temp - current_temp;
    elapsed_time += delta_time_ms / 1000.0f;

    // Check timeout
    if (elapsed_time > timeout) {
        *heater_output = 0;
        *fan_output = 0;
        return;
    }

    // Integral term
    integral_sum += error * (delta_time_ms / 1000.0f);

    // Derivative term
    float derivative = (error - prev_error) / (delta_time_ms / 1000.0f);

    // PID output
    float output = (kp * error) + (ki * integral_sum) + (kd * derivative);

    // Determine heater and fan output
    if (output > 0) {
        *heater_output = (output > 100) ? 100 : (uint8_t)output;
        *fan_output = 0;
    } else {
        *fan_output = (-output > 100) ? 100 : (uint8_t)(-output);
        *heater_output = 0;
    }

    // Update previous error
    prev_error = error;
}

