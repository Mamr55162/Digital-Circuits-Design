# Full Adder
## Overview
  Full Adder circuit, unlike half adder, accepts three binary digits and produces two binary digits, sum and carry, Unlike a Half Adder, a Full Adder accepts a carry input, allowing multiple Full Adders to be cascaded for multi-bit binary addition.
## Truth Table & Boolean Expressions

| A | B | C | Sum | Cin |
| - | - | - | --- | ----- |
| 0 | 0 | 0 |  0  |   0   |
| 0 | 0 | 1 |  1  |   0   |
| 0 | 1 | 0 |  1  |   0   |
| 0 | 1 | 1 |  0  |   1   |
| 1 | 0 | 0 |  1  |   0   |
| 1 | 0 | 1 |  0  |   1   |
| 1 | 1 | 0 |  0  |   1   |
| 1 | 1 | 1 |  1  |   1   |

Sum = A $\oplus$ B $\oplus$ Cin

Cout = AB + (A $\oplus$ B)Cin

## Circuit Structure
![Full Adder](https://www.build-electronic-circuits.com/wp-content/uploads/2022/10/fullAdder-1.png)

## Code Walkthrough
  The code in full adder is nearly the same as half adder except for the Carry input, here is the code snippet
```C++
    bool sum = Gates::XOR(Gates::XOR(A,B),C);
    bool carry = Gates::OR(Gates::AND(A,B),Gates::AND(Gates::XOR(A,B),C));
    return {sum, carry};
```
The function returns a vector of boolean values, first the sum which is computed by XORing the three inputs, and then the carry which is computed using the equation mentioned in ***Truth Table & Boolean Expressions*** section.

## Example Usage
  The following code snippet provides an example
```C++
  vector<bool> result = combinational_circuits::Full_Adder(1,1,1);
  cout << result[0] << " " << result[1];
```
Expected Output:
```Terminal
  1 1
```

## Multisim Design
The simulation uses:

- Three 5V Vcc sources
- Three DIP switches for inputs
- Two XOR gates
- Two AND gates
- One OR gate
- LEDs for Sum and Carry outputs

The Logic Converter can be connected to:

- Inputs A, B, and Cin
- Either the Sum or Carry output

<img width="568" height="386" alt="image" src="https://github.com/user-attachments/assets/2bd9c970-70e4-46b7-857b-b4f7fcd5c9a5" />

*Logic converter with output connected to sum wire.*


>[!Note]
> Do not connect the Logic Converter pins directly to the Vcc before the DIP switch, connect them with a junction after the DIP switch.
