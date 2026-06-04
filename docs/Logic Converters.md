# Logic Converters Classes
## Overview
These are 3 classes of Logic Converters that produce multiple options the user can choose from, starting from two inputs up to 4 inputs, I will also provide a Multisim guide on how to use Logic Converters.
**Logic Converters can:**
- Design truth table from given outputs.
- Derive SOP & POS expressions.
- Calculate number of gates needed to construct an expression, assuming two-input gates only.
- Construct a circuit only from NAND gates.

Here, I will provide code walkthroughs and Multisim guide for each function.
## Two-input Logic Converter
The logic converters implemented in our code does not mimic Multisim components' inner algorithms exactly, the functionality of both however, is the same.
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
      if (out1)
         exp += "A'B'";
      if (out2)
      {
         if (out1) // if there is a previous term add '+' between them.
            exp += " + A'B";
         else
            exp += "A'B";
      }
      if (out3)
      {
         if (out1 || out2)
            exp += " + AB'";
         else
            exp += "AB'";
      }
      if (out4)
      {
         if (out1 || out2 || out3)
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

* **POS Expression**:
In this function, we generate POS expression string from the given outputs, see the following code snippet:
```cpp

   static string POS_Expression(bool out1,bool out2,bool out3,bool out4)
   {
      string exp;
      if (!out1)
         exp += "(A+B)";
      if (!out2)
         exp += "(A+B')";
      if (!out3)
         exp += "(A'+B)";
      if (!out4)
         exp += "(A'+B')";
      return exp.empty() ? "1" : exp;
   }
```
The if conditions check the validity of the outputs, if it is zero, the if condition checks the inverse of it so it can work, every time an output is zero we add its corresponding POS (product-of-sums) term from the truth table. the function returns a string and checks if the string is empty it means that the table is filled with ones in the output row, so the POS expression value would be just **1**, if not then return the expression.
### Example Usage
The following code snippet provides an example:
```cpp
    cout << "\n" << twoLogicConverter::POS_Expression(0,1,1,0);
```
Expected Output:
```text
(A+B)(A'+B')
```
<img width="1417" height="385" alt="2 POS" src="https://github.com/user-attachments/assets/a721a452-0142-4fd6-a1a1-a0de787c1d76" />

***POS Expression from truth table***

* **SOP & POS Gate Construction, and the NAND construction method**:
In these three functions, we try to compute the number of gates needed to construct both an SOP or POS expression, see the following code snippet:
```cpp

   //Derive the POS Expression from truth table.
   static string POS_Expression(bool out1, bool out2, bool out3, bool out4)
   {
      string exp;
      if (!out1)
         exp += "(A+B)";
      if (!out2)
         exp += "(A+B')";
      if (!out3)
         exp += "(A'+B)";
      if (!out4)
         exp += "(A'+B')";
      return exp.empty() ? "1" : exp;
   }

   //Compute and show the number of gates needed to construct an SOP expression.
   static void SOP_GateConstruction(bool out1, bool out2, bool out3, bool out4)
   {
      int NOT_count = 0, AND_count = 0, OR_count = 0;
      if (out2)
      {
         AND_count++;
         NOT_count++;
      }
      if (out3)
      {
         AND_count++;
         NOT_count++;
      }
      if (out1)
      {
         AND_count++;
         NOT_count = 2;
      }
      if (out4)
         AND_count++;
      if (AND_count > 1)
         OR_count = AND_count - 1;
      cout << "Construction : " << NOT_count << " NOT Gates, " << AND_count << " AND Gates, " << OR_count <<
         " OR Gates\n";
   }

   //Compute and show the number of gates needed to construct an POS expression.
   static void POS_GateConstruction(bool out1, bool out2, bool out3, bool out4)
   {
      int NOT_count = 0, AND_count = 0, OR_count = 0;
      if (!out1)
         OR_count++;
      if (!out2)
      {
         OR_count++;
         NOT_count++;
      }
      if (!out3)
      {
         OR_count++;
         NOT_count++;
      }
      if (!out4)
      {
         OR_count++;
         NOT_count = 2;
      }
      if (OR_count > 1)
         AND_count = OR_count - 1;
      cout << "Construction : " << NOT_count << " NOT Gates, " << AND_count << " AND Gates, " << OR_count <<
         " OR Gates\n";
   }
   //Compute and show the number of NAND gates needed to construct an expression.
   static void NANDConstruction(bool out1,bool out2,bool out3,bool out4)
   {
      int terms = 0;

      if(out1) terms++;
      if(out2) terms++;
      if(out3) terms++;
      if(out4) terms++;

      // A' and B'
      int nand_NOT = 2;

      // Each minterm generated by NAND
      int nand_terms = terms;

      // Final NAND combination
      int nand_final = (terms > 1) ? 1 : 0;

      int total = nand_NOT + nand_terms + nand_final;

      cout << "NAND Construction : "
           << total << " NAND Gates\n";
   }

```
In the first function we initialize all three variables with zeros, then we start our if conditions and each term we meet we add one to the count of the AND gates, if we encounter the first term ( 0 0 ), we need two NOT gates to get A'B', and to count the OR gates we use a general formula that applies to all inputs which states: **$OR_{count} = AND_{count} - 1$**, and finally we output the counts of each gate, the POS function is exactly the same as the SOP but we need two NOT gates in the case of the last combination ( 1 1 ), and the mentioned general equation would be reversed: **$AND_{count} = OR_{count} - 1$**.
As for the NAND construction method it is commonly used in real logic converters, because NAND is well-known as the universal gate, in our function we have 4 variables:
- nand_NOT, which indicates the number of NOT gates used for inputs.
- nand_terms, which is the number of 2-input NAND gates required to perform the AND operation on the minterms. Each term represents a product of variables.
- nand_final, An additional NAND gate to combine all the minterm outputs into one final ORed result using ternary if to determine its value.
- total, The total number of NAND gates needed.
Then finally the output line.

### Example Usage
The following code snippet provides an example:
```cpp
twoLogicConverter::SOP_GateConstruction(0,1,1,0);
 twoLogicConverter::POS_GateConstruction(0,1,1,0);
 twoLogicConverter::NANDConstruction(0,1,1,0);
```
Expected Output:
```text
Construction : 2 NOT Gates, 2 AND Gates, 1 OR Gates
Construction : 2 NOT Gates, 1 AND Gates, 2 OR Gates
NAND Construction : 5 NAND Gates
```

<img width="863" height="370" alt="image" src="https://github.com/user-attachments/assets/5388f0fa-1fa8-48d4-91de-61b82afbdb3c" />

***SOP Construction of A'B + AB' in Multisim***

<img width="854" height="365" alt="image" src="https://github.com/user-attachments/assets/28e0f164-9127-4c82-8d23-618abc547083" />

***POS Construction of (A + B)(A' + B') in Multisim***

<img width="533" height="260" alt="image" src="https://github.com/user-attachments/assets/fe0fb189-e5f7-4c56-96fe-f0bf5173e318" />

***NAND Construction of A'B + AB' in Multisim using HEADERS_TEST from Connections group***

## Three-input Logic Converter

In the three-input logic converter class, nearly every function we talked about in the two-input logic converter class is the same, here I will discuss the differences and tweaks done on the functions, without going into too much details if not needed, see the provided codes in **src/** directory.
* **SOP & POS Expressions**:
The difference here is the optimization made, instead of making a code full of if statements that may reach up to 9 outer statements, we saved all the possible 3-input SOP ( the same applies for the POS function ) terms in a string array, and the outputs in a bool array and we loop through the outputs array, if an output is true we add the corresponding term to the expression, and we check if the expression is not empty we should add a '+' sign between terms, see the following code snippet for the SOP function:
```cpp
static string SOP_Expression(bool o1, bool o2, bool o3, bool o4,
                          bool o5, bool o6, bool o7, bool o8)
   {
      string exp;
      const string terms[8] = {
         "A'B'C'", "A'B'C", "A'BC'", "A'BC",
         "AB'C'",  "AB'C",  "ABC'",  "ABC"
      };
      const bool output[8] = {o1,o2,o3,o4,o5,o6,o7,o8};
      for (int i = 0; i < 8; i++)
      {
         if (output[i])
         {
            if (!exp.empty()) exp += " + ";
            exp += terms[i];
         }
      }
      return exp.empty() ? "0" : exp;
   }
```
* **SOP & POS Gate Construction**:
The difference here is that we use another approach, here we loop through the array of outputs and each output we add one to the AND_count if SOP, OR_count if POS, and we declare three boolean variables to detect whether we need a NOT gate for A, B or C, if the expression is anywhere less than 4 so A needs a NOT gate, if it is in ( 0,1,4,5 ) then B needs a NOT gate, if it is in ( 0,2,4,6 ) then C needs a NOT gate, to understand this mechanism refer to a three-input truth table, this logic applies for SOP, the same applies with POS with differences in the NOT gates conditions, after that we count how many NOT_count we need and apply the same formula we used before to get the OR_count from AND_count if using SOP, or the AND_count from OR_count if using POS.
