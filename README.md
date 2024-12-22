![image](https://github.com/user-attachments/assets/f91cdaf6-a262-42b7-b6de-4dcbd9d75a09)
# Reaction Time Game on FPGA Z7-20  

This project implements a reaction time game using the **FPGA Z7-20** development board, featuring a custom SPI-controlled OLED display and a ZYNQ Processing System (PS). The game challenges users to quickly match a displayed random number by pressing the corresponding keypad button, measuring their reaction speed.

---

## Features  
- **Platform**: FPGA Z7-20 with ZYNQ Processing System (PS).  
- **Display**: PMOD OLED (32x128 resolution) controlled via custom SPI interface.  
- **Input**: Keypad for user interaction.  
- **Game Mechanism**:  
  - Random number generation and display on the OLED.  
  - Keypad input handling and real-time reaction speed calculation.  

---
### Custom SPI Protocol Implementation  
![image](https://github.com/user-attachments/assets/134d80dc-1f02-4ffc-8c13-f9a63b08d6f8)

To drive the PMOD OLED display, a custom SPI interface was implemented with the following features:  

- **Initialization Sequence**:  
  - The SPI controller initializes the OLED display by sending a sequence of commands to configure the display settings (e.g., contrast, orientation).  
  - Timing and control signals are carefully managed to meet the OLED's specifications.  

- **Data Transmission**:  
  - ASCII characters are converted to their corresponding pixel patterns using a ROM-based look-up table.  
  - These pixel patterns are sent to the OLED through the SPI interface in a structured format.  

- **Efficient Communication**:  
  - Data is transmitted in 8-bit chunks, adhering to the SPI protocol's clock and timing requirements.  
  - Control signals (e.g., Chip Select, Data/Command) are toggled dynamically to switch between command and data modes.  

- **Custom Font Rendering**:  
  - Each ASCII character is pre-defined in ROM as a 2D pixel array.  
  - The SPI controller retrieves and renders these arrays on the OLED, ensuring clear and accurate text display.  

This implementation demonstrates precise hardware control and highlights the efficiency of custom SPI solutions for display interfacing.

![image](https://github.com/user-attachments/assets/8adfa134-dcfd-4d2b-994a-15b1425903d2) ![image](https://github.com/user-attachments/assets/a303e59e-5b1c-440e-a508-471d0f5e6d66)
