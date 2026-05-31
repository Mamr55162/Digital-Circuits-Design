# Indicators class
## Overview
This class focuses on implementing different types of indicators to use on the outputs of the circuits.
## Code Walkthrough & Multisim guide
* **LED**: the most important and most used indicator, implemented as a function that accepts a value and outputs a scentence whether the LED is on or off, see the following code snippet:
```cpp
 static void LED(bool val)
   {
      if (val == true)
         cout << "LED is ON\n";
      else
         cout << "LED is OFF\n";
   }
```
It is easy, readable and manageable. For Multisim design, a LED can be found in Diodes section in a subsection named LED, you can use LED_blue, LED_red, etc., each color has its own forward voltage and current, make sure to connect the anode of the LED with the output of the circuit or gate you are using, and connect the cathode directly to a ground.

<img width="257" height="167" alt="image" src="https://github.com/user-attachments/assets/c88be3ce-b746-42aa-b952-40865b10cb3e" />

***A LED_red connected to the output of an AND gate and a ground.***

* **Binary & BinaryVector**: both indicators are used to print binary digits, Binary is used to print one binary bit, while BinaryVector prints a series of binary bits, while it is not applicable in Multisim, it is still useful in viewing the output as a serial of binary digits, rathet than waveform, see the following code snippet:
```cpp
static void Binary(bool val)
   {
      cout << val;
   }
   static void BinaryVector(const vector<bool>& bits)
   {
      for (bool b : bits)
         cout << b;

      cout << '\n';
   }
```
* **Probe**: Another famously used indicator especially for digital circuits, it is implemented as a function that outputs a string either HIGH or LOW, see the following code snippet:
```cpp
static void Probe(bool val)
   {
      cout << (val ? "HIGH" : "LOW") << endl;
   }
```
A digital probe can be used in Multisim in the Indicator section under the Probe subsection, named PROBE_DIG, this is the suitable probe for digital circuits, it connects directly to the output of a gate or circuit with no ground connections, it lights when the wire is HIGH and turns off when the wire is LOW.

<img width="251" height="232" alt="Screenshot 2026-06-01 015457" src="https://github.com/user-attachments/assets/255a87ad-6f6b-445d-bbe6-ae826073a2ce" />

***A PROBE_DIG connected to XOR gate with HIGH output.***

<img width="265" height="247" alt="Screenshot 2026-06-01 015503" src="https://github.com/user-attachments/assets/83a42e4f-c4be-4465-aaff-35f44aa41b12" />

***A PROBE_DIG connected to XOR gate with LOW output.***

* **Decimal**: This indicator is more useful and applicable than the Binary and BinaryVector ones, it accepts BCD digits and converts them to a decimal value using the function ``` to_ulong() ``` from the header ``` <bitset> ```, which accepts a binary value and returns the decimal equivalent, see the following code snippet:
```cpp
static void Decimal(const bitset<4>& bcd)
   {
      cout << bcd.to_ulong() << endl;
   }
```
A decimal display in multisim can be found in Indicators section, under the HEX_DISPLAY subsection, named DCD_HEX, it can be found with many colors variations, it has four pins, each pin connects to a certain binary digit to output the number in decimal, this indicator is most applicable in decoder, MUX circuits and ICs that has too many output pins.

<img width="648" height="534" alt="image" src="https://github.com/user-attachments/assets/09c0aa30-0ba0-4131-a142-fc946ece459a" />

***A DCD_HEX connected to four interactive digital constant sources starting from the right (LSB) to the left (MSB) it outputs 5 which is decoded from 0101.***

* **Bus**: A bus is a multiple wires carrying information, here our information are binary digits, so we output the bus binary digits to see the information it is carrying, see the following code snippet:
```cpp
static void Bus(const bitset<4>& bus)
   {
      cout << "BUS: " << bus << endl;
   }
```
>[!NOTE]
>Do not reverse the LED polarity and always check for a ground connection.
>You don't have to worry about resistors, fuses or any circuitry protection in Multisim, just be abstract.
>When connecting a displayer (HEX or BCD) do not mix up the digits, know your MSB and your LSB.
