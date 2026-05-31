# Full Adder
## Functionality
  Full Adder circuit, unlike half adder, accepts three binary digits and produces two binary digits, sum and carry, what makes Full Adder more useful than half adders is that it accepts a carry as an input, so it can be cascaded to add more binary digits like 4 or 8 binary digits.
## Truth Table & Boolean Expressions

| A | B | C | Sum | Carry |
| - | - | - | --- | ----- |
| 0 | 0 | 0 |  0  |   0   |
| 0 | 0 | 1 |  1  |   0   |
| 0 | 1 | 0 |  1  |   0   |
| 0 | 1 | 1 |  0  |   1   |
| 1 | 0 | 0 |  1  |   0   |
| 1 | 0 | 1 |  0  |   1   |
| 1 | 1 | 0 |  0  |   1   |
| 1 | 1 | 1 |  1  |   1   |

Sum = A $\oplus$ B $\oplus$ C

Carry = ( A $\cdot$ B ) + ( A $\oplus$ B) $\cdot$ C

## Circuit Structure
![Full Adder](https://www.build-electronic-circuits.com/wp-content/uploads/2022/10/fullAdder-1.png)

## Code Walkthrough
  The code in full adder is nearly the same as half adder except for the Carry input, here is the code snippet
```
C++
    bool sum = Gates::XOR(Gates::XOR(A,B),C);
    bool carry = Gates::OR(Gates::AND(A,B),Gates::AND(Gates::XOR(A,B),C));
    return {sum, carry};
```
The function returns a vector of boolean values, first the sum which is computed by XORing the three inputs, and then the carry which is computed using the equation mentioned in ***Truth Table & Boolean Expressions*** section.

## Example Usage
  The following code snippet provides an example
```
C++
  vector<bool> result = combinational_circuits::Full_Adder(1,1,1);
  cout << result[0] << " " << result[1];
```
Expected Output:
```
Terminal
  1 1
```

## Multisim Design
For the simulation in multisim, 3 Vcc 5.0V sources were used, each for an input, each connected to a DIP switch that can be controlled, two XOR gates were used and two AND gates with one OR gate, the output of the sum and the carry are attached to a LED as an indicator and the LED to the ground, to connect the Logic converter, connect the first three pins from the left with A, B and C respectively, then connect the output to either the sum or the carry, double click the logic converter then choose an option.

[!Logic Converter with output connected to sum]<img width="568" height="386" alt="image" src="https://github.com/user-attachments/assets/2bd9c970-70e4-46b7-857b-b4f7fcd5c9a5" />


>[!Note]
> Do not connect the Logic Converter pins directly to the Vcc before the DIP switch, connect them with a junction after the DIP switch.
