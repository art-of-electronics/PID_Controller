import pytest
from pid_controller import PIDController  # Assuming the PID logic is in pid_controller.py

@pytest.fixture
def pid():
    # Initialize PID with example gains and a 100s timeout
    return PIDController(kp=1.5, ki=0.1, kd=0.01, timeout=100)

def test_pid_heater_activation(pid):
    heater, fan = pid.update(current_temp=20, target_temp=25, delta_time_ms=100)
    assert heater > 0  # Heater should activate
    assert fan == 0    # Fan should be off

def test_pid_fan_activation(pid):
    heater, fan = pid.update(current_temp=30, target_temp=25, delta_time_ms=100)
    assert fan > 0     # Fan should activate
    assert heater == 0 # Heater should be off

def test_pid_no_action(pid):
    heater, fan = pid.update(current_temp=25, target_temp=25, delta_time_ms=100)
    assert heater == 0 # No action required
    assert fan == 0    # No action required

def test_pid_timeout(pid):
    for _ in range(1000):  # Simulate 100 seconds in 100ms steps
        pid.update(current_temp=20, target_temp=25, delta_time_ms=100)

    heater, fan = pid.update(current_temp=20, target_temp=25, delta_time_ms=100)
    assert heater == 0  # Heater should be off after timeout
    assert fan == 100   # Fan should activate fully after timeout

