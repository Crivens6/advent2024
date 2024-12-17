#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>

using std::regex;
using std::regex_iterator;
using std::string;

// Constants
const string kInputFilePath = "input.txt";
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
    vector<pair<int, int>> a_press_list;
    vector<pair<int, int>> b_press_list;
    vector<pair<int, int>> prize_list;
    string input_line;
    while (getline(input_file, input_line))
    {
        // Match to A Pattern
        if (std::regex_match(input_line, kAPattern))
        {
            // Extract the two multipliers from the string
            size_t end_of_first_mult = 0;
            int x = std::stoi(input_line.substr(12), &end_of_first_mult);
            int y = std::stoi(input_line.substr(end_of_first_mult + 4));
            a_press_list.push_back({x, y});
        }
        // Match to B Pattern
        else if (std::regex_match(input_line, kBPattern))
        {
            // Extract the two multipliers from the string
            size_t end_of_first_mult = 0;
            int x = std::stoi(input_line.substr(12), &end_of_first_mult);
            int y = std::stoi(input_line.substr(end_of_first_mult + 4));
            b_press_list.push_back({x, y});
        }
        // Match to Prize Pattern
        else if (std::regex_match(input_line, kPrizePattern))
        {
            // Extract the two multipliers from the string
            size_t end_of_first_mult = 0;
            int x = std::stoi(input_line.substr(9), &end_of_first_mult);
            int y = std::stoi(input_line.substr(end_of_first_mult + 4));
            prize_list.push_back({x, y});
        }
    }

    std::cout << "Total Product: " << total_product << "\n";

    input_file.close();
    return 0;
}