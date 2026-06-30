// =============================================================================
// test_combinational_circuits.cpp
// -----------------------------------------------------------------------------
// Minimal smoke-test driver: calls every public function declared in
// combinational_circuits.h (in header order) with a few sample inputs and
// prints the result. No framework, no assertions — just "call it and look".
//
// Build:
//   g++ -std=c++17 combinational_circuits.cpp test_combinational_circuits.cpp \
//       -o test_combinational_circuits
// =============================================================================

#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include "combinational_circuits.h"

using namespace std;

// Helper: print a vector<bool> as "0"/"1" characters.
static void print_v(const string& label, const vector<bool>& v)
{
    cout << label << " = ";
    for (bool b : v) cout << b;
    cout << "\n";
}

// Helper: print a vector<int> as space-separated integers.
static void print_vi(const string& label, const vector<int>& v)
{
    cout << label << " =";
    for (int x : v) cout << " " << x;
    cout << "\n";
}

int main()
{
    cout << "=== combinational_circuits smoke test ===\n";

    // ----- 1. Wire -----
    cout << "\n--- Wire ---\n";
    Wire w;
    w.set(true);
    cout << "Wire after set(true): " << w.get() << "\n";

    // ----- 2. Indicator -----
    cout << "\n--- Indicator ---\n";
    Indicator::LED(true);
    Indicator::LED(false);
    Indicator::Binary(true);
    Indicator::BinaryVector({1, 0, 1, 1});
    Indicator::Probe(true);
    Indicator::Decimal(bitset<4>(0b1001));
    Indicator::Bus(bitset<4>(0b0101));

    // ----- 3. Waveform -----
    cout << "\n--- Waveform ---\n";
    vector<bool> clk = Waveform::Clock_Signal(1.0, 50.0, false, 4);
    print_v("Clock_Signal(1Hz,50%,LOW,4)", clk);

    Waveform::Generate_Wave({0, 1, 0, 1}, {});
    Waveform::Timed_Wave({0, 1, 1, 0}, {1, 1, 1, 1});

    print_vi("Rising_Edges({0,1,0,1,1,0})",  Waveform::Rising_Edges({0,1,0,1,1,0}));
    print_vi("Falling_Edges({1,1,0,1,0,0})", Waveform::Falling_Edges({1,1,0,1,0,0}));
    print_vi("All_Edges({0,1,0,1})",          Waveform::All_Edges({0,1,0,1}));

    cout << "Count_Pulses({0,1,0,1,0,1}) = " << Waveform::Count_Pulses({0,1,0,1,0,1}) << "\n";
    cout << "Duty_Cycle({1,1,0,0}) = "      << Waveform::Duty_Cycle({1,1,0,0})      << "\n";
    cout << "Max_Pulse_Width({0,1,1,1,0,1}) = " << Waveform::Max_Pulse_Width({0,1,1,1,0,1}) << "\n";
    cout << "Glitch_Density({0,1,0,1}) = "  << Waveform::Glitch_Density({0,1,0,1})  << "\n";

    print_v("Invert({1,0,1,0})", Waveform::Invert({1,0,1,0}));
    print_v("AND_Signals({1,1,0,0},{1,0,1,0})", Waveform::AND_Signals({1,1,0,0},{1,0,1,0}));
    print_v("OR_Signals({1,0,0,0},{0,0,1,0})",  Waveform::OR_Signals({1,0,0,0},{0,0,1,0}));
    print_v("XOR_Signals({1,1,0,0},{1,0,1,0})", Waveform::XOR_Signals({1,1,0,0},{1,0,1,0}));
    print_v("Shift_right({1,1,1,1},2)", Waveform::Shift_right({1,1,1,1}, 2));
    print_v("Stretch({1,0},3)",          Waveform::Stretch({1,0}, 3));
    print_v("Slice({1,0,1,1,0,1},1,4)",  Waveform::Slice({1,0,1,1,0,1}, 1, 4));
    print_v("Concatenate({1,0,1},{0,1})", Waveform::Concatenate({1,0,1}, {0,1}));

    // ----- 4. twoLogicConverter -----
    cout << "\n--- twoLogicConverter (XOR truth table) ---\n";
    twoLogicConverter::TruthTable(0, 1, 1, 0);
    cout << "SOP_Expression(XOR) = " << twoLogicConverter::SOP_Expression(0,1,1,0) << "\n";
    cout << "POS_Expression(XOR) = " << twoLogicConverter::POS_Expression(0,1,1,0) << "\n";
    twoLogicConverter::SOP_GateConstruction(0,1,1,0);
    twoLogicConverter::POS_GateConstruction(0,1,1,0);
    twoLogicConverter::NANDConstruction(0,1,1,0);

    // ----- 5. threeLogicConverter -----
    cout << "\n--- threeLogicConverter ---\n";
    threeLogicConverter::TruthTable(1,0,0,0, 0,0,0,0);
    cout << "SOP = " << threeLogicConverter::SOP_Expression(1,0,0,0, 0,0,0,0) << "\n";
    cout << "POS = " << threeLogicConverter::POS_Expression(1,0,0,0, 0,0,0,0) << "\n";
    threeLogicConverter::SOP_GateConstruction(1,1,1,1, 1,1,1,1);
    threeLogicConverter::POS_GateConstruction(0,0,0,0, 0,0,0,0);

    // ----- 6. fourLogicConverter -----
    cout << "\n--- fourLogicConverter ---\n";
    fourLogicConverter::TruthTable(1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);
    cout << "SOP = " << fourLogicConverter::SOP_Expression(1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0) << "\n";
    cout << "POS = " << fourLogicConverter::POS_Expression(1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0) << "\n";
    fourLogicConverter::SOP_GateConstruction(1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1);
    fourLogicConverter::POS_GateConstruction(0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);

    // ----- 7. TruthTable -----
    cout << "\n--- TruthTable (gate truth tables) ---\n";
    TruthTable::ANDTruthTable();
    TruthTable::ORTruthTable();
    TruthTable::NANDTruthTable();
    TruthTable::NORTruthTable();
    TruthTable::XORTruthTable();
    TruthTable::XNORTruthTable();
    TruthTable::NOTTruthTable();
    TruthTable::Tri_stateTruthTable();

    // ----- 8. Gates -----
    cout << "\n--- Gates ---\n";
    cout << "NAND(1,1)=" << Gates::NAND(1,1) << "  NAND(0,1)=" << Gates::NAND(0,1) << "\n";
    cout << "NOT(0)="    << Gates::NOT(0)    << "  NOT(1)="    << Gates::NOT(1)    << "\n";
    cout << "OR(0,1)="   << Gates::OR(0,1)   << "  OR(0,0)="   << Gates::OR(0,0)   << "\n";
    cout << "AND(1,1)="  << Gates::AND(1,1)  << "  AND(1,0)="  << Gates::AND(1,0)  << "\n";
    cout << "NOR(0,0)="  << Gates::NOR(0,0)  << "  NOR(1,1)="  << Gates::NOR(1,1)  << "\n";
    cout << "XOR(1,0)="  << Gates::XOR(1,0)  << "  XOR(1,1)="  << Gates::XOR(1,1)  << "\n";
    cout << "XNOR(0,0)=" << Gates::XNOR(0,0) << "  XNOR(1,0)=" << Gates::XNOR(1,0) << "\n";
    cout << "Tri_state(1,0)=" << (int)Gates::Tri_state(1,0)
         << "  Tri_state(1,1)=" << (int)Gates::Tri_state(1,1) << "\n";

    // ----- 9. ALU_74181 -----
    cout << "\n--- ALU_74181 (XNOR logic, A+B arithmetic) ---\n";
    print_v("74HC181 logic  S=1001 M=1 A=1010 B=1100",
            ALU_74181::_74HC181_(0b1010, 0b1100, 0b1001, 1, 0));
    print_v("74HC181 arith  S=1001 M=0 A=0011 B=0101 Cin=0 (3+5=8)",
            ALU_74181::_74HC181_(0b0011, 0b0101, 0b1001, 0, 0));
    print_v("74HC181 arith  S=1001 M=0 A=0011 B=0101 Cin=1 (3+5+1=9)",
            ALU_74181::_74HC181_(0b0011, 0b0101, 0b1001, 0, 1));

    // ----- 10. combinational_circuits -----
    cout << "\n--- combinational_circuits ---\n";

    print_v("Half_Adder(1,1)",           combinational_circuits::Half_Adder(1,1));
    print_v("Full_Adder(1,1,1)",         combinational_circuits::Full_Adder(1,1,1));
    print_v("Half_Subtractor(0,1)",      combinational_circuits::Half_Subtractor(0,1));
    print_v("Full_Subtractor(0,1,1)",    combinational_circuits::Full_Subtractor(0,1,1));

    // _74HC283_: result is {C_out, S3, S2, S1, S0}.
    print_v("74HC283 add  A=0101 B=0011 Cin=0 stat=0  (5+3=8)",
            combinational_circuits::_74HC283_(0b0101, 0b0011, 0, 0));
    print_v("74HC283 sub  A=0101 B=0011 Cin=0 stat=1  (5-3=2)",
            combinational_circuits::_74HC283_(0b0101, 0b0011, 0, 1));

    print_v("Carry_Lookahead_Adder_4bit A=0111 B=0001 Cin=0 (7+1=8)",
            combinational_circuits::Carry_Lookahead_Adder_4bit(0b0111, 0b0001, 0));

    print_v("Multiplier_2x2 A=11 B=11 (3*3=9)",
            combinational_circuits::Multiplier_2x2(0b11, 0b11));
    print_v("Multiplier_4x4 A=0011 B=0101 (3*5=15, LSB-first)",
            combinational_circuits::Multiplier_4x4(0b0011, 0b0101));

    cout << "Identity_Comparator(1010,1010) = "
         << combinational_circuits::Identity_Comparator(0b1010, 0b1010) << "\n";
    cout << "Identity_Comparator(1010,1011) = "
         << combinational_circuits::Identity_Comparator(0b1010, 0b1011) << "\n";

    print_v("74HC85 A=1000 B=0111 (A>B)",  combinational_circuits::_74HC85_(0b1000, 0b0111));
    print_v("74HC85 A=0011 B=0100 (A<B)",  combinational_circuits::_74HC85_(0b0011, 0b0100));
    print_v("74HC85 A=0101 B=0101 (A==B)", combinational_circuits::_74HC85_(0b0101, 0b0101));

    cout << "MUX_4_to_1 A=0101 S0=0 S1=0 = "
         << combinational_circuits::MUX_4_to_1(0b0101, 0, 0) << "  (selects A[0]=1)\n";
    cout << "MUX_4_to_1 A=0101 S0=1 S1=0 = "
         << combinational_circuits::MUX_4_to_1(0b0101, 1, 0) << "  (selects A[2]=1)\n";

    print_v("74HC151 A=00000001 S0S1S2=000 (selects A[0]=1)",
            combinational_circuits::_74HC151_(0b00000001, 0, 0, 0));
    print_v("74HC151 A=00010000 S0S1S2=100 (selects A[4]=1)",
            combinational_circuits::_74HC151_(0b00010000, 1, 0, 0));

    print_v("Decoder_2_to_4 A=0 B=1",  combinational_circuits::Decoder_2_to_4(0, 1));
    print_v("74HC154 addr=0101 EN=1 (active-low: OUT5=0)",
            combinational_circuits::_74HC154_(1, 0, 1, 0, 1));
    print_v("74HC154 EN=0 (disabled: all HIGH)",
            combinational_circuits::_74HC154_(0, 0, 0, 0, 0));

    cout << "Decoder_to_MUX S1=0 S0=1 I1=1 others=0 -> "
         << combinational_circuits::Decoder_to_MUX(0, 1, 0, 1, 0, 0) << "\n";

    print_v("74HC42 A=0101 (BCD=5, OUT5=0)",
            combinational_circuits::_74HC42_(1, 0, 1, 0));

    cout << "\n--- BCD_to_7_Segment (digit 9, exercises the c/g D9 fix) ---\n";
    print_v("BCD_to_7_Segment A3..A0=1001 dec=0 (expect 11110110)",
            combinational_circuits::BCD_to_7_Segment(1, 0, 0, 1, 0));

    print_v("Encoder_4_to_2 D3=1", combinational_circuits::Encoder_4_to_2(0,0,0,1));
    print_v("Decimal_to_BCD_Encoder D5=1", combinational_circuits::Decimal_to_BCD_Encoder(0,0,0,0,0,1,0,0,0,0));
    print_v("74HC148 D=10000000 (priority=7)", combinational_circuits::_74HC148_(0b10000000));
    print_v("74HC147 D9=0 others=1 (active-low 9)", combinational_circuits::_74HC147_(1,1,1,1,1,1,1,1,1,0));

    print_v("DEMUX_1_to_4 D=1 S0=0 S1=1", combinational_circuits::DEMUX_1_to_4(1, 0, 1));
    print_v("Decoder_to_DEMUX D=1 S3..S0=0101 (Y5=1)",
            combinational_circuits::Decoder_to_DEMUX(1, 1,0,1,0));

    cout << "SOP_Evaluator in=0101 minterm[10]=1 -> "
         << combinational_circuits::SOP_Evaluator(0b0101, bitset<16>(1ULL << 10)) << "\n";

    print_v("Parity_Generator_4bit A=0111",
            combinational_circuits::Parity_Generator_4bit(0b0111));
    cout << "Parity_Checker_4bit A=0001 received=1 even=1 -> "
         << combinational_circuits::Parity_Checker_4bit(0b0001, 1, 1) << " (no error)\n";
    print_v("74HC280 I=111111111 (odd)", combinational_circuits::_74HC280_(0b111111111));

    print_v("Binary_to_Gray B=1011", combinational_circuits::Binary_to_Gray(0b1011));
    print_v("Gray_to_Binary  G=1110", combinational_circuits::Gray_to_Binary(0b1110));
    print_v("BCD_to_Excess3  BCD=0101 (5+3=8)",
            combinational_circuits::BCD_to_Excess3(0b0101));
    print_v("Excess3_to_BCD  EX3=1000",
            combinational_circuits::Excess3_to_BCD(0b1000));
    print_v("Hex_to_7_Segment hex=F",
            combinational_circuits::Hex_to_7_Segment(0b1111));

    cout << "Overflow_Detect A=0101 B=0011 add=1 (5+3=8 overflow) -> "
         << combinational_circuits::Overflow_Detect(0b0101, 0b0011, 0, 1) << "\n";
    cout << "Zero_Detect A=0000 -> "
         << combinational_circuits::Zero_Detect(0b0000) << "\n";
    cout << "Zero_Detect A=0010 -> "
         << combinational_circuits::Zero_Detect(0b0010) << "\n";

    cout << "\n=== smoke test done ===\n";
    return 0;
}
