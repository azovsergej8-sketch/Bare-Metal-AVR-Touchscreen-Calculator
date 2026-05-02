# Bare-Metal AVR Touchscreen Calculator
Bare-metal C calculator for ATmega2560 featuring a custom ILI9341 SPI display driver and interrupt-driven I2C touch interface.
A high-performance engineering calculator developed in **Pure C** for the ATmega2560 (Arduino Mega) microcontroller. This project is designed for the Wokwi simulation environment and operates without the Arduino framework or any high-level libraries.

## 🚀 Key Technical Features

### 1. Low-Level Display Driver (ILI9341)
* **Direct Register Access:** Communication with the ILI9341 controller is implemented via raw SPI register manipulation.
* **Optimized SPI Interface:** Configured for maximum throughput using the `SPI2X` (double speed) bit.
* **Graphic Engine:** Includes custom implementations for:
    * Primitive rendering (rectangles, circles using Midpoint Circle Algorithm).
    * Bitmapped font rendering from custom glyph arrays.

### 2. Interrupt-Driven I2C Touch Driver
* **Non-blocking Architecture:** The touch interface is implemented using the TWI (I2C) peripheral with a complete **Finite State Machine (FSM)** inside the Interrupt Service Routine (`ISR(TWI_vect)`).
* **Async Processing:** Coordinates are processed only when the hardware signals data availability, ensuring CPU efficiency.

### 3. Computational Logic & Data Structures
* **Stack-Based Arithmetic:** Implements a custom Stack data structure to handle operation precedence and operand storage.
* **Memory Management:** Utilizes `volatile` qualifiers for cross-context (ISR/Main) variables to ensure memory coherency.

## 🛠 Hardware Configuration (Wokwi)
The system integrates an **Arduino Mega 2560** with an **ILI9341** capacitive touch display.

| Peripheral | Pins |
| :--- | :--- |
| **SPI (Display)** | MOSI (51), SCK (52), CS (10), D/C (9), RST (8) |
| **I2C (Touch)** | SDA (20), SCL (21) |

## 📂 Project Structure
* `main.c` — System initialization and main control loop.
* `spi.h` — Hardware SPI configuration and data transmission.
* `i2c.h` — TWI interrupt logic and touch coordinate decoding.
* `draw_functions.h` — Core graphics library and font definitions.
* `logic.h` — Calculator state machine and arithmetic functions.
* `stack.h` — Custom LIFO data structure implementation.
* [cite_start]`Button.h` — UI element definitions and coordinate mapping.

## 📋 How to Run
1. Open the project in the [Wokwi Simulator](https://wokwi.com).
2. Ensure all `.h` and `.c` files are uploaded to the project environment.
3. Use `diagram.json` to automatically set up the wiring.
4. Compile and run the simulation.

---
**Author:** Sergey Azov,

Candidate for Intern FPGA Design role at YADRO.
