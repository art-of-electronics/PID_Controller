import pytest
import ctypes
import json
import os


TEMPERATURE_THRESHOLD = 0.5
test_results = []

# Load the shared C library
pid_lib = ctypes.CDLL('./libpid_controller.so')

# Define the C function argument and return types
pid_lib.PID_Update.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_float, ctypes.c_float, ctypes.c_uint32,
    ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(ctypes.c_uint8)
]

# Simulation function to emulate system behavior
def temperature_model(temp, heater, fan, ambient_temp=20, heater_rate=0.5, fan_rate=0.7):
    """Simulates temperature changes based on heater and fan outputs."""
    temp += heater * heater_rate - fan * fan_rate
    temp += (ambient_temp - temp) * 0.01  # Gradual approach to ambient temperature
    return temp

# Python wrapper for C PID function
def pid_update(kp, ki, kd, timeout, current_temp, target_temp, delta_time_ms):
    heater = ctypes.c_uint8(0)
    fan = ctypes.c_uint8(0)

    pid_lib.PID_Update(kp, ki, kd, timeout,
                       current_temp, target_temp, delta_time_ms,
                       ctypes.byref(heater), ctypes.byref(fan))

    return heater.value, fan.value

# Pytest fixture for reusable setup
@pytest.fixture
def pid_params():
    return {"kp": 1.4, "ki": 0.15, "kd": 0.02, "timeout": 100.0}

# Basic test case
def test_pid_c(pid_params):
    heater, fan = pid_update(
        pid_params["kp"], pid_params["ki"], pid_params["kd"],
        pid_params["timeout"], 20.0, 60.0, 100
    )
    assert heater > 0
    assert fan == 0

# Test with simulation and plotting
def test_pid_with_simulation(pid_params):
    current_temp = 20.0
    target_temp = 60.0
    delta_time_ms = 100
    history = []

    for _ in range(200):  # Simulate 20 seconds (200 x 100ms)
        heater, fan = pid_update(
            pid_params["kp"], pid_params["ki"], pid_params["kd"],
            pid_params["timeout"], current_temp, target_temp, delta_time_ms
        )
        current_temp = temperature_model(current_temp, heater, fan)
        history.append(round(current_temp, 3))
    test_results.append({
    "kp": pid_params["kp"],
    "ki": pid_params["ki"],
    "kd": pid_params["kd"],
    "history": history,
    "target_temp": target_temp
    })

    # Assert the system stabilizes near the target
    assert abs(history[-1] - target_temp) < TEMPERATURE_THRESHOLD

# Parameterized tuning test
@pytest.mark.parametrize("kp, ki, kd", [
    (0.6, 0.2, 0.05),
    (2.0, 0.3, 0.06),
    (1.0, 0.1, 0.01),
    (1.5, 0.2, 0.02),
])
def test_pid_tuning(kp, ki, kd):
    current_temp = 20.0
    target_temp = 60.0
    delta_time_ms = 100
    history = []

    for _ in range(200):
        heater, fan = pid_update(kp, ki, kd, 100.0, current_temp, target_temp, delta_time_ms)
        current_temp = temperature_model(current_temp, heater, fan)
        history.append(round(current_temp,3))
        
    test_results.append({
    "kp": kp,
    "ki": ki,
    "kd": kd,
    "history": history,
    "target_temp": target_temp
    })

    results_file = "simulation_results.json"
    with open(results_file, "w") as f:
        json.dump(test_results, f, indent=4)
    
    steady_state_error = abs(history[-1] - target_temp)
    print(f"Gains: kp={kp}, ki={ki}, kd={kd}, Error: {steady_state_error}")
    assert steady_state_error < TEMPERATURE_THRESHOLD
