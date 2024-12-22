![image](https://github.com/user-attachments/assets/f91cdaf6-a262-42b7-b6de-4dcbd9d75a09)
FPGA Z7-20 Reaction Time Game
This project is a reaction time testing game designed using the FPGA Z7-20 board. It integrates a ZYNQ Processing System (PS) and features an SPI-controlled OLED display to provide a complete and responsive gaming experience. Below are the key highlights of the project:

Overview
Purpose: Measures user reaction time by displaying a random number on an OLED screen. The user must input the corresponding number using a keypad, and the system calculates the reaction speed.
Platform: FPGA Z7-20 with ZYNQ Processing System.
Display: PMOD OLED (32x128 resolution).
Input: Keypad for number input.
Key Features
SPI Interface Implementation:

The OLED display is driven by a custom SPI protocol implementation, enabling efficient communication and precise control.
ASCII characters are mapped to OLED pixels via ROM-based look-up tables.
Random Number Generation:

Random numbers are generated and displayed on the OLED, ensuring a dynamic and unpredictable gameplay experience.
User Input Handling:

Inputs from the keypad are captured and compared with the displayed number.
Reaction speed is calculated and displayed in real-time.
Optimized Hardware Design:

Designed to reduce latency and ensure real-time performance.
Efficient use of PS and PL for task distribution.
Challenges and Improvements
Initialization Issues: Resolved problems related to display initialization.
Scaling Font Sizes: Faced challenges in merging multiple ROM characters to increase text size on the OLED.
Future Scope: Enhance display capabilities and optimize input processing for even faster response times.
