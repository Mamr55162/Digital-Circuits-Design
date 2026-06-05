#pragma once
#include <iostream>
#include <vector>
#include <bitset>
using namespace std;
#ifndef COMBINATIONAL_CIRCUITS_H
#define COMBINATIONAL_CIRCUITS_H


class Wire
{
private:
    bool value = false;

public:
    void set(bool v) { value = v; }
    bool get() const { return value; }
};

//Multiple Indicators class.
class Indicator
{
public:
    //Use LED as indicator for HIGH (1) and LOW (0) outputs.
    static void LED(bool val)
    {
        if (val == true)
            cout << "LED is ON\n";
        else
            cout << "LED is OFF\n";
    }

    //Print the binary value(s).
    static void Binary(bool val)
    {
        cout << val;
    }

    static void BinaryVector(const vector<bool>& bits)
    {
        for (bool b : bits)
            cout << b;

        cout << '\n';
    }

    //Detect whether a wire is HIGH (1) or LOW (0).
    static void Probe(bool val)
    {
        cout << (val ? "HIGH" : "LOW") << endl;
    }

    //Print decimal equivalent of BCD.
    static void Decimal(const bitset<4>& bcd)
    {
        cout << bcd.to_ulong() << endl;
    }

    //Bus Monitor.
    static void Bus(const bitset<4>& bus)
    {
        cout << "BUS: " << bus << endl;
    }
};

class Waveform
{
public:
    static vector<bool> Clock_Signal(double freq, double D_C, bool start_state, int length)
    {
        double period = 1 / freq;
        double T_high = (D_C/100) * period;
        double T_low = period - T_high;
        double start = 0;
        vector<bool> res;
        while (start < length)
        {
            if (start_state)
            {
                cout << "<" << start << ", HIGH>\n";
                start += T_high;
                start_state = false;
                res.push_back(1);
            }
            else
            {
                cout << "<" << start << ", LOW>\n";
                start += T_low;
                start_state = true;
                res.push_back(0);
            }
        }
        return res;
    }
    static void Generate_Wave(const vector<bool>& A,const vector<int>& periods)
    {
        string wave;
        for (int i = 0;  i < A.size(); i++)
        {
            if (!A[i])
            {
                for (int j = 0; j < periods[i]; j++)
                    wave += '_';
            }

            else
            {
                for (int j = 0; j < periods[i]; j++)
                    wave += '-';
            }

        }
        cout << wave << endl;
    }
    static void Timed_Wave(const vector<bool>& A,const vector<int>& periods)
    {
        int start = 0;
        if (A.size() != periods.size())
        {
            cout << "Size mismatch!\n";
            return;
        }
        for (int i = 0; i < A.size(); i++)
        {
            if (A[i])
                cout << "<" << start << ", HIGH>\n";

            else
                cout << "<" << start << ", LOW>\n";
            start += periods[i];
        }
    }
};

class twoLogicConverter
{
public:
    //Construct and show Truth Table from given outputs.
    static void TruthTable(bool out1, bool out2, bool out3, bool out4)
    {
        cout << "A  B | F\n";
        cout << "0  0 | " << out1 << "\n";
        cout << "0  1 | " << out2 << "\n";
        cout << "1  0 | " << out3 << "\n";
        cout << "1  1 | " << out4 << "\n";
    }

    //Derive the SOP Expression from truth table.
    static string SOP_Expression(bool out1, bool out2, bool out3, bool out4)
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
    static void NANDConstruction(bool out1, bool out2, bool out3, bool out4)
    {
        int terms = 0;

        if (out1) terms++;
        if (out2) terms++;
        if (out3) terms++;
        if (out4) terms++;

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
};

class threeLogicConverter
{
public:
    static void TruthTable(bool o1, bool o2, bool o3, bool o4,
                           bool o5, bool o6, bool o7, bool o8)
    {
        cout << "A  B  C | F\n";
        cout << "0  0  0 | " << o1 << "\n";
        cout << "0  0  1 | " << o2 << "\n";
        cout << "0  1  0 | " << o3 << "\n";
        cout << "0  1  1 | " << o4 << "\n";
        cout << "1  0  0 | " << o5 << "\n";
        cout << "1  0  1 | " << o6 << "\n";
        cout << "1  1  0 | " << o7 << "\n";
        cout << "1  1  1 | " << o8 << "\n";
    }

    static string SOP_Expression(bool o1, bool o2, bool o3, bool o4,
                                 bool o5, bool o6, bool o7, bool o8)
    {
        string exp;
        const string terms[8] = {
            "A'B'C'", "A'B'C", "A'BC'", "A'BC",
            "AB'C'", "AB'C", "ABC'", "ABC"
        };
        const bool output[8] = {o1, o2, o3, o4, o5, o6, o7, o8};
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

    static string POS_Expression(bool o1, bool o2, bool o3, bool o4,
                                 bool o5, bool o6, bool o7, bool o8)
    {
        string exp;
        const string terms[8] = {
            "(A+B+C)", "(A+B+C')",
            "(A+B'+C)", "(A+B'+C')",
            "(A'+B+C)", "(A'+B+C')",
            "(A'+B'+C)", "(A'+B'+C')"
        };
        const bool output[8] = {o1, o2, o3, o4, o5, o6, o7, o8};
        for (int i = 0; i < 8; i++)
        {
            if (!output[i])
                exp += terms[i];
        }
        return exp.empty() ? "1" : exp;
    }

    static void SOP_GateConstruction(bool o1, bool o2, bool o3, bool o4,
                                     bool o5, bool o6, bool o7, bool o8)
    {
        bool output[8] = {o1, o2, o3, o4, o5, o6, o7, o8};

        int AND_count = 0;
        int OR_count = 0;

        bool needA = false;
        bool needB = false;
        bool needC = false;

        // Check active minterms
        for (int i = 0; i < 8; i++)
        {
            if (output[i])
            {
                AND_count++;

                // Detect complemented literals
                if (i < 4) needA = true;

                if (i == 0 || i == 1 || i == 4 || i == 5)
                    needB = true;

                if (i == 0 || i == 2 || i == 4 || i == 6)
                    needC = true;
            }
        }

        int NOT_count = needA + needB + needC;

        if (AND_count > 1)
            OR_count = AND_count - 1;

        cout << "SOP Construction : "
            << NOT_count << " NOT Gates, "
            << AND_count << " AND Gates, "
            << OR_count << " OR Gates\n";
    }

    static void POS_GateConstruction(bool o1, bool o2, bool o3, bool o4,
                                     bool o5, bool o6, bool o7, bool o8)
    {
        bool output[8] = {o1, o2, o3, o4, o5, o6, o7, o8};

        int OR_count = 0;
        int AND_count = 0;

        bool needA = false;
        bool needB = false;
        bool needC = false;

        for (int i = 0; i < 8; i++)
        {
            if (!output[i])
            {
                OR_count++;

                // Detect complemented literals in POS

                if (i >= 4) needA = true;

                if (i == 2 || i == 3 || i == 6 || i == 7)
                    needB = true;

                if (i == 1 || i == 3 || i == 5 || i == 7)
                    needC = true;
            }
        }

        int NOT_count = needA + needB + needC;

        if (OR_count > 1)
            AND_count = OR_count - 1;

        cout << "POS Construction : "
            << NOT_count << " NOT Gates, "
            << AND_count << " AND Gates, "
            << OR_count << " OR Gates\n";
    }
};

class fourLogicConverter
{
public:
    static void TruthTable(
        bool o1, bool o2, bool o3, bool o4,
        bool o5, bool o6, bool o7, bool o8,
        bool o9, bool o10, bool o11, bool o12,
        bool o13, bool o14, bool o15, bool o16)
    {
        cout << "A  B  C  D | F\n";
        const bool outs[16] = {
            o1, o2, o3, o4, o5, o6, o7, o8,
            o9, o10, o11, o12, o13, o14, o15, o16
        };
        for (int i = 0; i < 16; ++i)
            cout << ((i >> 3) & 1) << "  " << ((i >> 2) & 1) << "  "
                << ((i >> 1) & 1) << "  " << (i & 1) << "  | " << outs[i] << "\n";
    }

    static string SOP_Expression(
        bool o1, bool o2, bool o3, bool o4,
        bool o5, bool o6, bool o7, bool o8,
        bool o9, bool o10, bool o11, bool o12,
        bool o13, bool o14, bool o15, bool o16)
    {
        string exp;
        const string terms[16] = {
            "A'B'C'D'", "A'B'C'D", "A'B'CD'", "A'B'CD",
            "A'BC'D'", "A'BC'D", "A'BCD'", "A'BCD",
            "AB'C'D'", "AB'C'D", "AB'CD'", "AB'CD",
            "ABC'D'", "ABC'D", "ABCD'", "ABCD"
        };
        const bool output[16] = {
            o1, o2, o3, o4, o5, o6, o7, o8,
            o9, o10, o11, o12, o13, o14, o15, o16
        };
        for (int i = 0; i < 16; i++)
        {
            if (output[i])
            {
                if (!exp.empty())
                    exp += " + ";
                exp += terms[i];
            }
        }
        return exp.empty() ? "0" : exp;
    }

    static string POS_Expression(
        bool o1, bool o2, bool o3, bool o4,
        bool o5, bool o6, bool o7, bool o8,
        bool o9, bool o10, bool o11, bool o12,
        bool o13, bool o14, bool o15, bool o16)
    {
        string exp;
        const string terms[16] = {
            "(A+B+C+D)", "(A+B+C+D')",
            "(A+B+C'+D)", "(A+B+C'+D')",
            "(A+B'+C+D)", "(A+B'+C+D')",
            "(A+B'+C'+D)", "(A+B'+C'+D')",
            "(A'+B+C+D)", "(A'+B+C+D')",
            "(A'+B+C'+D)", "(A'+B+C'+D')",
            "(A'+B'+C+D)", "(A'+B'+C+D')",
            "(A'+B'+C'+D)", "(A'+B'+C'+D')"
        };
        const bool output[16] = {
            o1, o2, o3, o4, o5, o6, o7, o8,
            o9, o10, o11, o12, o13, o14, o15, o16
        };
        for (int i = 0; i < 16; i++)
        {
            if (!output[i])
                exp += terms[i];
        }
        return exp.empty() ? "1" : exp;
    }

    static void SOP_GateConstruction(bool o1, bool o2, bool o3, bool o4,
                                     bool o5, bool o6, bool o7, bool o8,
                                     bool o9, bool o10, bool o11, bool o12,
                                     bool o13, bool o14, bool o15, bool o16)
    {
        bool output[16] =
        {
            o1, o2, o3, o4,
            o5, o6, o7, o8,
            o9, o10, o11, o12,
            o13, o14, o15, o16
        };

        int AND_count = 0;

        bool needA = false;
        bool needB = false;
        bool needC = false;
        bool needD = false;

        for (int i = 0; i < 16; i++)
        {
            if (output[i])
            {
                AND_count++;

                // A = 0
                if (i < 8)
                    needA = true;

                // B = 0
                if (i < 4 || (i >= 8 && i < 12))
                    needB = true;

                // C = 0
                if ((i % 8) < 2 || ((i % 8) >= 4 && (i % 8) < 6))
                    needC = true;

                // D = 0
                if (i % 2 == 0)
                    needD = true;
            }
        }

        int NOT_count = needA + needB + needC + needD;

        // Multi-input OR gate
        int OR_count = (AND_count > 1) ? AND_count - 1 : 0;

        cout << "SOP Construction : "
            << NOT_count << " NOT Gates, "
            << AND_count << " AND Gates, "
            << OR_count << " OR Gates\n";
    }

    static void POS_GateConstruction(bool o1, bool o2, bool o3, bool o4,
                                     bool o5, bool o6, bool o7, bool o8,
                                     bool o9, bool o10, bool o11, bool o12,
                                     bool o13, bool o14, bool o15, bool o16)
    {
        bool output[16] =
        {
            o1, o2, o3, o4,
            o5, o6, o7, o8,
            o9, o10, o11, o12,
            o13, o14, o15, o16
        };

        int OR_count = 0;

        bool needA = false;
        bool needB = false;
        bool needC = false;
        bool needD = false;

        for (int i = 0; i < 16; i++)
        {
            if (!output[i])
            {
                OR_count++;

                // A'
                if (i >= 8)
                    needA = true;

                // B'
                if ((i >= 4 && i < 8) || i >= 12)
                    needB = true;

                // C'
                if ((i % 8) >= 2 && (i % 8) < 4 ||
                    (i % 8) >= 6)
                    needC = true;

                // D'
                if (i % 2 == 1)
                    needD = true;
            }
        }

        int NOT_count = needA + needB + needC + needD;

        // Multi-input AND gate
        int AND_count = (OR_count > 1) ? OR_count - 1 : 0;

        cout << "POS Construction : "
            << NOT_count << " NOT Gates, "
            << AND_count << " AND Gates, "
            << OR_count << " OR Gates\n";
    }
};

//Print two-input gates truth tables.
class TruthTable
{
public:
    static void ANDTruthTable()
    {
        cout << "A  B | F\n";
        cout << "0  0 | 0\n";
        cout << "0  1 | 0\n";
        cout << "1  0 | 0\n";
        cout << "1  1 | 1\n";
    }

    static void ORTruthTable()
    {
        cout << "A  B | F\n";
        cout << "0  0 | 0\n";
        cout << "0  1 | 1\n";
        cout << "1  0 | 1\n";
        cout << "1  1 | 1\n";
    }

    static void NANDTruthTable()
    {
        cout << "A  B | F\n";
        cout << "0  0 | 1\n";
        cout << "0  1 | 1\n";
        cout << "1  0 | 1\n";
        cout << "1  1 | 0\n";
    }

    static void NORTruthTable()
    {
        cout << "A  B | F\n";
        cout << "0  0 | 1\n";
        cout << "0  1 | 0\n";
        cout << "1  0 | 0\n";
        cout << "1  1 | 0\n";
    }

    static void XORTruthTable()
    {
        cout << "A  B | F\n";
        cout << "0  0 | 0\n";
        cout << "0  1 | 1\n";
        cout << "1  0 | 1\n";
        cout << "1  1 | 0\n";
    }

    static void XNORTruthTable()
    {
        cout << "A  B | F\n";
        cout << "0  0 | 1\n";
        cout << "0  1 | 0\n";
        cout << "1  0 | 0\n";
        cout << "1  1 | 1\n";
    }

    static void NOTTruthTable()
    {
        cout << "A | F\n";
        cout << "0 | 1\n";
        cout << "1 | 0\n";
    }

    static void Tri_stateTruthTable()
    {
        cout << "EN  A | F\n";
        cout << "0  0 | Hi-Z\n";
        cout << "0  1 | Hi-z\n";
        cout << "1  0 | 0\n";
        cout << "1  1 | 1\n";
    }
};


//Implement NOT, OR, AND, NOR, XOR, XNOR, Tri-state gates using NAND gate only.
class Gates
{
public:
    static bool NAND(bool A, bool B)
    {
        return !(A & B);
    }

    static bool NOT(bool A)
    {
        return NAND(A, A);
    }

    static bool OR(bool A, bool B)
    {
        return NAND(NAND(A, A), NAND(B, B));
    }

    static bool AND(bool A, bool B)
    {
        return NAND(NAND(A, B), NAND(A, B));
    }

    static bool NOR(bool A, bool B)
    {
        return NAND(NAND(NAND(A, A), NAND(B, B)),
                    NAND(NAND(A, A), NAND(B, B)));
    }

    static bool XOR(bool A, bool B)
    {
        bool x = NAND(A, B);
        bool y = NAND(A, x);
        bool z = NAND(B, x);
        return NAND(y, z);
    }

    static bool XNOR(bool A, bool B)
    {
        bool x = NAND(A, B);
        bool y = NAND(A, x);
        bool z = NAND(B, x);
        bool out_invert = NAND(y, z);
        return NAND(out_invert, out_invert);
    }

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
};


class combinational_circuits
{
public:
    static vector<bool> Half_Adder(bool A, bool B);
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
    static vector<bool> Encoder_4_to_2(bool D0, bool D1, bool D2, bool D3);
    static vector<bool> Decimal_to_BCD_Encoder(bool D0, bool D1, bool D2, bool D3, bool D4, bool D5, bool D6, bool D7, bool D8 ,bool D9);
    static vector<bool> _74HC147_(bool D0, bool D1, bool D2, bool D3, bool D4, bool D5, bool D6, bool D7, bool D8 ,bool D9);
    static vector<bool> DEMUX_1_to_4();
};


#endif //COMBINATIONAL_CIRCUITS_H