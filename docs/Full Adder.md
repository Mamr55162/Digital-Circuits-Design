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
