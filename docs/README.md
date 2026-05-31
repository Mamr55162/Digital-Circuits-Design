# Documentation:
## Contens:
  This folder provides documentation for each concept and each circuit implemented in the code, it will also provide a guide to the multisim simulations, each circuit will have its own document explaining the circuitry and the logic behind it, the code and the multisim design of it.
## Topics:
- Half Adder
- Full Adder
- 4-bit Full Adder *74HC283* IC
- 4-bit Identity Comparator
- 4-bit Magnitude Comparator *74HC85* IC
- 4-to-1 Multiplexer
- 8-to-1 Muliplexer *74HC151* IC
- 2-to-4 Decoder
- 4-to-16 Decoder *74HC154* IC
- Constructing a MUX using Decoder
- BCD-to-Decimal Decoder *74HC42* IC
- BCD-to-7-Segment Decoder *74LS47D* IC
## Notes and Concepts:
  In the code I used `vector<bool>`, while it is not the best practice, it is okay, you can replace it by `bitset<2>` if you wish. The *Wire* class can be ignored and you can directly create boolean variable and assign a boolean value to it, but the class is made for abstraction, so that if you don't want to deal directly with built-in data types of the language you can use *Wire* as a user-defined data type that is more abstract. When using the functions in the code, use indicators for the output for abstraction, better than directly outputting the result which is not a real-life scenario.
  In multisim simulations, simpler designs use Vcc 5.0V source while more complex designs *(usually ICs)* use interactive digital constant source for abstraction's sake to focus the design on the IC functionality. Mainly used indicators are LEDs while other indicators also can be used such as digital probes, fuse lamps and voltmeters. I encourage you to try Logic Analyzer and Logic converter on each design for better comprehension of the material.
