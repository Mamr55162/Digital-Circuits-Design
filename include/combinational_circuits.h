#pragma once
#include <vector>
#include <bitset>
using namespace std;
#ifndef C___CODED_CIRCUITS_COMBINATIONAL_CIRCUITS_H
#define C___CODED_CIRCUITS_COMBINATIONAL_CIRCUITS_H


class Wire
{
public:
   bool value;
};

//Use LED as indicator for HIGH (1) and LOW (0) outputs.
class Indicator
{
public:
   static void LED(bool val)
   {
      if (val == true)
         cout << "LED is ON\n";
      else
         cout << "LED is OFF\n";
   }
};

//Universal gate NAND implementation.
class NAND
{
public:
  static bool compute(bool A, bool B)
   {
      return !(A & B);
   }
};

//Implement NOT, OR, AND, NOR, XOR, XNOR, Tri-state gates using NAND gate only.
class Gates
{
public:
   static bool NOT(bool A)
   {
      return NAND::compute(A, A);
   }
   static bool OR(bool A, bool B)
   {
      return NAND::compute(NAND::compute(A,A), NAND::compute(B, B));
   }
   static bool AND(bool A, bool B)
   {
      return NAND::compute(NAND::compute(A, B), NAND::compute(A, B));
   }
   static bool NOR(bool A, bool B)
   {
      return NAND::compute(NAND::compute(NAND::compute(A, A), NAND::compute(B, B)),
                           NAND::compute(NAND::compute(A, A), NAND::compute(B, B)));
   }
   static bool XOR(bool A, bool B)
   {
      return NAND::compute(NAND::compute(NAND::compute(A,A),B), NAND::compute(NAND::compute(B,B),A));
   }
   static bool XNOR(bool A, bool B)
   {
      return NAND::compute(NAND::compute(NAND::compute(NAND::compute(A, A), B), NAND::compute(NAND::compute(B, B), A)),
                           NAND::compute(NAND::compute(NAND::compute(A, A), B), NAND::compute(NAND::compute(B, B), A)));
   }
   static bool Tri_state(bool A, bool EN)
   {
      if (!EN)
      {
         cout << "High Impedance\n";
         return 0;
      }
      else
         return Gates::AND(A,EN);
   }
};


class combinational_circuits
{

public:
   static bool Half_Adder(bool A, bool B);
   static vector<bool> Full_Adder(bool A, bool B, bool C);
   static vector<bool> _74HC283_(bitset<4> A, bitset<4> B, bool C_in, bool stat);
   static bool Identity_Comparator(bitset<4> A, bitset<4> B);
   static vector<bool> _74HC85_(bitset<4> A, bitset<4> B, bool eq = 0b1, bool larger = 0b0, bool smaller = 0b0);
   static bool MUX_4_to_1(bitset<4> A, bool S0, bool S1);
   static vector<bool> _74HC151_(bitset<8> A, bool S0, bool S1, bool S2);
   static vector<bool> Decoder_2_to_4(bool A, bool B);
   static vector<bool> _74HC154_(bool A0, bool A1, bool A2, bool A3, bool EN);
   static bool Decoder_to_MUX(bool S1, bool S0, bool I0, bool I1, bool I2, bool I3);
   static vector<bool> _74HC42_(bool A0, bool A1, bool A2, bool A3);
   static vector<bool> BCD_to_7_Segment(bool A0, bool A1, bool A2, bool A3, bool decimal);
};


#endif //C___CODED_CIRCUITS_COMBINATIONAL_CIRCUITS_H
