# Controller Handle

**Part 1: Building vehicle hardware and connecting STM32F407 to Motor, H Bridge**

![doan1](https://github.com/tanphanzero/Cotroller_Handle/assets/85573204/5f946395-3e9f-494e-b795-c4cd4ca1ff05)

**Part 2: Using the MANUAL vehicle control handle**

- Use the Input Capture function of STM32F407 (Timer) to read the pulse width from the Handle and output the PWM pulse to the H-bridge to control the Motor
- Functions:
  * CHANNEL 2: forward and backward
  * CHANNEL 4: left and right
  * CHANNEL 5: switch 3 mode

![image](https://github.com/tanphanzero/project2/assets/85573204/2b451071-9688-4a6b-9a7f-f8c52dbdaec3)

- File source: in1/core/src/main.c



