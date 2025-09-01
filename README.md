# Alien Shooter Game (C & Assembly)

## 📖 Project Description
This project implements a simple **Alien Shooter game** on the **PYNQ-Z1 development board** using an **LED matrix shield**.  
It was developed as part of the *Introduction to Embedded Systems* course to practice **embedded C programming**, hardware control, and basic assembly.

The game runs directly on the ARM Cortex-A9 processor of the PYNQ board, with the FPGA fabric used only for I/O routing.  
It demonstrates the use of **interrupts**, **memory-mapped I/O**, and **PWM** in an embedded environment.

---

## 🎯 Learning Goals
- Learn the basics of **embedded C programming** with hardware.
- Understand how to read and use **datasheets** for programming peripherals.
- Practice using the **Xilinx SDK** (debugger, software loading).
- Implement a small **game loop** with real-time interaction.
- Use **interrupts** (timer + GPIO) to handle input and timing.

---

## 🕹️ Game Features
- **Ship movement**: controlled by board buttons.
- **Alien movement**: automated enemy across the matrix.
- **Firing bullets**: ship can shoot moving projectiles.
- **Scoring system**:
  - Hit alien → increase score
  - Miss → increase miss counter
- **Game over & restart**: clear ending condition with option to play again.
- **Score display**: results shown on LEDs or matrix.

---

## ⚙️ Hardware & Software
- **Board**: PYNQ-Z1 Development Board
  - Zynq-7000 SoC
  - Dual-core ARM Cortex-A9
  - Artix-7 FPGA fabric
- **Shield**: 8×8 RGB LED matrix
- **Software**:
  - Xilinx SDK 2019.1
  - C for main game logic
  - Assembly (for LED blinker extra task)

---

## 🚀 How It Works
- **Ship & alien rendering**: controlled via LED matrix channels.
- **Input**: GPIO button interrupts (falling edge).
- **Timing**: Timer interrupts at 10 Hz / 800 Hz for game updates.
- **Output**: PWM-driven LED matrix to show sprites.
- **Extra**: LD0–LD3 LEDs blink using an assembly routine.
