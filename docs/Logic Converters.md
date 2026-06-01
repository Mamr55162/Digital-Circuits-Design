# Logic Converters classes
## Overview
These are 3 classes of Logic Converters that produce multiple options the user can choose from, starting from two inputs up to 4 inputs, I will also provide a Multisim guide on how to use Logic Converters.
**Logic Converters can:**
- Design truth table from given outputs.
- Derive SOP & POS expressions.
- Calculate number of gates needed to construct an expression, assuming two-input gates only.
- Construct a circuit only from NAND gates.
## Two-input Logic Converter
The logic converters implemented in our code are not that similar to multisim ones, but I will provide a full code and multisim guide, the functionality of both however, is the same.
### Code Walkthrough
* **Truth Table Generator**:
A two-input logic converter is implemented using a class of public methods, we will analyze each method in code order, see the following code snippet for the first function, which generates a truth table depending on the given outputs:
```cpp
static void TruthTable(bool out1,bool out2,bool out3,bool out4)
   {
      cout << "A  B | F\n";
      cout << "0  0 | " << out1 << "\n";
      cout << "0  1 | " << out2 << "\n";
      cout << "1  0 | " << out3 << "\n";
      cout << "1  1 | " << out4 << "\n";
   }
```
Here the code is quite simple and brute-forced, it doesn't need any tweaks or complications, given the outputs, which the user must provide, the truth table is generated.
### Example Usage
The following code snippet provides an example.
```cpp
twoLogicConverter::TruthTable(0,1,1,0);
```
Expected Output:
```text
A  B | F
0  0 | 0
0  1 | 1
1  0 | 1
1  1 | 0
```

<img width="1418" height="385" alt="image" src="https://github.com/user-attachments/assets/0afb3679-40c5-4102-92d6-f8ddfdf3cb61" />

***Truth Table generated***


* **SOP Expression**:
In this function we generate a string of SOP (sum-of-products) expression from the given outputs, see the following code snippet:
```cpp

   static string SOP_Expression(bool out1,bool out2,bool out3,bool out4)
   {
      string exp;
      if (out1 == true)
         exp += "A'B'";
      if (out2 == true)
      {
         if (out1 == true) // if there is a previous term add '+' between them.
            exp += " + A'B";
         else
            exp += "A'B";
      }
      if (out3 == true)
      {
         if (out1 == true || out2 == true)
            exp += " + AB'";
         else
            exp += "AB'";
      }
      if (out4 == true)
      {
         if (out1 == true || out2 == true || out3 == true)
            exp += " + AB";
         else
            exp += "AB";
      }
      return exp.empty() ? "0" : exp;
   }
```
In this code, we evaluate the truth values of each output, if it is 1, we append its corresponding product term from the truth table, in the following if conditions we have to check if there exists a previous term to append a plus sign '+' between it and the new added term, the function returns the string in the return statement if there exists a product term, but if the truth table is filled with zero in all rows in the output, the SOP expression becomes **0**.
### Example Usage
The following code snippet provides an example:
```cpp
    cout << "\n" << twoLogicConverter::SOP_Expression(0,1,1,0);
```
Expected Output:
```text
A'B + AB'
```

<img width="1117" height="304" alt="2 SOP" src="https://github.com/user-attachments/assets/49dd0a07-2804-4d05-b68c-18a083099577" />

***SOP Expression from truth table***
