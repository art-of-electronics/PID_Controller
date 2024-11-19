class PIDController:
    def __init__(self, kp, ki, kd, timeout):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.integral = 0.0
        self.prev_error = 0.0
        self.timeout = timeout * 1000  # ms
        self.elapsed_time = 0  # ms

    def update(self, current_temp, target_temp, delta_time_ms):
        if self.elapsed_time >= self.timeout:
            return 0, 100  # Full cooling if timeout is reached

        error = target_temp - current_temp

        # Proportional term
        proportional = self.kp * error

        # Integral term
        self.integral += error * (delta_time_ms / 1000.0)
        integral = self.ki * self.integral

        # Derivative term
        derivative = self.kd * (error - self.prev_error) / (delta_time_ms / 1000.0)

        # PID output
        output = proportional + integral + derivative

        # Save the current error for the next cycle
        self.prev_error = error

        # Update elapsed time
        self.elapsed_time += delta_time_ms

        # Return heater and fan output
        if output > 0:
            return min(output, 100), 0  # Heater, Fan
        else:
            return 0, min(-output, 100)  # Heater, Fan
