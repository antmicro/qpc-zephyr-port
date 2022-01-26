# Port of QPc framework to Zephyr

This repository contains the port of [QPc framework](https://github.com/QuantumLeaps/qpc/) to [Zephyr RTOS](https://github.com/zephyrproject-rtos/zephyr).

The [port](port) directory contains implementation of the port itself. The [apps](apps) directory contains sample QPc applications:
- [hello](apps/hello) - the most basic demo, it just initializes QPc and prints a hello message.
- [blinky](apps/blinky) - a QPc application with a single thread that simulates a blinking LED by printing messages to UART in intervals.
- [dpp](apps/dpp) -  a multi-threaded implementation of the dining philosophers problem.

Note: the blinky and hello samples were imported from QPc source and are licensed under GPL.
