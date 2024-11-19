# PID_Controller
1) open terminal and run make which will create shared library module libpid_controller.so from pid_controller.c
2) edit PID parameters according to your needs
3) run pytest pid_test_tuning.py - it will run a set of PID settings as test cases and preliminary evaluate which settings exceed given threshold. It will also produce a json file with collected simulation data
4) run python3 plot_results.py to visualise json data in form of a plot
