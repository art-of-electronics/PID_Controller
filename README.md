# PID Controller evaluation with pytest

A fun approach to complex problem - evaluation of a PID controller. PID controllers tend to give headaches to some people. No wonder, beacause writing an algorythm for PID controller alone can be challenging, not to mention tuning a PID controller, estimating robustness vs agressiveness, estimating overshoot, etc. Instead of complex calculation of the transmittance of process / plant and following PID tuning I decided to try an embedded software engineer approach: write code and test it. So I wrote a simple C code for PID controller (which, with minor tweaks, can be imported directly into an embedded system) and used pytest as evaluation tool. Neat, right? 

---

### 1) create .so file

Open terminal and type:

``` bash
make
```
This will create a shared library module `libpid_controller.so` from `pid_controller.c`

---

### 2) edit parameters

This step is optional. However, if you go to line `83` of `pid_test_tuning.py`, you can edit the sample test set.

``` python
# Parameterized tuning test
@pytest.mark.parametrize("kp, ki, kd", [
    (0.6, 0.2, 0.05),
    (2.0, 0.3, 0.06),
    (1.0, 0.1, 0.01),
    (1.5, 0.2, 0.02),
])
```

Here you can add a set of `(kp, ki, kd)` values, delete any unnecessary set or edit any value. If you want 100 test sets - go ahead :)

You can also change the default temperature threshold on line `7` of `pid_test_tuning.py`, which, by default is set to 0.5&deg;C

``` python
TEMPERATURE_THRESHOLD = 0.5
```

---

### 3) run pytest

Open terminal and type:

``` bash
pytest pid_test_tuning.py
```

This will simulate all the sets you entered in step 2). Basically, you will have a preliminary evaluation of `(kp, ki, kd)` sets in form of pass / fail. 

Fail condition is considered when simulated system temperature differs from target temperature by &pm;TEMPERATURE_THRESHOLD

**In this step pytest should also create `simulation_results.json` with all the data that were collected during simulation.**

---

### 4) generate a plot

Having a pass / fail is good, but even better would be to have a visualization in form of a figure with plotted data, right? In that case open terminal and type:

``` bash
python3 plot_results.py
```

This will generate a `combined_pid_simulation.png` figure with all the results collected from the pytest simulation. Something like this:

![](/combined_pid_simulation.png)

---
