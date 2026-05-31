# Gates class
## Overview
This class focuses on implementing every logic gate using a universal logic gate NAND.

<img width="340" height="188" alt="image" src="https://github.com/user-attachments/assets/02bb158c-6ffc-4a64-97b5-2b286b41ffe7" />


***NAND Gate***

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
* NOT Gate: implemented using a NAND with its two inputs tied together, see the following code snippet:
```cpp
static bool NOT(bool A)
   {
      return NAND(A, A);
   }
```
Here I use one NAND with the same input (tied two-input) to produce the complement of the input, refer to the NAND truth table.

<img width="183" height="43" alt="image" src="https://github.com/user-attachments/assets/9f2f22c9-5ca4-4b7c-a0ec-abeabbc5335c" />

* OR Gate: implemented using three NAND gates, two for inverting inputs working as NOT gates, and a usual one, so when you apply De Morgan's law the product becomes sum and the complements cancel, see the following code snippet:
```cpp
static bool OR(bool A, bool B)
   {
      return NAND(NAND(A,A), NAND(B, B));
   }
```
Here the inner NAND gates work as the inverters, while the outer NAND works as a usual NAND.

<img width="248" height="77" alt="image" src="https://github.com/user-attachments/assets/2e74a040-525d-44e0-8ea3-cba50a5484dc" />

* AND Gate: implemented using four NAND gates, two for inverting the inputs, one usual NAND, and one for inverting the output, so when applying De Morgan's law, it gets canceled by the last inverter gate at the output producing a normal product term, in the code we use 3 NAND, see the following code snippet:
```cpp
static bool AND(bool A, bool B)
   {
      return NAND(NAND(A, B), NAND(A, B));
   }
```
Here the inner NAND gates each compute a normal NAND, the outer NAND works as an inverter because it takes the same input twice, so logically we did one NAND operation then we inverted it to become an AND, another implementation more logical would be:
```cpp
static bool AND(bool A, bool B)
   {
      A = NAND(A,A);
      B = NAND(B,B);
      bool out_invert = NAND(A,B);
      return NAND(out_invert, out_invert);
   }
```
It takes more lines but it lays out the complete step-by-step logic.

<img width="236" height="51" alt="image" src="https://github.com/user-attachments/assets/90a606f7-a316-487d-b6fb-abfa91f51996" />

