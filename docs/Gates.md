# Gates class
## Overview
This class focuses on implementing every logic gate using a universal logic gate NAND.

<img width="340" height="188" alt="image" src="https://github.com/user-attachments/assets/02bb158c-6ffc-4a64-97b5-2b286b41ffe7" />


***NAND Gate***

***NAND Truth Table:***
| A | B | Y |
| - | - | - |
| 0 | 0 | 1 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 0 |

Y = (AB)' = A' + B'

## Code Walkthrough 
Implementation of NAND gate is used in the same class:
```cpp
static bool NAND(bool A, bool B)
   {
      return !(A & B);
   }
```
It uses the bitwise AND **&** with a NOT operator **!** at the parentheses to inverse the AND operation.
## Gates class:
* **NOT Gate**: implemented using a NAND with its two inputs tied together, see the following code snippet:
```cpp
static bool NOT(bool A)
   {
      return NAND(A, A);
   }
```
Here I use one NAND with the same input (tied two-input) to produce the complement of the input, refer to the NAND truth table.

<img width="183" height="43" alt="image" src="https://github.com/user-attachments/assets/9f2f22c9-5ca4-4b7c-a0ec-abeabbc5335c" />

* **OR Gate**: implemented using three NAND gates, two for inverting inputs working as NOT gates, and a usual one, so when you apply De Morgan's law the product becomes sum and the complements cancel, see the following code snippet:
```cpp
static bool OR(bool A, bool B)
   {
      return NAND(NAND(A,A), NAND(B, B));
   }
```
Here the inner NAND gates work as the inverters, while the outer NAND works as a usual NAND.

<img width="248" height="77" alt="image" src="https://github.com/user-attachments/assets/2e74a040-525d-44e0-8ea3-cba50a5484dc" />

* **AND Gate**: implemented using two NAND gates, one usual NAND, and one for inverting the output, so when applying De Morgan's law, it gets canceled by the last inverter gate at the output producing a normal product term, in the code we use 3 NAND calls, see the following code snippet:
```cpp
static bool AND(bool A, bool B)
   {
      return NAND(NAND(A, B), NAND(A, B));
   }
```
Here the inner NAND gate compute a normal NAND, the outer NAND works as an inverter because it takes the same input twice, so logically we did one NAND operation then we inverted it to become an AND.


<img width="236" height="51" alt="image" src="https://github.com/user-attachments/assets/90a606f7-a316-487d-b6fb-abfa91f51996" />


* **NOR Gate**: implemented using four NAND gates, two for inverting the inputs, one usual NAND, and another inverter at the output, first two inverters complement the input then a usual NAND gate produces a normal OR operation, the last inverter puts the complement on the output to become NOR, see the following code snippet:
```cpp
static bool NOR(bool A, bool B)
   {
      return NAND(NAND(NAND(A, A), NAND(B, B)),
                           NAND(NAND(A, A), NAND(B, B)));
   }
```
Here the we use 6 NAND calls, the innermost 2 invert the inputs, the outer 2 perform NAND operation and the outermost 2 invert the output, a more logical code would be like the following:
```cpp
static bool NOR(bool A, bool B)
   {
      A = NAND(A,A);
      B = NAND(B,B);
      bool out_invert = NAND(A,B);
      return NAND(out_invert, out_invert);
   }
```
It takes more lines but it shows the exact logical step-by-step procedure and each gate in the design independently.

<img width="293" height="81" alt="image" src="https://github.com/user-attachments/assets/18e072c9-d491-497e-a569-d033014e34f9" />


* **XOR Gate**: implemented using four NAND gates, all of them perform natural NAND operations, none of them is an inverter, the first NAND performs a normal NAND operation, the second NAND gate takes a connection from A and the output of the first NAND, the third NAND also takes a connection from B and the output of the first NAND, the fourth and last NAND takes both the outputs of the second and third NANDs performing the XOR operation, see the following code snippet:
```cpp
static bool XOR(bool A, bool B)
   {
      bool x = NAND(A, B);
      bool y = NAND(A, x);
      bool z = NAND(B, x);
      return NAND(y, z);
   }
```
Here three accomplice variables are used which are: x, y and z, the first NAND applies to A and B, the second one takes the output of the first with A, the third one takes the output of the first with B, and finally the last NAND gate takes both the second and third output.

<img width="534" height="156" alt="image" src="https://github.com/user-attachments/assets/f6f20982-c2ea-4da1-9fc4-b19572661e30" />

* **XNOR Gate**: implemented using five NAND gates, all the same logic as XOR except the last gate which will be an inverter for the output, see the following code snippet:
```cpp
static bool XNOR(bool A, bool B)
   {
      bool x = NAND(A, B);
      bool y = NAND(A, x);
      bool z = NAND(B, x);
      bool out_invert = NAND(y, z);
      return NAND(out_invert, out_invert);
   }
```
Here the same XOR logic is used except for the last gate which works as an inverter to the output which makes the XNOR operation.

<img width="539" height="170" alt="image" src="https://github.com/user-attachments/assets/eecd7324-5095-4445-a175-2038fb00cc5e" />

* **Tri-state Gate**: implemented using enum which describes the three states of the circuit, either LOW, HIGH, or High Impedance, its truth table looks like this:

| EN | A | Out |
| -- | - | --- |
| 0  | X | Hi-Z|
| 1  | 0 | 0   |
| 1  | 1 | 1   |

So if the Enable input is LOW, the output will not care about A, that's what X means, a don't care term, exactly an open circuit in a design, High impedance and no signal passes, see the following code snippet:
```cpp
enum TriState
    {
        LOW = 0,
        HIGH = 1,
        HIGH_Z
    };

    static TriState Tri_state(bool A, bool EN)
    {
        if (!EN)
            return HIGH_Z;

        return A ? HIGH : LOW;
    }
```
Here if the Enable is low it outputs HIGH_Z, a state which no boolean value can express, so we used enum called TriState for this third state especially.

<img width="250" height="179" alt="image" src="https://github.com/user-attachments/assets/04058a33-19a4-4965-a9d8-2b812b7e9fde" />
