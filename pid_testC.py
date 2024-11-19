import ctypes

# Load the shared library
pid_lib = ctypes.CDLL('./libpid_controller.so')

# Define function argument and return types
pid_lib.PID_Update.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_float, ctypes.c_float, ctypes.c_uint32,
    ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(ctypes.c_uint8)
]

# Test the PID function
def test_pid_c():
    heater = ctypes.c_uint8(0)
    fan = ctypes.c_uint8(0)

    pid_lib.PID_Update(1.5, 0.1, 0.01, 100.0,
                       20.0, 25.0, 100,
                       ctypes.byref(heater), ctypes.byref(fan))

    assert heater.value > 0
    assert fan.value == 0
