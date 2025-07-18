# 🌡️ STM32 DHT11 Temperature & Humidity Warning System

A simple embedded system using STM32F4 and DHT11 to display and monitor temperature/humidity with warning LED and configuration mode via buttons and LCD1602A display.

## 📸 Demo
![Demo GIF/IMAGE](./assets/demo.gif)

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
| LCD Display    | LCD1602A (I2C or parallel) |
| Buttons        | 3x tactile switches   |
| Warning        | LED (with resistor)   |

---

## 🗂️ Folder Structure
