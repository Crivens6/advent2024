#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

using std::pair;
using std::regex;
using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input.txt";
const int kTokenPerA = 3;
const int kTokenPerB = 1;
const long kErrorOffset = 10000000000000;
const regex kAPattern("Button A: X\\+[0-9]+, Y\\+[0-9]+");
const regex kBPattern("Button B: X\\+[0-9]+, Y\\+[0-9]+");
const regex kPrizePattern("Prize: X\\=[0-9]+, Y\\=[0-9]+");
const regex kNumPattern("[0-9]+");

int main()
{
    // Open the input file
    std::ifstream input_file(kInputFilePath);
    if (!input_file.is_open())
    {
        std::cerr << "File " << kInputFilePath << " was not found.\n";
        return 1;
    }

    // Iterate through each line and scan for instructions
    vector<pair<int, int>> a_button_list;
    vector<pair<int, int>> b_button_list;
    vector<pair<long, long>> prize_list;
    string input_line;
    while (getline(input_file, input_line))
    {
        // Match to A Pattern
        if (std::regex_match(input_line, kAPattern))
        {
            // Extract the two multipliers from the string
            size_t end_of_first = 0;
            int x = std::stoi(input_line.substr(12), &end_of_first);
            int y = std::stoi(input_line.substr(12 + end_of_first + 4));
            a_button_list.push_back({x, y});
        }
        // Match to B Pattern
        else if (std::regex_match(input_line, kBPattern))
        {
            // Extract the two multipliers from the string
            size_t end_of_first = 0;
            int x = std::stoi(input_line.substr(12), &end_of_first);
            int y = std::stoi(input_line.substr(12 + end_of_first + 4));
            b_button_list.push_back({x, y});
        }
        // Match to Prize Pattern
        else if (std::regex_match(input_line, kPrizePattern))
        {
            // Extract the two multipliers from the string
            size_t end_of_first = 0;
            long x = std::stoi(input_line.substr(9), &end_of_first) + kErrorOffset;
            long y = std::stoi(input_line.substr(9 + end_of_first + 4)) + kErrorOffset;
            prize_list.push_back({x, y});
        }
    }

    long token_tally = 0;
    for (int claw_index = 0; claw_index < prize_list.size(); claw_index++)
    {
        std::cout << "Machine " << (claw_index + 1) << "\n";
        // Find with slope intersept
        long Xg = prize_list[claw_index].first;
        long Yg = prize_list[claw_index].second;
        long Xa = a_button_list[claw_index].first;
        long Ya = a_button_list[claw_index].second;
        long Xb = b_button_list[claw_index].first;
        long Yb = b_button_list[claw_index].second;

        long long dividend_a = Yb * Xg - Xb * Yg;
        long long divisor_a = Xa * Yb - Xb * Ya;
        long long dividend_b = Ya * Xg - Xa * Yg;
        long long divisor_b = Xb * Ya - Xa * Yb;

        if (dividend_a % divisor_a == 0 && dividend_b % divisor_b == 0)
        {
            long a_presses = dividend_a / divisor_a;
            long b_presses = dividend_b / divisor_b;
            token_tally += a_presses * kTokenPerA + b_presses * kTokenPerB;
            std::cout << a_presses << " , " << b_presses << "\n";
        }
    }

    std::cout << "Min Tokens Needed: " << token_tally << "\n";

    input_file.close();
    return 0;
}
