# 🌡️ STM32 DHT11 Temperature & Humidity Warning System

A simple embedded system using STM32F4 and DHT11 to display and monitor temperature/humidity with warning LED and configuration mode via buttons and LCD1602A display.

## 📸 Demo
[![Watch the demo](https://img.youtube.com/vi/YOUR_VIDEO_ID/0.jpg)]([https://www.youtube.com/watch?v=YOUR_VIDEO_ID](https://youtu.be/YxfCUHxHrRw))

---

## 🚀 Features

- 📟 Display temperature and humidity on LCD1602A
- 🔘 Button-based mode switch:
  - Show data
  - Set temperature threshold
  - Set humidity threshold
- 🚨 Warning LED if threshold exceeded
- ⌨️ Interrupt-based button handling (GPIO external interrupts)
- ⏲️ Uses precise delay_us for DHT11 protocol

---

## 🔧 Hardware Used

| Device         | Model                 |
|----------------|-----------------------|
| MCU            | STM32F407VG           |
| Temp/Humi Sensor | DHT11                |
| LCD Display    | LCD1602A (GPIO 4-Bits) |
| Buttons        | 3x tactile switches    |
| Warning        | LED                    |
|  LCD Contrast  | Register 10k           |
---

## 🗂️ Folder Structure
<pre lang="no-highlight"><code>. ├── Core │ ├── Inc │ │ ├── dht11.h │ │ ├── gpio.h │ │ ├── lcd.h │ │ ├── main.h │ │ ├── stm32f4xx_hal_conf.h │ │ └── stm32f4xx_it.h │ └── Src │ ├── dht11.c │ ├── gpio.c │ ├── lcd.c │ ├── main.c │ ├── stm32f4xx_hal_msp.c │ ├── stm32f4xx_it.c │ ├── syscalls.c │ └── system_stm32f4xx.c ├── Drivers │ └── CMSIS └── Startup </code></pre>
