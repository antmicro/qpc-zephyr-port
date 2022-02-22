# Port of the QP/C framework to Zephyr

This repository contains a port of the [QP/C framework](https://github.com/QuantumLeaps/qpc/) to the [Zephyr RTOS](https://github.com/zephyrproject-rtos/zephyr).

The [port](port) directory contains an implementation of the port itself. The [apps](apps) directory contains sample QP/C applications:
- [hello](apps/hello) - the most basic demo, it just initializes QP/C and prints a hello message.
- [blinky](apps/blinky) - a QP/C application with a single thread that simulates a blinking LED by printing messages to UART in intervals.
- [dpp](apps/dpp) -  a multi-threaded implementation of the dining philosophers problem.

Note: the blinky and hello samples were imported from QP/C source and are licensed under GPL.
