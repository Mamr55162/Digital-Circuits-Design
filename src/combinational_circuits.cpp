#include <iostream>
#include "combinational_circuits.h"
#include <bitset>
#include <vector>

//Implement the Half Adder circuit.
//Returns {Sum, Carry}.
vector<bool> combinational_circuits::Half_Adder(bool A, bool B)
{
    //Compute addition by XORing and carry by ANDing.
    bool sum = Gates::XOR(A, B);
    bool carry = Gates::AND(A, B);
    return {sum, carry};
}

//Implementation of Full Adder circuit.
//Returns {Sum, Carry}.
vector<bool> combinational_circuits::Full_Adder(bool A, bool B, bool C)
{
    bool sum = Gates::XOR(Gates::XOR(A, B), C);
    bool carry = Gates::OR(Gates::AND(A, B), Gates::AND(Gates::XOR(A, B), C));
    return {sum, carry};
}

//Implementation of Half Subtractor circuit.
//Returns {Diff, Borrow}.
vector<bool> combinational_circuits::Half_Subtractor(bool A, bool B)
{
    bool Diff = Gates::XOR(A,B);
    bool B_out = Gates::AND(Gates::NOT(A),B);
    return {Diff, B_out};
}

//Implementation of Full Subtractor circuit.
//Returns {Diff, Borrow}.
vector<bool> combinational_circuits::Full_Subtractor(bool A, bool B, bool B_in)
{
    bool Diff = Gates::XOR(Gates::XOR(A,B),B_in);
    bool B_out = Gates::OR(Gates::AND(Gates::NOT(A),B),Gates::OR(Gates::AND(Gates::NOT(A),B_in),Gates::AND(B,B_in)));
    return {Diff, B_out};
}
//Implementation of 74HC283 IC 4-Bit Full Adder/Subtractor.
//stat = 0 → perform A + B + C_in (addition).
//stat = 1 → perform A - B - C_in (subtraction, achieved by 1's-complementing B
//           and toggling the carry-in to form the 2's complement).
//Result layout: result[0] = C_out (carry-out), result[1..4] = S3,S2,S1,S0.
vector<bool> combinational_circuits::_74HC283_(bitset<4> A, bitset<4> B, bool C_in, bool stat)
{
    vector<bool> result(5);   //5-bit output: 1 carry + 4 sum bits

    // 1. Conditional inversion of B (1's complement if stat == 1)
    bitset<4> B_conditioned;
    for (int i = 0; i < 4; i++)
    {
        B_conditioned[i] = Gates::XOR(B[i], stat);
    }

    // 2. Derive the correct initial carry-in
    // XORing C_in with stat perfectly handles both standalone and cascaded operations.
    bool carry = Gates::XOR(C_in, stat);

    // 3. Ripple-carry adder loop
    for (int i = 0; i < 4; i++)
    {
        // Assuming Full_Adder returns {Sum, Cout}
        vector<bool> sum = Full_Adder(A[i], B_conditioned[i], carry);
        result[5 - i - 1] = sum[0]; // Store Sum bit (S0 to S3)
        carry = sum[1];             // Propagate Carry to next stage
    }

    // 4. A real 74HC283 IC ALWAYS outputs its final carry-out (C4)
    result[0] = carry;

    return result;
}

//Implement 4-bit Carry Lookahead Adder
//Returns {C_out, S3, S2, S1, S0}.
//Carry-lookahead uses G_i = A_i·B_i and P_i = A_i XOR B_i to compute every carry
//in parallel (depth = O(1) per carry) instead of rippling through 4 stages.
vector<bool> combinational_circuits::Carry_Lookahead_Adder_4bit(bitset<4> A, bitset<4> B, bool C_in)
{
    // Generate (G) terms
    bool G0 = Gates::AND(A[0], B[0]);
    bool G1 = Gates::AND(A[1], B[1]);
    bool G2 = Gates::AND(A[2], B[2]);
    bool G3 = Gates::AND(A[3], B[3]);

    // Propagate (P) terms
    bool P0 = Gates::XOR(A[0], B[0]);
    bool P1 = Gates::XOR(A[1], B[1]);
    bool P2 = Gates::XOR(A[2], B[2]);
    bool P3 = Gates::XOR(A[3], B[3]);

    // Carry Lookahead Logic Generator
    // C_n+1 = G_n + P_n·C_n
    bool C1 = Gates::OR(G0, Gates::AND(P0, C_in));
    bool C2 = Gates::OR(G1, Gates::AND(P1, C1));
    bool C3 = Gates::OR(G2, Gates::AND(P2, C2));
    bool C4 = Gates::OR(G3, Gates::AND(P3, C3));

    // Sum Logic: S_n = P_n XOR C_n
    bool S0 = Gates::XOR(P0, C_in);
    bool S1 = Gates::XOR(P1, C1);
    bool S2 = Gates::XOR(P2, C2);
    bool S3 = Gates::XOR(P3, C3);

    // Order: {C_out, S3, S2, S1, S0}
    return {C4, S3, S2, S1, S0};
}

//Implementation of 2x2 Multiplier
//Computes A·B as a 4-bit product. Returns {M0, M1, M2, M3} (LSB first).
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
// Array multiplier: 16 partial products → 3 stages of half/full adders.
// Returns {M0..M7} (LSB first), 8-bit product.
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
//Returns true iff every bit of A matches the corresponding bit of B (4-bit XNOR-reduce).
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
//Note: the comment title is the original; the chip is actually a 4-bit comparator
//with cascading inputs (eq/larger/smaller) for chaining wider comparisons.
//Returns {A>B, A==B, A<B}.
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
//Select codes: S1 S0 = 00→A[0], 01→A[1], 10→A[2], 11→A[3].
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
//Select convention used here: S0 is treated as the MSB of the 3-bit select code
//(S0 S1 S2), so each Di is gated by the minterm of (S0,S1,S2) equal to i.
vector<bool> combinational_circuits::_74HC151_(bitset<8> A, bool S0, bool S1, bool S2)
{
    //FIX: D0 minterm must be NOT(S0)·NOT(S1)·NOT(S2). The original code wrote
    //NOT(S0) twice instead of NOT(S2), which made D0 selected for both S=000 and S=100.
    bool D0 = Gates::AND(Gates::AND(A[0], Gates::NOT(S0)), Gates::AND(Gates::NOT(S1), Gates::NOT(S2)));
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
//Inputs: A (MSB), B (LSB). Returns {Y0, Y1, Y2, Y3} (active-high, one-hot).
vector<bool> combinational_circuits::Decoder_2_to_4(bool A, bool B)
{
    bool A1 = Gates::AND(Gates::NOT(A), Gates::NOT(B));
    bool A2 = Gates::AND(Gates::NOT(A), B);
    bool A3 = Gates::AND(A, Gates::NOT(B));
    bool A4 = Gates::AND(A, B);
    return {A1, A2, A3, A4};
}


//Implementation of 74HC154 IC 4-to-16 Decoder.
//Inputs A3..A0 (A3 is MSB). Outputs are active-low (one selected output = 0, others = 1).
//When EN=0, ALL outputs go HIGH (chip disabled).
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
//The internal decoder produces one-hot select lines D0..D3, which gate the
//corresponding data input I0..I3 into the OR-tree output.
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
//Inputs A3..A0 form a BCD nibble (A3 is MSB). Outputs are active-low; for BCD
//values > 9 (i.e. 1010..1111) ALL outputs stay HIGH (no valid decoding).
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
//Inputs A3..A0 form a BCD nibble (A3 is MSB). `decimal` controls the decimal point.
//Outputs the 8 segment signals {a,b,c,d,e,f,g,dp} (active-high, common cathode)
//AND prints an ASCII rendering of the digit to stdout.
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
    // Each segment is the OR of every digit that lights it.

    bool a = Gates::OR(Gates::OR(Gates::OR(D0, D2), Gates::OR(D3, D5)),
                       Gates::OR(Gates::OR(D6, D7), Gates::OR(D8, D9)));

    bool b = Gates::OR(Gates::OR(Gates::OR(D0, D1), Gates::OR(D2, D3)),
                       Gates::OR(Gates::OR(D4, D7), Gates::OR(D8, D9)));

    //FIX: segment c is also ON for digit 9 (D9 was missing from the OR chain).
    bool c = Gates::OR(Gates::OR(Gates::OR(D0, D1), Gates::OR(D3, D4)),
                       Gates::OR(Gates::OR(D5, D6), Gates::OR(Gates::OR(D7, D8), D9)));

    bool d = Gates::OR(Gates::OR(Gates::OR(D0, D2), Gates::OR(D3, D5)),
                       Gates::OR(Gates::OR(D6, D8), D9));

    bool e = Gates::OR(Gates::OR(D0, D2), Gates::OR(D6, D8));

    bool f = Gates::OR(Gates::OR(Gates::OR(D0, D4), Gates::OR(D5, D6)),
                       Gates::OR(D8, D9));

    //FIX: segment g is also ON for digit 9 (D9 was missing from the OR chain).
    bool g = Gates::OR(Gates::OR(Gates::OR(D2, D3), Gates::OR(D4, D5)),
                       Gates::OR(Gates::OR(D6, D8), D9));

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

//Simple 4-to-2 line encoder (no priority, assumes exactly one input is active).
//Returns {Q1, Q0} where Q1 = D2|D3, Q0 = D1|D3.
//Note: D0 is implicit — when only D0 is active, the output is 00 by default.
vector<bool> combinational_circuits::Encoder_4_to_2(bool D0, bool D1, bool D2, bool D3)
{
    bool Q1 = Gates::OR(D2, D3);
    bool Q0 = Gates::OR(D1, D3);

    return {Q1, Q0};
}

//10-line decimal-to-BCD encoder. Returns {A3, A2, A1, A0} (BCD, A3 = MSB).
//Assumes exactly one Di is active; D0 is the implicit default (output 0000).
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

// Implementation of an Active-HIGH 8-to-3 Priority Encoder.
// D[7] has the highest priority, D[0] has the lowest.
vector<bool> combinational_circuits::_74HC148_(bitset<8> D)
{
    // 1. Generate "Winner Lines" (W) using a sequential "None Higher" (N) cascade.
    // An active bit only wins if no higher-priority bit has blocked its path.

    bool W7 = D[7];
    bool N7 = Gates::NOT(D[7]); // N7 = True only if D[7] is inactive

    bool W6 = Gates::AND(D[6], N7);
    bool N6 = Gates::AND(N7, Gates::NOT(D[6]));

    bool W5 = Gates::AND(D[5], N6);
    bool N5 = Gates::AND(N6, Gates::NOT(D[5]));

    bool W4 = Gates::AND(D[4], N5);
    bool N4 = Gates::AND(N5, Gates::NOT(D[4]));

    bool W3 = Gates::AND(D[3], N4);
    bool N3 = Gates::AND(N4, Gates::NOT(D[3]));

    bool W2 = Gates::AND(D[2], N3);
    bool N2 = Gates::AND(N3, Gates::NOT(D[2]));

    bool W1 = Gates::AND(D[1], N2);
    bool N1 = Gates::AND(N2, Gates::NOT(D[1]));

    bool W0 = Gates::AND(D[0], N1);

    // 2. Encode the single active Winner line into 3-bit binary (Y2, Y1, Y0)

    // Y2 is HIGH for winners 4, 5, 6, 7
    bool Y2 = Gates::OR(
        Gates::OR(W4, W5),
        Gates::OR(W6, W7)
    );

    // Y1 is HIGH for winners 2, 3, 6, 7
    bool Y1 = Gates::OR(
        Gates::OR(W2, W3),
        Gates::OR(W6, W7)
    );

    // Y0 is HIGH for winners 1, 3, 5, 7
    bool Y0 = Gates::OR(
        Gates::OR(W1, W3),
        Gates::OR(W5, W7)
    );

    // Return ordered as {MSB, Middle, LSB} -> {Y2, Y1, Y0}
    return {Y2, Y1, Y0};
}

//Implementation of 74HC147 IC, a Decimal-to-BCD priority Encoder with active low inputs and outputs.
//D9 has the highest priority, D1 the lowest (D0 is implicit and never an input on the real chip).
//Each Wi is asserted (active-HIGH internally) when Di is the lowest-numbered active-low input.
//Final outputs are inverted to match the 74HC147's active-low BCD output pins.
vector<bool> combinational_circuits::_74HC147_(
    bool D0, bool D1, bool D2, bool D3, bool D4,
    bool D5, bool D6, bool D7, bool D8, bool D9)
{
    // Winner lines (active HIGH internally)

    // W9 fires when D9 is asserted active-low (D9 == 0).
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
//Routes single input D to one of 4 outputs selected by S1 S0 (S1 = MSB).
//Returns {Y0, Y1, Y2, Y3}, where exactly one equals D and the others are 0.
vector<bool> combinational_circuits::DEMUX_1_to_4(bool D, bool S0, bool S1)
{
    bool Y0 = Gates::AND(Gates::AND(Gates::NOT(S1),Gates::NOT(S0)),D);
    bool Y1 = Gates::AND(Gates::AND(Gates::NOT(S1),S0),D);
    bool Y2 = Gates::AND(Gates::AND(S1,Gates::NOT(S0)),D);
    bool Y3 = Gates::AND(Gates::AND(S1,S0),D);
    return {Y0, Y1, Y2, Y3};
}

//Implementation of 1-to-16 DEMUX using 4-to-16 Decoder
//Routes D to one of 16 outputs selected by S3 S2 S1 S0 (S3 = MSB).
//Returns {Y0..Y15}; the selected output equals D, all others are 0.
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
//current_inputs[3..0] = A3..A0 (A3 = MSB). active_minterms[i] = 1 means minterm i is included.
//Returns the OR of every active minterm whose 4-bit address matches current_inputs.
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

//Implement 4-bit Parity Generator.
//Returns {even_parity, odd_parity} for the 4-bit input A.
vector<bool> combinational_circuits::Parity_Generator_4bit(bitset<4> A)
{
    bool even_parity = Gates::XOR(Gates::XOR(A[0],A[1]),Gates::XOR(A[2],A[3]));
    bool odd_parity = Gates::NOT(even_parity);
    return {even_parity, odd_parity};
}

//Implement 4-bit Parity Checker.
//Returns true (error) when the recomputed parity does not match the convention.
//`even`=true → even-parity system (good transmission has total XOR = 0).
//`even`=false → odd-parity system  (good transmission has total XOR = 1).
bool combinational_circuits::Parity_Checker_4bit(bitset<4> A, bool received_parity, bool even)
{
    bool data_parity = Gates::XOR(Gates::XOR(A[0],A[1]),Gates::XOR(A[2],A[3]));
    bool parity_mismatch = Gates::XOR(data_parity,received_parity);
    return Gates::XOR(Gates::NOT(even),parity_mismatch);
}


//Implement 74HC280 IC 9-bit Parity Generator/Checker
//Returns {even, odd} parity outputs for the 9-bit input I.
//Tree-structured XOR network (8 XORs in 3 stages + 1 final XOR for the 9th bit).
vector<bool> combinational_circuits::_74HC280_(bitset<9> I)
{
    // Stage 1: Group the first 8 bits into four parallel XOR gates
    bool s1_0 = Gates::XOR(I[0], I[1]);
    bool s1_1 = Gates::XOR(I[2], I[3]);
    bool s1_2 = Gates::XOR(I[4], I[5]);
    bool s1_3 = Gates::XOR(I[6], I[7]);

    // Stage 2: Compress to 2 bits
    bool s2_0 = Gates::XOR(s1_0, s1_1);
    bool s2_1 = Gates::XOR(s1_2, s1_3);

    // Stage 3: The XOR sum of the first 8 bits
    bool s3 = Gates::XOR(s2_0, s2_1);

    // Final Stage: XOR the 9th bit to get the total Odd Parity
    bool odd_parity = Gates::XOR(s3, I[8]);

    // Even parity is simply the inverted Odd Parity
    bool even_parity = Gates::NOT(odd_parity);

    // 74HC280 outputs are typically {Even, Odd}
    return {even_parity, odd_parity};
}

// Code Converter from Binary to Gray
//Standard rule: G_i = B_i XOR B_{i+1} for i < n-1; G_{n-1} = B_{n-1} (MSB passes through).
//Returns {G0, G1, G2, G3} (LSB first).
vector<bool> combinational_circuits::Binary_to_Gray(bitset<4> B)
{
    bool OUT0 = B[3];
    bool OUT1 = Gates::XOR(B[3],B[2]);
    bool OUT2 = Gates::XOR(B[2],B[1]);
    bool OUT3 = Gates::XOR(B[1],B[0]);
    return {OUT3, OUT2, OUT1, OUT0};
}

// Code Converter from Gray to Binary
//Standard rule: B_i = G_i XOR B_{i+1} for i < n-1; B_{n-1} = G_{n-1}.
//Implemented as a prefix-XOR cascade from MSB down to LSB.
//Returns {B0, B1, B2, B3} (LSB first).
vector<bool> combinational_circuits::Gray_to_Binary(bitset<4> G)
{
    bool OUT0 = G[3];                             // B3 = G3 (MSB)
    bool OUT1 = Gates::XOR(OUT0, G[2]);           // B2 = B3 XOR G2
    bool OUT2 = Gates::XOR(OUT1, G[1]);           // B1 = B2 XOR G1
    bool OUT3 = Gates::XOR(OUT2, G[0]);           // B0 = B1 XOR G0 (LSB)

    return {OUT3, OUT2, OUT1, OUT0};
}

// Code Converter from BCD to Excess-3
//Excess-3 = BCD + 3. Implemented bit-by-bit using the sum/carry equations
//of adding 0011 to the BCD nibble. Returns {E0, E1, E2, E3} (LSB first).
vector<bool> combinational_circuits::BCD_to_Excess3(bitset<4> BCD)
{
    // E0 = NOT(B0)
    bool OUT3 = Gates::NOT(BCD[0]);

    // E1 = NOT(B1 XOR B0)
    bool OUT2 = Gates::NOT(Gates::XOR(BCD[1], BCD[0]));

    // Shared term for E2 and E3
    bool OR_1_0 = Gates::OR(BCD[1], BCD[0]);

    // E2 = B2 XOR (B1 OR B0)
    bool OUT1 = Gates::XOR(BCD[2], OR_1_0);

    // E3 = B3 OR (B2 AND (B1 OR B0))
    bool OUT0 = Gates::OR(BCD[3], Gates::AND(BCD[2], OR_1_0));

    return {OUT3, OUT2, OUT1, OUT0};
}

// Code Converter from Excess-3 to BCD
//Inverse of BCD_to_Excess3: BCD = Excess-3 - 3. Returns {B0, B1, B2, B3} (LSB first).
vector<bool> combinational_circuits::Excess3_to_BCD(bitset<4> EX3)
{
    // B0 = NOT(E0)
    bool OUT3 = Gates::NOT(EX3[0]);

    // B1 = E1 XOR E0
    bool OUT2 = Gates::XOR(EX3[1], EX3[0]);

    // Shared term for B2 and B3
    bool AND_1_0 = Gates::AND(EX3[1], EX3[0]);

    // B2 = NOT(E2 XOR (E1 AND E0))  -> XNOR logic
    bool OUT1 = Gates::NOT(Gates::XOR(EX3[2], AND_1_0));

    // B3 = E3 AND (E2 OR (E1 AND E0))
    bool OUT0 = Gates::AND(EX3[3], Gates::OR(EX3[2], AND_1_0));

    return {OUT3, OUT2, OUT1, OUT0};
}

// Implementation of a Hexadecimal-to-7-Segment Decoder (0-9, A-F)
// Standard Active-HIGH outputs for Common Cathode displays.
vector<bool> combinational_circuits::Hex_to_7_Segment(bitset<4> hex)
{
    // Extract inputs for readability (A3 is MSB, A0 is LSB)
    bool A3 = hex[3];
    bool A2 = hex[2];
    bool A1 = hex[1];
    bool A0 = hex[0];

    // 1. Full 16-State Minterm Decoding
    bool D0  = Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), Gates::AND(Gates::NOT(A1), Gates::NOT(A0)));
    bool D1  = Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), Gates::AND(Gates::NOT(A1), A0));
    bool D2  = Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), Gates::AND(A1, Gates::NOT(A0)));
    bool D3  = Gates::AND(Gates::AND(Gates::NOT(A3), Gates::NOT(A2)), Gates::AND(A1, A0));

    bool D4  = Gates::AND(Gates::AND(Gates::NOT(A3), A2), Gates::AND(Gates::NOT(A1), Gates::NOT(A0)));
    bool D5  = Gates::AND(Gates::AND(Gates::NOT(A3), A2), Gates::AND(Gates::NOT(A1), A0));
    bool D6  = Gates::AND(Gates::AND(Gates::NOT(A3), A2), Gates::AND(A1, Gates::NOT(A0)));
    bool D7  = Gates::AND(Gates::AND(Gates::NOT(A3), A2), Gates::AND(A1, A0));

    bool D8  = Gates::AND(Gates::AND(A3, Gates::NOT(A2)), Gates::AND(Gates::NOT(A1), Gates::NOT(A0)));
    bool D9  = Gates::AND(Gates::AND(A3, Gates::NOT(A2)), Gates::AND(Gates::NOT(A1), A0));
    bool D10 = Gates::AND(Gates::AND(A3, Gates::NOT(A2)), Gates::AND(A1, Gates::NOT(A0))); // A
    bool D11 = Gates::AND(Gates::AND(A3, Gates::NOT(A2)), Gates::AND(A1, A0));           // b

    bool D12 = Gates::AND(Gates::AND(A3, A2), Gates::AND(Gates::NOT(A1), Gates::NOT(A0))); // C
    bool D13 = Gates::AND(Gates::AND(A3, A2), Gates::AND(Gates::NOT(A1), A0));           // d
    bool D14 = Gates::AND(Gates::AND(A3, A2), Gates::AND(A1, Gates::NOT(A0)));           // E
    bool D15 = Gates::AND(Gates::AND(A3, A2), Gates::AND(A1, A0));                       // F

    // 2. Segment Logic Routing

    // Segment A (ON for: 0, 2, 3, 5, 6, 7, 8, 9, A, C, E, F)
    bool a_p1 = Gates::OR(Gates::OR(D0, D2), Gates::OR(D3, D5));
    bool a_p2 = Gates::OR(Gates::OR(D6, D7), Gates::OR(D8, D9));
    bool a_p3 = Gates::OR(Gates::OR(D10, D12), Gates::OR(D14, D15));
    bool a = Gates::OR(Gates::OR(a_p1, a_p2), a_p3);

    // Segment B (ON for: 0, 1, 2, 3, 4, 7, 8, 9, A, d)
    bool b_p1 = Gates::OR(Gates::OR(D0, D1), Gates::OR(D2, D3));
    bool b_p2 = Gates::OR(Gates::OR(D4, D7), Gates::OR(D8, D9));
    bool b = Gates::OR(Gates::OR(b_p1, b_p2), Gates::OR(D10, D13));

    // Segment C (ON for: 0, 1, 3, 4, 5, 6, 7, 8, 9, A, b, d)
    bool c_p1 = Gates::OR(Gates::OR(D0, D1), Gates::OR(D3, D4));
    bool c_p2 = Gates::OR(Gates::OR(D5, D6), Gates::OR(D7, D8));
    bool c_p3 = Gates::OR(Gates::OR(D9, D10), Gates::OR(D11, D13));
    bool c = Gates::OR(Gates::OR(c_p1, c_p2), c_p3);

    // Segment D (ON for: 0, 2, 3, 5, 6, 8, 9, b, C, d, E)
    bool d_p1 = Gates::OR(Gates::OR(D0, D2), Gates::OR(D3, D5));
    bool d_p2 = Gates::OR(Gates::OR(D6, D8), Gates::OR(D9, D11));
    bool d_p3 = Gates::OR(Gates::OR(D12, D13), D14);
    bool d = Gates::OR(Gates::OR(d_p1, d_p2), d_p3);

    // Segment E (ON for: 0, 2, 6, 8, A, b, C, d, E, F)
    bool e_p1 = Gates::OR(Gates::OR(D0, D2), Gates::OR(D6, D8));
    bool e_p2 = Gates::OR(Gates::OR(D10, D11), Gates::OR(D12, D13));
    bool e = Gates::OR(Gates::OR(e_p1, e_p2), Gates::OR(D14, D15));

    // Segment F (ON for: 0, 4, 5, 6, 8, 9, A, b, C, E, F)
    bool f_p1 = Gates::OR(Gates::OR(D0, D4), Gates::OR(D5, D6));
    bool f_p2 = Gates::OR(Gates::OR(D8, D9), Gates::OR(D10, D11));
    bool f_p3 = Gates::OR(Gates::OR(D12, D14), D15);
    bool f = Gates::OR(Gates::OR(f_p1, f_p2), f_p3);

    // Segment G (ON for: 2, 3, 4, 5, 6, 8, 9, A, b, d, E, F)
    bool g_p1 = Gates::OR(Gates::OR(D2, D3), Gates::OR(D4, D5));
    bool g_p2 = Gates::OR(Gates::OR(D6, D8), Gates::OR(D9, D10));
    bool g_p3 = Gates::OR(Gates::OR(D11, D13), Gates::OR(D14, D15));
    bool g = Gates::OR(Gates::OR(g_p1, g_p2), g_p3);

    // Decimal Point (Hardcoded to OFF since it's not in the input parameters)
    bool dp = 0;

    // Return all 8 pins to maintain interface compatibility with your display wrapper
    return {a, b, c, d, e, f, g, dp};
}

//Overflow Detection
//Detects 4-bit two's-complement overflow for A ± B.
//`addition`=true → check A+B overflow; `addition`=false → check A-B overflow.
//Overflow rule: V = (signs_match_of_operands) AND (result_sign != A_sign).
//For subtraction the effective B sign is inverted (since A-B == A + (-B)).
bool combinational_circuits::Overflow_Detect(bitset<4> A, bitset<4> B, bool C_in, bool addition)
{
    bool A_sign = A[3];
    bool B_sign = B[3];
    if (addition)
    {
        vector<bool> add = combinational_circuits::_74HC283_(A,B,C_in,0);
        bool Sum_sign = add[1];
        bool signs_match = Gates::NOT(Gates::XOR(A_sign, B_sign));
        bool sum_is_wrong = Gates::XOR(A_sign, Sum_sign);

        return Gates::AND(signs_match, sum_is_wrong);
    }
    else
    {
         vector<bool> diff = combinational_circuits::_74HC283_(A,B,C_in,1);
         bool Sum_sign = diff[1];
        // For subtraction, you are adding the 2's complement of B.
        // Therefore, the effective sign of B entering the adder is inverted.
        bool B_eff_sign = Gates::NOT(B_sign);

        // V = (A_sign XNOR B_eff_sign) AND (A_sign XOR Sum_sign)
        bool signs_match = Gates::NOT(Gates::XOR(A_sign, B_eff_sign));
        bool sum_is_wrong = Gates::XOR(A_sign, Sum_sign);

        return Gates::AND(signs_match, sum_is_wrong);
    }
}

//Detect if a 4-bit number is fully composed of zeros.
//Returns true iff A == 0000 (i.e. NOR-reduce of all 4 bits).
bool combinational_circuits::Zero_Detect(bitset<4> A)
{
    bool result = Gates::OR(Gates::OR(A[0],A[1]),Gates::OR(A[2],A[3]));
    return Gates::NOT(result);
}


//The original scratch main() is preserved verbatim below but disabled with #if 0
//so that an external test driver (test_combinational_circuits.cpp) can supply its
//own main() without causing a duplicate-symbol link error. Toggle the #if to 1 to
//re-enable this main() in a standalone build of combinational_circuits.cpp.
#if 0
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
   // vector<bool> res = combinational_circuits::Binary_to_Gray(0b1011);
   // for (int i = res.size() - 1; i >= 0; i--)
    //    cout << res[i];
   //cout << combinational_circuits::Overflow_Detect(0b0101, 0b0110,false,true);
   // vector<bool> res = combinational_circuits::_74HC283_(0b0101,0b0101,0,0);
   // cout << res[1];
    //cout << combinational_circuits::Zero_Detect(0b0010);

    return 0;
}
#endif