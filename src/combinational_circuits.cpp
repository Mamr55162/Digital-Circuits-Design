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

//Implementation of Half Subtractor circuit.
vector<bool> combinational_circuits::Half_Subtractor(bool A, bool B)
{
    bool Diff = Gates::XOR(A,B);
    bool B_out = Gates::AND(Gates::NOT(A),B);
    return {Diff, B_out};
}

//Implementation of Full Subtractor circuit.
vector<bool> combinational_circuits::Full_Subtractor(bool A, bool B, bool B_in)
{
    bool Diff = Gates::XOR(Gates::XOR(A,B),B_in);
    bool B_out = Gates::OR(Gates::AND(Gates::NOT(A),B),Gates::OR(Gates::AND(Gates::NOT(A),B_in),Gates::AND(B,B_in)));
    return {Diff, B_out};
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

//Implementation of 2x2 Multiplier
vector<bool> combinational_circuits::Multiplier_2x2(bitset<2> A, bitset<2> B)
{
    //Partial Products
    bool P0 = Gates::AND(A[0],B[0]);
    bool P1 = Gates::AND(A[1],B[0]);
    bool P2 = Gates::AND(A[0],B[1]);
    bool P3 = Gates::AND(A[1],B[1]);

    //The Addition Circuit
    bool C0 = P0;
    bool C1 = Gates::XOR(P1,P2);
    bool C2 = Gates::XOR(P3,Gates::AND(P1,P2));
    bool C3 = Gates::AND(P3, Gates::AND(P1, P2));
    return {C0, C1, C2, C3};
}

// Implementation of 4x4 Multiplier
vector<bool> combinational_circuits::Multiplier_4x4(bitset<4> A, bitset<4> B)
{
    // ==========================================
    // 1. GENERATE ALL 16 PARTIAL PRODUCTS
    // ==========================================

    // Row 0
    bool r0_0 = Gates::AND(A[0], B[0]);
    bool r0_1 = Gates::AND(A[1], B[0]);
    bool r0_2 = Gates::AND(A[2], B[0]);
    bool r0_3 = Gates::AND(A[3], B[0]);

    // Row 1
    bool r1_0 = Gates::AND(A[0], B[1]);
    bool r1_1 = Gates::AND(A[1], B[1]);
    bool r1_2 = Gates::AND(A[2], B[1]);
    bool r1_3 = Gates::AND(A[3], B[1]);

    // Row 2
    bool r2_0 = Gates::AND(A[0], B[2]);
    bool r2_1 = Gates::AND(A[1], B[2]);
    bool r2_2 = Gates::AND(A[2], B[2]);
    bool r2_3 = Gates::AND(A[3], B[2]);

    // Row 3
    bool r3_0 = Gates::AND(A[0], B[3]);
    bool r3_1 = Gates::AND(A[1], B[3]);
    bool r3_2 = Gates::AND(A[2], B[3]);
    bool r3_3 = Gates::AND(A[3], B[3]);

    // ==========================================
    // 2. ADDITION STAGE 1
    // ==========================================

    bool M0 = r0_0;

    // Half Adder
    bool M1   = Gates::XOR(r0_1, r1_0);
    bool c1_1 = Gates::AND(r0_1, r1_0);

    // Full Adder
    bool s1_1 = Gates::XOR(Gates::XOR(r0_2, r1_1), c1_1);
    bool c1_2 = Gates::OR(
        Gates::AND(r0_2, r1_1),
        Gates::AND(Gates::XOR(r0_2, r1_1), c1_1));

    // Full Adder
    bool s1_2 = Gates::XOR(Gates::XOR(r0_3, r1_2), c1_2);
    bool c1_3 = Gates::OR(
        Gates::AND(r0_3, r1_2),
        Gates::AND(Gates::XOR(r0_3, r1_2), c1_2));

    // Half Adder
    bool s1_3 = Gates::XOR(r1_3, c1_3);
    bool c1_4 = Gates::AND(r1_3, c1_3);

    // ==========================================
    // 3. ADDITION STAGE 2
    // ==========================================

    // Half Adder
    bool M2   = Gates::XOR(s1_1, r2_0);
    bool c2_1 = Gates::AND(s1_1, r2_0);

    // Full Adder
    bool s2_1 = Gates::XOR(Gates::XOR(s1_2, r2_1), c2_1);
    bool c2_2 = Gates::OR(
        Gates::AND(s1_2, r2_1),
        Gates::AND(Gates::XOR(s1_2, r2_1), c2_1));

    // Full Adder
    bool s2_2 = Gates::XOR(Gates::XOR(s1_3, r2_2), c2_2);
    bool c2_3 = Gates::OR(
        Gates::AND(s1_3, r2_2),
        Gates::AND(Gates::XOR(s1_3, r2_2), c2_2));

    // Full Adder
    bool s2_3 = Gates::XOR(Gates::XOR(c1_4, r2_3), c2_3);
    bool c2_4 = Gates::OR(
        Gates::AND(c1_4, r2_3),
        Gates::AND(Gates::XOR(c1_4, r2_3), c2_3));

    // ==========================================
    // 4. ADDITION STAGE 3
    // ==========================================

    // Half Adder
    bool M3   = Gates::XOR(s2_1, r3_0);
    bool c3_1 = Gates::AND(s2_1, r3_0);

    // Full Adder
    bool M4 = Gates::XOR(Gates::XOR(s2_2, r3_1), c3_1);
    bool c3_2 = Gates::OR(
        Gates::AND(s2_2, r3_1),
        Gates::AND(Gates::XOR(s2_2, r3_1), c3_1));

    // Full Adder
    bool M5 = Gates::XOR(Gates::XOR(s2_3, r3_2), c3_2);
    bool c3_3 = Gates::OR(
        Gates::AND(s2_3, r3_2),
        Gates::AND(Gates::XOR(s2_3, r3_2), c3_2));

    // Full Adder
    bool M6 = Gates::XOR(Gates::XOR(c2_4, r3_3), c3_3);
    bool M7 = Gates::OR(
        Gates::AND(c2_4, r3_3),
        Gates::AND(Gates::XOR(c2_4, r3_3), c3_3));

    // ==========================================
    // 5. RETURN RESULT (LSB -> MSB)
    // ==========================================

    return {M0, M1, M2, M3, M4, M5, M6, M7};
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
        Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(D9, D8), D7), D6), D5), D4), D3),
                   D2),
        Gates::NOT(D1));

    bool W0 = Gates::AND(
        Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(Gates::AND(D9, D8), D7), D6), D5),
                                                    D4), D3), D2), D1),
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

//Implementation of Demultiplexer 1-to-4
vector<bool> combinational_circuits::DEMUX_1_to_4(bool D, bool S0, bool S1)
{
    bool Y0 = Gates::AND(Gates::AND(Gates::NOT(S1),Gates::NOT(S0)),D);
    bool Y1 = Gates::AND(Gates::AND(Gates::NOT(S1),S0),D);
    bool Y2 = Gates::AND(Gates::AND(S1,Gates::NOT(S0)),D);
    bool Y3 = Gates::AND(Gates::AND(S1,S0),D);
    return {Y0, Y1, Y2, Y3};
}

//Implementation of 1-to-16 DEMUX using 4-to-16 Decoder
vector<bool> combinational_circuits::Decoder_to_DEMUX(bool D, bool S0, bool S1, bool S2, bool S3)
{
    bool Y0  = Gates::AND(D, Gates::AND(Gates::AND(Gates::NOT(S3), Gates::NOT(S2)), Gates::AND(Gates::NOT(S1), Gates::NOT(S0))));
    bool Y1  = Gates::AND(D, Gates::AND(Gates::AND(Gates::NOT(S3), Gates::NOT(S2)), Gates::AND(Gates::NOT(S1), S0)));
    bool Y2  = Gates::AND(D, Gates::AND(Gates::AND(Gates::NOT(S3), Gates::NOT(S2)), Gates::AND(S1, Gates::NOT(S0))));
    bool Y3  = Gates::AND(D, Gates::AND(Gates::AND(Gates::NOT(S3), Gates::NOT(S2)), Gates::AND(S1, S0)));

    bool Y4  = Gates::AND(D, Gates::AND(Gates::AND(Gates::NOT(S3), S2), Gates::AND(Gates::NOT(S1), Gates::NOT(S0))));
    bool Y5  = Gates::AND(D, Gates::AND(Gates::AND(Gates::NOT(S3), S2), Gates::AND(Gates::NOT(S1), S0)));
    bool Y6  = Gates::AND(D, Gates::AND(Gates::AND(Gates::NOT(S3), S2), Gates::AND(S1, Gates::NOT(S0))));
    bool Y7  = Gates::AND(D, Gates::AND(Gates::AND(Gates::NOT(S3), S2), Gates::AND(S1, S0)));

    bool Y8  = Gates::AND(D, Gates::AND(Gates::AND(S3, Gates::NOT(S2)), Gates::AND(Gates::NOT(S1), Gates::NOT(S0))));
    bool Y9  = Gates::AND(D, Gates::AND(Gates::AND(S3, Gates::NOT(S2)), Gates::AND(Gates::NOT(S1), S0)));
    bool Y10 = Gates::AND(D, Gates::AND(Gates::AND(S3, Gates::NOT(S2)), Gates::AND(S1, Gates::NOT(S0))));
    bool Y11 = Gates::AND(D, Gates::AND(Gates::AND(S3, Gates::NOT(S2)), Gates::AND(S1, S0)));

    bool Y12 = Gates::AND(D, Gates::AND(Gates::AND(S3, S2), Gates::AND(Gates::NOT(S1), Gates::NOT(S0))));
    bool Y13 = Gates::AND(D, Gates::AND(Gates::AND(S3, S2), Gates::AND(Gates::NOT(S1), S0)));
    bool Y14 = Gates::AND(D, Gates::AND(Gates::AND(S3, S2), Gates::AND(S1, Gates::NOT(S0))));
    bool Y15 = Gates::AND(D, Gates::AND(Gates::AND(S3, S2), Gates::AND(S1, S0)));

    return {
        Y0, Y1, Y2, Y3,
        Y4, Y5, Y6, Y7,
        Y8, Y9, Y10, Y11,
        Y12, Y13, Y14, Y15
    };
}

// Evaluates an SOP expression given the current input state and a truth table/minterm mask
bool combinational_circuits::SOP_Evaluator(bitset<4> current_inputs, bitset<16> active_minterms)
{
    bool A0 = current_inputs[3];
    bool A1 = current_inputs[2];
    bool A2 = current_inputs[1];
    bool A3 = current_inputs[0];
    //Decode the 4 bits to their corresponding value ( 0-15 )
    bool OUT0  = Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), Gates::NOT(A1)), Gates::NOT(A0));
    bool OUT1  = Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), Gates::NOT(A1)), A0);
    bool OUT2  = Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), A1), Gates::NOT(A0));
    bool OUT3  = Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), A1), A0);

    bool OUT4  = Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), A2), Gates::NOT(A1)), Gates::NOT(A0));
    bool OUT5  = Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), A2), Gates::NOT(A1)), A0);
    bool OUT6  = Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), A2), A1), Gates::NOT(A0));
    bool OUT7  = Gates::AND(Gates::AND(Gates::AND(Gates::NOT(A3), A2), A1), A0);

    bool OUT8  = Gates::AND(Gates::AND(Gates::AND(A3, Gates::NOT(A2)), Gates::NOT(A1)), Gates::NOT(A0));
    bool OUT9  = Gates::AND(Gates::AND(Gates::AND(A3, Gates::NOT(A2)), Gates::NOT(A1)), A0);
    bool OUT10 = Gates::AND(Gates::AND(Gates::AND(A3, Gates::NOT(A2)), A1), Gates::NOT(A0));
    bool OUT11 = Gates::AND(Gates::AND(Gates::AND(A3, Gates::NOT(A2)), A1), A0);

    bool OUT12 = Gates::AND(Gates::AND(Gates::AND(A3, A2), Gates::NOT(A1)), Gates::NOT(A0));
    bool OUT13 = Gates::AND(Gates::AND(Gates::AND(A3, A2), Gates::NOT(A1)), A0);
    bool OUT14 = Gates::AND(Gates::AND(Gates::AND(A3, A2), A1), Gates::NOT(A0));
    bool OUT15 = Gates::AND(Gates::AND(Gates::AND(A3, A2), A1), A0);

    bool E0 = Gates::AND(OUT0, active_minterms[0]);
    bool E1 = Gates::AND(OUT1, active_minterms[1]);
    bool E2 = Gates::AND(OUT2, active_minterms[2]);
    bool E3 = Gates::AND(OUT3, active_minterms[3]);

    bool E4 = Gates::AND(OUT4, active_minterms[4]);
    bool E5 = Gates::AND(OUT5, active_minterms[5]);
    bool E6 = Gates::AND(OUT6, active_minterms[6]);
    bool E7 = Gates::AND(OUT7, active_minterms[7]);

    bool E8 = Gates::AND(OUT8, active_minterms[8]);
    bool E9 = Gates::AND(OUT9, active_minterms[9]);
    bool E10 = Gates::AND(OUT10, active_minterms[10]);
    bool E11 = Gates::AND(OUT11, active_minterms[11]);

    bool E12 = Gates::AND(OUT12, active_minterms[12]);
    bool E13 = Gates::AND(OUT13, active_minterms[13]);
    bool E14 = Gates::AND(OUT14, active_minterms[14]);
    bool E15 = Gates::AND(OUT15, active_minterms[15]);

    bool res = Gates::OR(
      Gates::OR(Gates::OR(E0, E1), Gates::OR(E2, E3)),
      Gates::OR(
          Gates::OR(Gates::OR(E4, E5), Gates::OR(E6, E7)),
          Gates::OR(
              Gates::OR(Gates::OR(E8, E9), Gates::OR(E10, E11)),
              Gates::OR(Gates::OR(E12, E13), Gates::OR(E14, E15))
          )
      )
  );
    return res;
}

int main()
{
    //Waveform::Generate_Wave({0,1,0,1,0,0,0,0}); //01010000
    //vector<bool> res = combinational_circuits::_74HC85_(0b0101,0b0101);
    //Waveform::Generate_Wave(res);
    //vector<bool> result = Waveform::Clock_Signal(1,50,0,4);
    //for (int i = 0; i < result.size(); i++)
    //  cout << result[i];
    //cout << "\n";
    //Waveform::Generate_Wave(result);
    //Waveform::Timed_Wave({0,1, 1, 0},{1,1,1,1});
    //Waveform::Generate_Wave({0,1,1,0},{});
    // vector<int> res = Waveform::Rising_Edges({0,1,0,1,1,0});
    //for (int i = 0; i < res.size(); i++)
    //   cout << res[i] << " ";
    //vector<bool> res = Waveform::Concatenate({0,0,1,1,1,0},{1,1,0,0,1});
    //for (int i = 0; i < res.size(); i++)
    // cout << res[i];
   // cout << combinational_circuits::SOP_Evaluator(0b0101,0b0101101010110101);
    vector<bool> res = combinational_circuits::Multiplier_2x2(0b11,0b10);
    for (int i = res.size() - 1; i >= 0; i--)
        cout << res[i];
    return 0;
}