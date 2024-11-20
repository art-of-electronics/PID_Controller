# Copyright 2024 _art_of_electronics_

import os
import json
import matplotlib.pyplot as plt


# Load simulation results
results_file = "simulation_results.json"
if not os.path.exists(results_file):
    raise FileNotFoundError(f"Results file {results_file} not found. Run tests first.")

with open(results_file, "r") as f:
    test_results = json.load(f)

# Combined plot
plt.figure(figsize=(10, 6))
for result in test_results:
    plt.plot(result["history"], label=f"Kp={result['kp']}, Ki={result['ki']}, Kd={result['kd']}")
plt.axhline(y=test_results[0]["target_temp"], color='r', linestyle='--', label="Target")
plt.legend()
plt.xlabel("Time (steps)")
plt.ylabel("Temperature (°C)")
plt.title("Combined PID Simulation Results")
plt.savefig("combined_pid_simulation.png")
plt.show()

print("Plot saved successfully")
