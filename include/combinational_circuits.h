#pragma once
#include <iostream>
#include <vector>
#include <bitset>
#include <stdexcept>   // Required for std::invalid_argument / std::out_of_range thrown in Waveform methods
#include <string>      // Required for std::string returned by *LogicConverter classes
using namespace std;
#ifndef COMBINATIONAL_CIRCUITS_H
#define COMBINATIONAL_CIRCUITS_H


//A simple single-bit wire abstraction used to model a physical wire connection.
class Wire
{
private:
    bool value = false;   //default logic level is LOW (0)

public:
    void set(bool v) { value = v; }    //drive the wire to a new logic level
    bool get() const { return value; } //read the current logic level on the wire
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
    //Note: bitset::to_ulong() interprets the bits as a binary number.
    static void Decimal(const bitset<4>& bcd)
    {
        cout << bcd.to_ulong() << endl;
    }

    //Bus Monitor.
    //Prints the raw 4-bit pattern (MSB first, as bitset::operator<< does).
    static void Bus(const bitset<4>& bus)
    {
        cout << "BUS: " << bus << endl;
    }
};

//Utility class for generating, drawing and analysing digital waveforms.
class Waveform
{
public:
    // Generate a clock signal as a list of timed (timestamp, level) events.
    // freq    : frequency in Hz
    // D_C     : duty cycle in percent (0–100)
    // start_state : true = start HIGH, false = start LOW
    // length  : total simulation time (same unit as 1/freq)
    // Note: callers must guarantee freq > 0 to avoid a division-by-zero.
    static vector<bool> Clock_Signal(double freq, double D_C, bool start_state, int length)
    {
        double period = 1 / freq;
        double T_high = (D_C / 100) * period;
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

    // Draw a simple ASCII waveform; each bit occupies `periods[i]` characters.
    // If `periods` is empty, every bit is drawn as a single character.
    static void Generate_Wave(const vector<bool>& A, vector<int> periods)
    {
        string wave;
        if (periods.empty())
            periods = vector<int>(A.size(), 1);   //default width = 1 char per bit
        for (int i = 0; i < A.size(); i++)
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

    // Print each bit with its start timestamp.
    static void Timed_Wave(const vector<bool>& A, const vector<double>& periods)
    {
        double start = 0;
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

    // Return indices of every rising edge (0→1 transition).
    static vector<int> Rising_Edges(const vector<bool>& sig)
    {
        vector<int> edges;
        for (int i = 1; i < sig.size(); i++)
            if (!sig[i - 1] && sig[i]) edges.push_back(i);
        return edges;
    }

    // Return indices of every falling edge (1→0 transition).
    static vector<int> Falling_Edges(const vector<bool>& sig)
    {
        vector<int> edges;
        for (int i = 1; i < sig.size(); i++)
            if (sig[i - 1] && !sig[i]) edges.push_back(i);
        return edges;
    }

    // Return indices of ALL transitions (rising or falling).
    static vector<int> All_Edges(const vector<bool>& sig)
    {
        vector<int> edges;
        for (int i = 1; i < sig.size(); i++)
            if (sig[i] != sig[i - 1]) edges.push_back(i);
        return edges;
    }

    // Count total number of rising edges (= clock pulse count).
    static int Count_Pulses(const vector<bool>& sig)
    {
        return static_cast<int>(Rising_Edges(sig).size());
    }

    // Measure the HIGH time as a fraction of total samples (0.0 – 1.0).
    static double Duty_Cycle(const vector<bool>& sig)
    {
        if (sig.empty()) return 0.0;
        int highs = 0;
        for (bool b : sig) if (b) highs++;
        return static_cast<double>(highs) / sig.size();
    }

    // Longest uninterrupted HIGH run (in samples).
    static int Max_Pulse_Width(const vector<bool>& sig)
    {
        int maxW = 0, cur = 0;
        for (bool b : sig)
        {
            cur = b ? cur + 1 : 0;
            maxW = max(maxW, cur);
        }
        return maxW;
    }

    // Count transitions per sample – a "glitch density" metric.
    static double Glitch_Density(const vector<bool>& sig)
    {
        if (sig.size() < 2) return 0.0;
        return static_cast<double>(All_Edges(sig).size()) / sig.size();
    }

    // Invert every bit in the signal.
    static vector<bool> Invert(const vector<bool>& sig)
    {
        vector<bool> out(sig.size());
        for (int i = 0; i < sig.size(); i++) out[i] = !sig[i];
        return out;
    }

    // Bitwise AND of two equal-length signals (bus masking, enable logic).
    static vector<bool> AND_Signals(const vector<bool>& A,
                                    const vector<bool>& B)
    {
        if (A.size() != B.size()) throw invalid_argument("Size mismatch");
        vector<bool> out(A.size());
        for (int i = 0; i < A.size(); i++) out[i] = A[i] & B[i];
        return out;
    }

    // Bitwise OR of two equal-length signals.
    static vector<bool> OR_Signals(const vector<bool>& A,
                                   const vector<bool>& B)
    {
        if (A.size() != B.size()) throw invalid_argument("Size mismatch");
        vector<bool> out(A.size());
        for (int i = 0; i < A.size(); i++) out[i] = A[i] | B[i];
        return out;
    }

    // Bitwise XOR of two equal-length signals.
    static vector<bool> XOR_Signals(const vector<bool>& A,
                                    const vector<bool>& B)
    {
        if (A.size() != B.size()) throw invalid_argument("Size mismatch");
        vector<bool> out(A.size());
        for (int i = 0; i < A.size(); i++) out[i] = A[i] ^ B[i];
        return out;
    }

    // Shift the signal right by `n` samples (inserts n LOWs at the front).
    // Models propagation delay through a combinational path.
    static vector<bool> Shift_right(const vector<bool>& sig, int n)
    {
        if (n < 0) throw invalid_argument("Delay must be >= 0");
        if (n == 0)
            return sig;
        if (n >= sig.size())
            return vector<bool>(sig.size(), false);
        vector<bool> out(sig.size(), false);
        for (size_t i = 0; i < sig.size() - n; i++)
            out[i + n] = sig[i];
        return out;
    }

    // Stretch each sample by `factor` (turns 1 sample into `factor` copies).
    // Useful when two signals have different time resolutions
    static vector<bool> Stretch(const vector<bool>& sig, int factor)
    {
        if (factor < 1) throw invalid_argument("Factor must be >= 1");
        vector<bool> out;
        out.reserve(sig.size() * factor);
        for (bool b : sig)
            for (int i = 0; i < factor; i++) out.push_back(b);
        return out;
    }

    // Slice a sub-window [start, end) of a signal.
    static vector<bool> Slice(const vector<bool>& sig, int start, int end)
    {
        if (start < 0 || end > static_cast<int>(sig.size()) || start >= end)
            throw out_of_range("Invalid slice range");
        vector<bool> out(sig.begin() + start, sig.begin() + end);
        return out;
    }

    // Concatenate two signals end-to-end.
    static vector<bool> Concatenate(const vector<bool>& A, const vector<bool>& B)
    {
        vector<bool> out;
        out.resize(A.size() + B.size());
        for (int i = 0; i < A.size(); i++)
            out[i] = A[i];
        for (int i = 0; i < B.size(); i++)
            out[i + A.size()] = B[i];
        return out;
    }
};

//Helper class for 2-input truth tables: builds SOP / POS expressions and
//estimates the gate count needed to realise them.
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

//Helper class for 3-input truth tables (8 rows): same idea as twoLogicConverter.
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

//Helper class for 4-input truth tables (16 rows): same idea as the smaller converters.
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
//Each static method prints the truth table of one fundamental logic gate.
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
//NAND is functionally complete, so every other gate can be built from it.
class Gates
{
public:
    //Primitive NAND gate — the only "real" gate; the rest are derived from it.
    static bool NAND(bool A, bool B)
    {
        return !(A & B);
    }

    //NOT(A) = NAND(A, A)
    static bool NOT(bool A)
    {
        return NAND(A, A);
    }

    //OR(A,B) = NAND(NOT(A), NOT(B))
    static bool OR(bool A, bool B)
    {
        return NAND(NAND(A, A), NAND(B, B));
    }

    //AND(A,B) = NOT(NAND(A,B))
    static bool AND(bool A, bool B)
    {
        return NAND(NAND(A, B), NAND(A, B));
    }

    //NOR(A,B) = NOT(OR(A,B))
    static bool NOR(bool A, bool B)
    {
        return NAND(NAND(NAND(A, A), NAND(B, B)),
                    NAND(NAND(A, A), NAND(B, B)));
    }

    //XOR(A,B) using the classic 4-NAND construction.
    static bool XOR(bool A, bool B)
    {
        bool x = NAND(A, B);
        bool y = NAND(A, x);
        bool z = NAND(B, x);
        return NAND(y, z);
    }

    //XNOR(A,B) = NOT(XOR(A,B))
    static bool XNOR(bool A, bool B)
    {
        bool x = NAND(A, B);
        bool y = NAND(A, x);
        bool z = NAND(B, x);
        bool out_invert = NAND(y, z);
        return NAND(out_invert, out_invert);
    }

    //Tri-state buffer enum: LOW / HIGH are driven levels, HIGH_Z is high-impedance.
    enum TriState
    {
        LOW = 0,
        HIGH = 1,
        HIGH_Z
    };

    //Tri-state buffer: passes A when EN=1, otherwise goes high-impedance.
    static TriState Tri_state(bool A, bool EN)
    {
        if (!EN)
            return HIGH_Z;

        return A ? HIGH : LOW;
    }
};

//Behavioural model of the TTL 74181 4-bit ALU slice.
//Mode pin M=1 → logic functions, M=0 → arithmetic functions selected by S[3..0].
//C_in behaves as the active-low carry input on the real chip.
class ALU_74181
{
public:
    static vector<bool> _74HC181_(bitset<4> A, bitset<4> B,
                                  bitset<4> S, bool M, bool C_in)
    {
        bool F[4] = {false, false, false, false};
        bool C_out = false;

        if (M)
        {
            // ---------------- LOGIC MODE (M = 1) ----------------
            // Each output bit is an independent boolean function of
            // Ai, Bi selected by S3..S0 (per-bit, no carry chain).
            for (int i = 0; i < 4; i++)
                F[i] = Logic_Function(A[i], B[i], S);

            C_out = false; // carry has no meaning in logic mode
        }
        else
        {
            // ------------- ARITHMETIC MODE (M = 0) ---------------
            // Build the per-bit "pseudo-sum" inputs (Xi, Yi) defined
            // by the 74181 function table, then ripple-add them with
            // the real carry chain — same structure as your
            // Full_Adder, just with select-dependent operands.
            bool carry = C_in;
            for (int i = 0; i < 4; i++)
            {
                bool X, Y;
                Arithmetic_Operands(A[i], B[i], S, X, Y);

                bool sum   = Gates::XOR(Gates::XOR(X, Y), carry);
                bool cnext = Gates::OR(Gates::AND(X, Y),
                                       Gates::AND(Gates::XOR(X, Y), carry));

                F[i] = sum;
                carry = cnext;
            }
            C_out = carry;
        }

        // A = B comparator output.
        // On the real 74181, the A=B pin is only meaningful when the ALU is
        // configured for subtraction (S=0110, M=0, Cin=1): in that mode it
        // goes HIGH exactly when F == 0000, i.e. A - B == 0.
        // We reproduce that: A_eq_B is HIGH iff every result bit is LOW.
        bool A_eq_B = Gates::NOT(Gates::OR(Gates::OR(F[0], F[1]), Gates::OR(F[2], F[3])));

        return { F[0], F[1], F[2], F[3], C_out, A_eq_B };
    }

private:
    // ---- LOGIC MODE function table (M=1) ----
    // S3 S2 S1 S0 | F (per bit, in terms of Ai, Bi)
    //  0  0  0  0 | NOT Ai
    //  0  0  0  1 | NOT(Ai OR Bi)        (NOR)
    //  0  0  1  0 | (NOT Ai) AND Bi
    //  0  0  1  1 | 0
    //  0  1  0  0 | NOT(Ai AND Bi)       (NAND)
    //  0  1  0  1 | NOT Bi
    //  0  1  1  0 | Ai XOR Bi
    //  0  1  1  1 | Ai AND (NOT Bi)
    //  1  0  0  0 | (NOT Ai) OR Bi
    //  1  0  0  1 | Ai XNOR Bi
    //  1  0  1  0 | Bi
    //  1  0  1  1 | Ai AND Bi
    //  1  1  0  0 | 1
    //  1  1  0  1 | Ai OR (NOT Bi)
    //  1  1  1  0 | Ai OR Bi
    //  1  1  1  1 | Ai
    static bool Logic_Function(bool Ai, bool Bi, const bitset<4>& S)
    {
        int sel = (S[3] << 3) | (S[2] << 2) | (S[1] << 1) | S[0];

        switch (sel)
        {
            case 0:  return Gates::NOT(Ai);
            case 1:  return Gates::NOT(Gates::OR(Ai, Bi));
            case 2:  return Gates::AND(Gates::NOT(Ai), Bi);
            case 3:  return false;
            case 4:  return Gates::NOT(Gates::AND(Ai, Bi));
            case 5:  return Gates::NOT(Bi);
            case 6:  return Gates::XOR(Ai, Bi);
            case 7:  return Gates::AND(Ai, Gates::NOT(Bi));
            case 8:  return Gates::OR(Gates::NOT(Ai), Bi);
            case 9:  return Gates::XNOR(Ai, Bi);
            case 10: return Bi;
            case 11: return Gates::AND(Ai, Bi);
            case 12: return true;
            case 13: return Gates::OR(Ai, Gates::NOT(Bi));
            case 14: return Gates::OR(Ai, Bi);
            case 15: return Ai;
        }
        return false; // unreachable
    }

    // ---- ARITHMETIC MODE operand table (M=0) ----
    // The 74181 computes each arithmetic function by feeding a
    // select-dependent pair (Xi, Yi) into a full-adder carry chain.
    // This table reproduces the datasheet's active-HIGH arithmetic
    // function list at C_in = 0 (carry chain handles the +1/+Cin part):
    //
    //  S3210 | Function (Cin=0)         | Xi          Yi
    //   0000 | F = A                    | Ai          0
    //   0001 | F = A OR B               | Ai OR Bi    0
    //   0010 | F = A OR NOT B           | Ai OR ~Bi   0
    //   0011 | F = minus 1 (all 1s)     | 1           0   (with carry forced)
    //   0100 | F = A + (A AND NOT B)    | Ai          Ai AND ~Bi
    //   0101 | F = (A OR B)+(A AND ~B)  | Ai OR Bi    Ai AND ~Bi
    //   0110 | F = A minus B minus 1    | Ai          ~Bi   (subtract form)
    //   0111 | F = (A AND NOT B) - 1    | Ai AND ~Bi  1     (subtract form)
    //   1000 | F = A + (A AND B)        | Ai          Ai AND Bi
    //   1001 | F = A + B                | Ai          Bi
    //   1010 | F = (A OR ~B)+(A AND B)  | Ai OR ~Bi   Ai AND Bi
    //   1011 | F = A AND B  minus 1     | Ai AND Bi   1   (subtract form)
    //   1100 | F = A + A                | Ai          Ai
    //   1101 | F = (A OR B) + A         | Ai OR Bi    Ai
    //   1110 | F = (A OR ~B) + A        | Ai OR ~Bi   Ai
    //   1111 | F = A minus 1            | Ai          1  (subtract form)
    //
    // To keep this implementation tractable with a simple ripple chain,
    // we directly express each line's X/Y pair (subtract lines use the
    // ones'-complement trick: A - B - 1 == A + (~B) + Cin, so Y becomes
    // the complement of B and the chain handles the rest).
    static void Arithmetic_Operands(bool Ai, bool Bi, const bitset<4>& S,
                                    bool& X, bool& Y)
    {
        int sel = (S[3] << 3) | (S[2] << 2) | (S[1] << 1) | S[0];

        switch (sel)
        {
            case 0:  X = Ai;                              Y = false;            break; // A
            case 1:  X = Gates::OR(Ai, Bi);               Y = false;            break; // A+B
            case 2:  X = Gates::OR(Ai, Gates::NOT(Bi));   Y = false;            break; // A+~B
            case 3:  X = true;                            Y = false;            break; // minus 1
            case 4:  X = Ai;                               Y = Gates::AND(Ai, Gates::NOT(Bi)); break; // A+A.~B
            case 5:  X = Gates::OR(Ai, Bi);                Y = Gates::AND(Ai, Gates::NOT(Bi)); break;
            case 6:  X = Ai;                               Y = Gates::NOT(Bi);  break; // A - B - 1
            case 7:  X = Gates::AND(Ai, Gates::NOT(Bi));   Y = true;            break; // A.~B - 1
            case 8:  X = Ai;                               Y = Gates::AND(Ai, Bi); break; // A+A.B
            case 9:  X = Ai;                               Y = Bi;              break; // A+B
            case 10: X = Gates::OR(Ai, Gates::NOT(Bi));    Y = Gates::AND(Ai, Bi); break;
            case 11: X = Gates::AND(Ai, Bi);               Y = true;            break; // A.B - 1
            case 12: X = Ai;                               Y = Ai;              break; // A+A
            case 13: X = Gates::OR(Ai, Bi);                Y = Ai;              break;
            case 14: X = Gates::OR(Ai, Gates::NOT(Bi));    Y = Ai;              break;
            case 15: X = Ai;                               Y = true;            break; // A - 1
            default: X = false; Y = false; break; // unreachable
        }
    }
};

//Top-level facade exposing every modelled combinational IC / circuit as a
//static member function. All methods are pure (no internal state).
class combinational_circuits
{
public:
    //Returns {Sum, Carry}.
    static vector<bool> Half_Adder(bool A, bool B);
    //Returns {Sum, Carry}.
    static vector<bool> Full_Adder(bool A, bool B, bool C);
    //Returns {Diff, Borrow}.
    static vector<bool> Half_Subtractor(bool A, bool B);
    //Returns {Diff, Borrow}.
    static vector<bool> Full_Subtractor(bool A, bool B, bool B_in);
    //74HC283 4-bit adder/subtractor. stat=0 → add, stat=1 → subtract.
    //Returns {C_out, S3, S2, S1, S0} (5 bits).
    static vector<bool> _74HC283_(bitset<4> A, bitset<4> B, bool C_in, bool stat);
    //Returns {C_out, S3, S2, S1, S0}.
    static vector<bool> Carry_Lookahead_Adder_4bit(bitset<4> A, bitset<4> B, bool C_in);
    //Returns {M0, M1, M2, M3} (4-bit product).
    static vector<bool> Multiplier_2x2(bitset<2> A, bitset<2> B);
    static vector<bool> Multiplier_4x4(bitset<4> A, bitset<4> B); //
    //Returns true iff A == B (4-bit equality).
    static bool Identity_Comparator(bitset<4> A, bitset<4> B);
    //Returns {A>B, A==B, A<B} with cascade inputs (default standalone = eq:1).
    static vector<bool> _74HC85_(bitset<4> A, bitset<4> B, bool eq = 0b1, bool larger = 0b0, bool smaller = 0b0);
    //4-to-1 MUX. Note: this implementation treats S0 as the MSB of the 2-bit
    //select code (S0 S1): 00→A[0], 01→A[1], 10→A[2], 11→A[3].
    static bool MUX_4_to_1(bitset<4> A, bool S0, bool S1);
    //74HC151 8-to-1 MUX. Returns {Y, Y_bar} (non-inverted and inverted outputs).
    //Select convention: S0 is MSB of the 3-bit select code (S0 S1 S2).
    static vector<bool> _74HC151_(bitset<8> A, bool S0, bool S1, bool S2);
    //2-to-4 decoder (active-high outputs). Returns {Y0,Y1,Y2,Y3} for inputs A B (A=MSB).
    static vector<bool> Decoder_2_to_4(bool A, bool B);
    //74HC154 4-to-16 decoder (active-low outputs) with active-high enable EN.
    static vector<bool> _74HC154_(bool A0, bool A1, bool A2, bool A3, bool EN);
    //Implements a 4-to-1 MUX using an internal 2-to-4 decoder.
    static bool Decoder_to_MUX(bool S1, bool S0, bool I0, bool I1, bool I2, bool I3);
    //74HC42 BCD-to-decimal decoder (active-low outputs).
    static vector<bool> _74HC42_(bool A0, bool A1, bool A2, bool A3);
    //BCD-to-7-segment decoder + ASCII rendering. Returns {a,b,c,d,e,f,g,dp}.
    static vector<bool> BCD_to_7_Segment(bool A0, bool A1, bool A2, bool A3, bool decimal);
    //Simple 4-to-2 encoder. Returns {Q1, Q0}.
    static vector<bool> Encoder_4_to_2(bool D0, bool D1, bool D2, bool D3);
    //Decimal-to-BCD encoder (10 inputs). Returns {A3,A2,A1,A0}.
    static vector<bool> Decimal_to_BCD_Encoder(bool D0, bool D1, bool D2, bool D3, bool D4, bool D5, bool D6, bool D7,
                                               bool D8, bool D9);
    //74HC148 8-to-3 priority encoder. Returns {Y2, Y1, Y0}.
    static vector<bool> _74HC148_(bitset<8> D);
    //74HC147 decimal-to-BCD priority encoder (active-low I/O).
    static vector<bool> _74HC147_(bool D0, bool D1, bool D2, bool D3, bool D4, bool D5, bool D6, bool D7, bool D8,
                                  bool D9);
    //1-to-4 demultiplexer. Returns {Y0,Y1,Y2,Y3}.
    static vector<bool> DEMUX_1_to_4(bool D, bool S0, bool S1);
    //1-to-16 demux built from a 4-to-16 decoder.
    static vector<bool> Decoder_to_DEMUX(bool D, bool S0, bool S1, bool S2, bool S3);
    //Evaluate an SOP expression given a 4-bit input and a 16-bit minterm mask.
    static bool SOP_Evaluator(bitset<4> current_inputs, bitset<16> active_minterms);
    //4-bit parity generator. Returns {even_parity, odd_parity}.
    static vector<bool> Parity_Generator_4bit(bitset<4> A);
    //4-bit parity checker. `even`=true → check even-parity convention. Returns true on error.
    static bool Parity_Checker_4bit(bitset<4> A, bool received_parity, bool even);
    //74HC280 9-bit parity generator/checker. Returns {even, odd}.
    static vector<bool> _74HC280_(bitset<9> I);
    //4-bit Binary→Gray converter. Returns {G0,G1,G2,G3} (LSB-first).
    static vector<bool> Binary_to_Gray(bitset<4> B);
    //4-bit Gray→Binary converter. Returns {B0,B1,B2,B3} (LSB-first).
    static vector<bool> Gray_to_Binary(bitset<4> G);
    //BCD→Excess-3 code converter. Returns {E0,E1,E2,E3} (LSB-first).
    static vector<bool> BCD_to_Excess3(bitset<4> BCD);
    //Excess-3→BCD code converter. Returns {B0,B1,B2,B3} (LSB-first).
    static vector<bool> Excess3_to_BCD(bitset<4> EX3);
    //Hex→7-segment decoder (digits 0-9, letters A-F). Returns {a,b,c,d,e,f,g,dp}.
    static vector<bool> Hex_to_7_Segment(bitset<4> hex);
    //Detect 4-bit signed overflow. `addition`=true → check add, false → check subtract.
    static bool Overflow_Detect(bitset<4> A, bitset<4> B, bool C_in,bool addition);
    //True iff all 4 bits of A are zero.
    static bool Zero_Detect(bitset<4> A);
};


#endif //COMBINATIONAL_CIRCUITS_H
