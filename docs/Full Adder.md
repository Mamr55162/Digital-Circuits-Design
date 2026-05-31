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
