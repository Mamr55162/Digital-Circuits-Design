# Half Adders
## Functionality
  Half Adders circuits are an example of combinational logic circuits, it accepts two binary digits and outputs two binary digits the sum & carry, it is implemented using XOR gate
for adding the two bits, & AND gate for computing the carry.
## Truth Table & Boolean Expressions
| A | B | Sum | Carry |
| - | - | --- | ----- |
| 0 | 0 |  0  |   0   |
| 0 | 1 |  1  |   0   |
| 1 | 0 |  1  |   0   |
| 1 | 1 |  0  |   1   |

Sum = A $\oplus$ B

Carry = A $\cdot$ B

## Circuit Structure
![Circuit Design](https://www.build-electronic-circuits.com/wp-content/uploads/2022/10/halfadder2-2.png)

## Code Walkthrough
  The implementation of Half Adder in the code is a function with a return value of a vector of boolean values, it accepts two boolean inputs A & B, which are the two binary digits we will be adding, inside the function we declare the sum and carry variables each as a boolean data type, then we assign them values by accessing the class **Gates** and calling the gates methods, after the computation is done the functions returns both sum and carry as a vector of boolean values.
```
C++
    bool sum = Gates::XOR(A,B);
    bool carry = Gates::AND(A, B);
    return {sum, carry};
```

## Example Usage
The following is a code snippet of an example
```
C++
  vector<bool> result = combinational_circuits::Half_Adder(1,1);
  cout << result[0] << " " << result[1];
```
Expected Output:
```
Terminal
  0 1
```
## Multisim Design
  In the design file, I used two Vcc voltage sources each with 5.0V with DIP switches to control the signal, switch on is HIGH and switch off is LOW, each can be switched on and off using the keyboard either button A or button B, each of the voltages source then connected to the XOR & AND gates to produce both sum and carry, for indicating the output there is two LEDs each connected with on of the two outputs & to a ground, to use the Logic Converter connect the first pin from the left to the first Vcc and the pin next to it to the second Vcc, connect the last pin on the right to the output then double click the Logic Converter and choose an option.

![Logic Converter](logic_converter.png)
  
> [!NOTE]
> Do not connect the Logic Converter pins directly to the Vcc before the switch, connect them with a junction after the switch.
