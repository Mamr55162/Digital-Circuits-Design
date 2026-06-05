#include <iostream>
#include "combinational_circuits.h"
#include <bitset>
#include <vector>

//Implement the Half Adder circuit.
vector<bool> combinational_circuits::Half_Adder(bool A, bool B)
{
    //Compute addition by XORing and carry by ANDing.
    bool sum = Gates::XOR(A, B);
    bool carry = Gates::AND(A, B);
    return {sum, carry};
}

//Implementation of Full Adder circuit.
vector<bool> combinational_circuits::Full_Adder(bool A, bool B, bool C)
{
    bool sum = Gates::XOR(Gates::XOR(A, B), C);
    bool carry = Gates::OR(Gates::AND(A, B), Gates::AND(Gates::XOR(A, B), C));
    return {sum, carry};
}

//Implementation of 74HC283 IC 4-Bit Full Adder/Subtractor.
vector<bool> combinational_circuits::_74HC283_(bitset<4> A, bitset<4> B, bool C_in, bool stat)
{
    //XOR inputs, if stat == 1 -> (1's Compliment).
    B[0] = Gates::XOR(B[0], stat);
    B[1] = Gates::XOR(B[1], stat);
    B[2] = Gates::XOR(B[2], stat);
    B[3] = Gates::XOR(B[3], stat);

    C_in = stat; //If stat == 1, the carry will add 1 to the LSBs (2's Compliment).
    vector<bool> result(5);
    vector<bool> sum = {0b0, C_in};
    for (int i = 0; i < 4; i++)
    {
        sum = Full_Adder(A[i], B[i], sum[1]);
        result[result.size() - i - 1] = sum[0];
    }
    if (stat == 0) //When working as a subtractor (stat == 1) ignore the MSB.
        result[0] = sum[1];
    return result;
}

//Implementation of Identity Comparator.
bool combinational_circuits::Identity_Comparator(bitset<4> A, bitset<4> B)
{
    bool result;
    result = Gates::AND(
        Gates::AND(
            Gates::XNOR(A[0], B[0]),
            Gates::XNOR(A[1], B[1])
        ),
        Gates::AND(
            Gates::XNOR(A[2], B[2]),
            Gates::XNOR(A[3], B[3])
        )
    );
    return result;
}

//Implementation of 74HC85 8-bit Magnitude Comparator.
vector<bool> combinational_circuits::_74HC85_(bitset<4> A, bitset<4> B, bool eq, bool larger, bool smaller)
{
    // --- Bit equality flags (XNOR per bit) ---
    bool x3 = Gates::XNOR(A[3], B[3]);
    bool x2 = Gates::XNOR(A[2], B[2]);
    bool x1 = Gates::XNOR(A[1], B[1]);
    bool x0 = Gates::XNOR(A[0], B[0]);

    // --- A[i] > B[i]: A[i]=1 AND B[i]=0 ---
    bool g3 = Gates::AND(A[3], Gates::NOT(B[3]));
    bool g2 = Gates::AND(A[2], Gates::NOT(B[2]));
    bool g1 = Gates::AND(A[1], Gates::NOT(B[1]));
    bool g0 = Gates::AND(A[0], Gates::NOT(B[0]));

    // --- A[i] < B[i]: B[i]=1 AND A[i]=0 ---
    bool l3 = Gates::AND(B[3], Gates::NOT(A[3]));
    bool l2 = Gates::AND(B[2], Gates::NOT(A[2]));
    bool l1 = Gates::AND(B[1], Gates::NOT(A[1]));
    bool l0 = Gates::AND(B[0], Gates::NOT(A[0]));

    // --- Cascaded equality prefix masks ---
    bool x32 = Gates::AND(x3, x2);
    bool x321 = Gates::AND(x32, x1);
    bool x3210 = Gates::AND(x321, x0);

    // --- EQ output ---
    // All bits equal AND cascade-eq input
    bool eq_out = Gates::AND(x3210, eq);

    // --- LARGER output (A > B) ---
    // Priority: bit3 wins, then bit2 if bit3 equal, etc.
    // larger = g3
    //        | (x3  & g2)
    //        | (x3  & x2  & g1)
    //        | (x3  & x2  & x1  & g0)
    //        | (x3  & x2  & x1  & x0 & larger_cascade)
    bool larger_out =
        Gates::OR(g3,
                  Gates::OR(Gates::AND(x3, g2),
                            Gates::OR(Gates::AND(x32, g1),
                                      Gates::OR(Gates::AND(x321, g0),
                                                Gates::AND(x3210, larger)))));

    // --- SMALLER output (A < B) ---
    // Mirror of larger with l instead of g
    bool smaller_out =
        Gates::OR(l3,
                  Gates::OR(Gates::AND(x3, l2),
                            Gates::OR(Gates::AND(x32, l1),
                                      Gates::OR(Gates::AND(x321, l0),
                                                Gates::AND(x3210, smaller)))));

    // Result order: { A>B, A==B, A<B }
    return {larger_out, eq_out, smaller_out};
}

//Implementation of 4-to-1 Multiplexer.
bool combinational_circuits::MUX_4_to_1(bitset<4> A, bool S0, bool S1)
{
    bool D0 = Gates::AND(A[0], Gates::AND(Gates::NOT(S0), Gates::NOT(S1)));
    bool D1 = Gates::AND(A[1], Gates::AND(Gates::NOT(S0), S1));
    bool D2 = Gates::AND(A[2], Gates::AND(S0, Gates::NOT(S1)));
    bool D3 = Gates::AND(A[3], Gates::AND(S0, S1));
    bool out = Gates::OR(D0, Gates::OR(D1, Gates::OR(D2, D3)));
    return out;
}

//Implementation of 74HC151 IC 8-to-1 MUX.
vector<bool> combinational_circuits::_74HC151_(bitset<8> A, bool S0, bool S1, bool S2)
{
    bool D0 = Gates::AND(Gates::AND(A[0], Gates::NOT(S0)), Gates::AND(Gates::NOT(S1), Gates::NOT(S0)));
    bool D1 = Gates::AND(Gates::AND(A[1], Gates::NOT(S0)), Gates::AND(Gates::NOT(S1), S2));
    bool D2 = Gates::AND(Gates::AND(A[2], Gates::NOT(S0)), Gates::AND(S1, Gates::NOT(S2)));
    bool D3 = Gates::AND(Gates::AND(A[3], Gates::NOT(S0)), Gates::AND(S1, S2));
    bool D4 = Gates::AND(Gates::AND(A[4], S0), Gates::AND(Gates::NOT(S1), Gates::NOT(S2)));
    bool D5 = Gates::AND(Gates::AND(A[5], S0), Gates::AND(Gates::NOT(S1), S2));
    bool D6 = Gates::AND(Gates::AND(A[6], S0), Gates::AND(S1, Gates::NOT(S2)));
    bool D7 = Gates::AND(Gates::AND(A[7], S0), Gates::AND(S1, S2));

    bool Y = Gates::OR(Gates::OR(Gates::OR(D0, D1), Gates::OR(D2, D3)),
                       Gates::OR(Gates::OR(D4, D5), Gates::OR(D6, D7)));

    return {Y, Gates::NOT(Y)};
}


//Implementation of a 2-to-4 Decoder
vector<bool> combinational_circuits::Decoder_2_to_4(bool A, bool B)
{
    bool A1 = Gates::AND(Gates::NOT(A), Gates::NOT(B));
    bool A2 = Gates::AND(Gates::NOT(A), B);
    bool A3 = Gates::AND(A, Gates::NOT(B));
    bool A4 = Gates::AND(A, B);
    return {A1, A2, A3, A4};
}


//Implementation of 74HC154 IC 4-to-16 Decoder.
vector<bool> combinational_circuits::_74HC154_(bool A0, bool A1, bool A2, bool A3, bool EN)
{
    bool OUT0 = Gates::NOT(Gates::AND(
        Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), EN), Gates::NOT(A2)), Gates::NOT(A1)), Gates::NOT(A0)));
    bool OUT1 = Gates::NOT(
        Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), EN), Gates::NOT(A2)), Gates::NOT(A1)), A0));
    bool OUT2 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), EN), Gates::NOT(A2)), A1),
                                      Gates::NOT(A0)));
    bool OUT3 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), EN), Gates::NOT(A2)), A1), A0));

    bool OUT4 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), EN), A2), Gates::NOT(A1)),
                                      Gates::NOT(A0)));
    bool OUT5 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), EN), A2), Gates::NOT(A1)), A0));
    bool OUT6 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), EN), A2), A1), Gates::NOT(A0)));
    bool OUT7 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), EN), A2), A1), A0));

    bool OUT8 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(A3, EN), Gates::NOT(A2)), Gates::NOT(A1)),
                                      Gates::NOT(A0)));
    bool OUT9 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(A3, EN), Gates::NOT(A2)), Gates::NOT(A1)), A0));
    bool OUT10 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(A3, EN), Gates::NOT(A2)), A1), Gates::NOT(A0)));
    bool OUT11 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(A3, EN), Gates::NOT(A2)), A1), A0));

    bool OUT12 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(A3, EN), A2), Gates::NOT(A1)), Gates::NOT(A0)));
    bool OUT13 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(A3, EN), A2), Gates::NOT(A1)), A0));
    bool OUT14 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(A3, EN), A2), A1), Gates::NOT(A0)));
    bool OUT15 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::AND(A3, EN), A2), A1), A0));

    return {
        OUT0, OUT1, OUT2, OUT3,
        OUT4, OUT5, OUT6, OUT7,
        OUT8, OUT9, OUT10, OUT11,
        OUT12, OUT13, OUT14, OUT15
    };
}

//Implementation of using a 2-to-4 Decoder to construct a 4-to-1 MUX
bool combinational_circuits::Decoder_to_MUX(bool S1, bool S0, bool I0, bool I1, bool I2, bool I3)
{
    bool D0 = Gates::AND(Gates::NOT(S1), Gates::NOT(S0));
    bool D1 = Gates::AND(Gates::NOT(S1), S0);
    bool D2 = Gates::AND(S1, Gates::NOT(S0));
    bool D3 = Gates::AND(S1, S0);

    bool F = Gates::OR(
        Gates::OR(Gates::AND(D0, I0), Gates::AND(D1, I1)),
        Gates::OR(Gates::AND(D2, I2), Gates::AND(D3, I3))
    );

    return F;
}

//Implementation of 74HC42 IC BCD-to-Decimal Decoder
vector<bool> combinational_circuits::_74HC42_(bool A0, bool A1, bool A2, bool A3)
{
    bool OUT0 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), Gates::NOT(A1)),
                                      Gates::NOT(A0)));
    bool OUT1 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), Gates::NOT(A1)), A0));
    bool OUT2 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), A1), Gates::NOT(A0)));
    bool OUT3 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), A1), A0));

    bool OUT4 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), A2), Gates::NOT(A1)), Gates::NOT(A0)));
    bool OUT5 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), A2), Gates::NOT(A1)), A0));
    bool OUT6 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), A2), A1), Gates::NOT(A0)));
    bool OUT7 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), A2), A1), A0));

    bool OUT8 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(A3, Gates::NOT(A2)), Gates::NOT(A1)), Gates::NOT(A0)));
    bool OUT9 = Gates::NOT(Gates::AND(Gates::AND(Gates::AND(A3, Gates::NOT(A2)), Gates::NOT(A1)), A0));

    return {
        OUT0, OUT1, OUT2, OUT3, OUT4,
        OUT5, OUT6, OUT7, OUT8, OUT9
    };
}

//Implementation of a BCD-to-7-Segment Decoder with a LED Display 74LS47D.
vector<bool> combinational_circuits::BCD_to_7_Segment(bool A3, bool A2, bool A1, bool A0, bool decimal)
{
    // Digit detection (correct BCD decoding)
    bool D0 = Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)),
                         Gates::AND(Gates::NOT(A1), Gates::NOT(A0)));

    bool D1 = Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)),
                         Gates::AND(Gates::NOT(A1), A0));

    bool D2 = Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)),
                         Gates::AND(A1, Gates::NOT(A0)));

    bool D3 = Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)),
                         Gates::AND(A1, A0));

    bool D4 = Gates::AND(Gates::AND(Gates::NOT(A3), A2),
                         Gates::AND(Gates::NOT(A1), Gates::NOT(A0)));

    bool D5 = Gates::AND(Gates::AND(Gates::NOT(A3), A2),
                         Gates::AND(Gates::NOT(A1), A0));

    bool D6 = Gates::AND(Gates::AND(Gates::NOT(A3), A2),
                         Gates::AND(A1, Gates::NOT(A0)));

    bool D7 = Gates::AND(Gates::AND(Gates::NOT(A3), A2),
                         Gates::AND(A1, A0));

    bool D8 = Gates::AND(Gates::AND(A3, Gates::NOT(A2)),
                         Gates::AND(Gates::NOT(A1), Gates::NOT(A0)));

    bool D9 = Gates::AND(Gates::AND(A3, Gates::NOT(A2)),
                         Gates::AND(Gates::NOT(A1), A0));

    // Segment logic (standard active-high common cathode)

    bool a = Gates::OR(Gates::OR(Gates::OR(D0, D2), Gates::OR(D3, D5)),
                       Gates::OR(Gates::OR(D6, D7), Gates::OR(D8, D9)));

    bool b = Gates::OR(Gates::OR(Gates::OR(D0, D1), Gates::OR(D2, D3)),
                       Gates::OR(Gates::OR(D4, D7), Gates::OR(D8, D9)));

    bool c = Gates::OR(Gates::OR(Gates::OR(D0, D1), Gates::OR(D3, D4)),
                       Gates::OR(Gates::OR(D5, D6), Gates::OR(D7, D8)));

    bool d = Gates::OR(Gates::OR(Gates::OR(D0, D2), Gates::OR(D3, D5)),
                       Gates::OR(Gates::OR(D6, D8), D9));

    bool e = Gates::OR(Gates::OR(D0, D2), Gates::OR(D6, D8));

    bool f = Gates::OR(Gates::OR(Gates::OR(D0, D4), Gates::OR(D5, D6)),
                       Gates::OR(D8, D9));

    bool g = Gates::OR(Gates::OR(Gates::OR(D2, D3), Gates::OR(D4, D5)),
                       Gates::OR(D6, D8));

    bool dp = decimal;

    // TOP (a)
    if (a) cout << " --- " << endl;
    else cout << "     " << endl;

    // UPPER (f and b)
    if (f && b) cout << "|   |" << endl;
    else if (f && !b)cout << "|    " << endl;
    else if (!f && b)cout << "    |" << endl;
    else cout << "     " << endl;

    // MIDDLE (g)
    if (g) cout << " --- " << endl;
    else cout << "     " << endl;

    // LOWER (e and c)
    if (e && c) cout << "|   |" << endl;
    else if (e && !c)cout << "|    " << endl;
    else if (!e && c)cout << "    |" << endl;
    else cout << "     " << endl;

    // BOTTOM (d) and DECIMAL POINT (dp)
    if (d) cout << " --- ";
    else cout << "     ";

    if (dp) cout << " ." << endl;
    else cout << "  " << endl;

    return {a, b, c, d, e, f, g, dp};
}

vector<bool> combinational_circuits::Encoder_4_to_2(bool D0, bool D1, bool D2, bool D3)
{
    bool Q1 = Gates::OR(D2, D3);
    bool Q0 = Gates::OR(D1, D3);

    return {Q1, Q0};
}

vector<bool> combinational_circuits::Decimal_to_BCD_Encoder(
    bool D0, bool D1, bool D2, bool D3, bool D4,
    bool D5, bool D6, bool D7, bool D8, bool D9)
{
    bool A0 = Gates::OR(
                Gates::OR(Gates::OR(D1, D3), Gates::OR(D5, D7)),
                D9);

    bool A1 = Gates::OR(
                Gates::OR(D2, D3),
                Gates::OR(D6, D7));

    bool A2 = Gates::OR(
                Gates::OR(D4, D5),
                Gates::OR(D6, D7));

    bool A3 = Gates::OR(D8, D9);

    return {A3, A2, A1, A0};
}

//Implementation of 74HC147 IC, a Decimal-to-BCD priority Encoder with active low inputs and outputs.
vector<bool> combinational_circuits::_74HC147_(
    bool D0, bool D1, bool D2, bool D3, bool D4,
    bool D5, bool D6, bool D7, bool D8, bool D9)
{
    // Winner lines (active HIGH internally)

    bool W9 = Gates::NOT(D9);

    bool W8 = Gates::AND(D9,
               Gates::NOT(D8));

    bool W7 = Gates::AND(
               Gates::AND(D9, D8),
               Gates::NOT(D7));

    bool W6 = Gates::AND(
               Gates::AND(Gates::AND(D9, D8), D7),
               Gates::NOT(D6));

    bool W5 = Gates::AND(
               Gates::AND(Gates::AND(Gates::AND(D9, D8), D7), D6),
               Gates::NOT(D5));

    bool W4 = Gates::AND(
               Gates::AND(Gates::AND(Gates::AND(Gates::AND(D9, D8), D7), D6), D5),
               Gates::NOT(D4));

    bool W3 = Gates::AND(
               Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(D9, D8), D7), D6), D5), D4),
               Gates::NOT(D3));

    bool W2 = Gates::AND(
               Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(D9, D8), D7), D6), D5), D4), D3),
               Gates::NOT(D2));

    bool W1 = Gates::AND(
               Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(D9, D8), D7), D6), D5), D4), D3), D2),
               Gates::NOT(D1));

    bool W0 = Gates::AND(
               Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(D9, D8), D7), D6), D5), D4), D3), D2), D1),
               Gates::NOT(D0));

    // BCD outputs (before inversion)
    bool B0 = Gates::OR(
                Gates::OR(W1, W3),
                Gates::OR(
                    Gates::OR(W5, W7),
                    W9));

    bool B1 = Gates::OR(
                Gates::OR(W2, W3),
                Gates::OR(W6, W7));

    bool B2 = Gates::OR(
                Gates::OR(W4, W5),
                Gates::OR(W6, W7));

    bool B3 = Gates::OR(W8, W9);

    // Active LOW outputs
    bool A0 = Gates::NOT(B0);
    bool A1 = Gates::NOT(B1);
    bool A2 = Gates::NOT(B2);
    bool A3 = Gates::NOT(B3);

    return {A3, A2, A1, A0};
}

int main()
{
    
    return 0;
}