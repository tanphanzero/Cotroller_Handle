# Controller Handle

## Đồ án 1: BK TPHCM

**Phần 1: Xây dựng phần cứng xe và kết nối STM32F407 với Motor, Cầu H**
![da1](https://github.com/tanphanzero/project2/assets/85573204/833cc78d-0281-4ba0-9676-fb88219e6a0a)

**Phần 2: Sử dụng tay cầm điều khiển xe MANUAL**

- Sử dụng chức năng Input Capture của STM32F407 (Timer) để đọc độ rộng xung từ Tay cầm và xuất xung PWM cho cầu H điều khiển Motor
- Các chức năng:
  * CHANNEL 2: tiến lùi
  * CHANNEL 4: trái phải
  * CHANNEL 5: switch 3 mode

![image](https://github.com/tanphanzero/project2/assets/85573204/2b451071-9688-4a6b-9a7f-f8c52dbdaec3)

- File source: in1/core/src/main.c



