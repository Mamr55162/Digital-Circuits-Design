# Half Adder
## Overview
Half Adders circuits are an example of combinational logic circuits, it accepts two binary digits and outputs two binary digits the sum & carry, it is implemented using XOR gate for adding the two bits, & AND gate for computing the carry.
## Truth Table & Boolean Expressions
| A | B | Sum | Carry |
| - | - | --- | ----- |
| 0 | 0 |  0  |   0   |
| 0 | 1 |  1  |   0   |
| 1 | 0 |  1  |   0   |
| 1 | 1 |  0  |   1   |

Sum = A $\oplus$ B

Carry = AB

## Circuit Structure
![Circuit Design](https://www.build-electronic-circuits.com/wp-content/uploads/2022/10/halfadder2-2.png)

## Code Walkthrough
The implementation of Half Adder in the code is a function with a return value of a vector of boolean values, it accepts two boolean inputs A & B, which are the two binary digits we will be adding, inside the function we declare the sum and carry variables each as a boolean data type, then we assign them values by accessing the class **Gates** and calling the gates methods, after the computation is done the functions returns both sum and carry as a vector of boolean values. All the gates used in **Gates** class are internally designed using only NAND gate.
```C++
bool sum = Gates::XOR(A,B);
bool carry = Gates::AND(A, B);
return {sum, carry};
```

## Example Usage
Here is the code snippet
```C++
vector<bool> result = combinational_circuits::Half_Adder(1,1);
cout << result[0] << " " << result[1];
```
Expected Output:
```Terminal
0 1
```
## Multisim Design
The simulation uses:

- Two Vcc 5.0V sources
- Two DIP switches for inputs
- Two LEDs grounded for Sum and Carry
- XOR gate
- AND gate

<img width="1194" height="559" alt="image" src="https://github.com/user-attachments/assets/21fbe8ea-cce4-4a43-bf0d-e82cba11b6f1" />

***Multisim design of Half Adder***


The Logic Converter can be connected to:

- Inputs A and B
- Either the Sum or Carry output

<img width="573" height="381" alt="logic_converter" src="https://github.com/user-attachments/assets/17d937ce-80ae-49e3-8c75-c87f5e1a36e1" />

*Logic converter with output connected to sum wire.*

  
> [!NOTE]
> - Do not connect the Logic Converter pins directly to the Vcc before the switch, connect them with a junction after the switch.
> - The DIP switches can be controlled by pressing the buttons labeled as "Key = " under the switch symbol.
