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
const regex kAPattern("Button A: X\\+[0-9]+, Y\\+[0-9]+");
const regex kBPattern("Button B: X\\+[0-9]+, Y\\+[0-9]+");
const regex kPrizePattern("Prize: X\\=[0-9]+, Y\\=[0-9]+");
const regex kNumPattern("[0-9]+");

class Button
{
public:
    int x;
    int y;
    Button(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    int MaxPressesInBounds(pair<int, int> prize_coor)
    {
        int x_moves = prize_coor.first / x;
        int y_moves = prize_coor.second / y;
        return std::min(x_moves, y_moves);
    }
    int MinTokensToWin(pair<int, int> prize_coor, int max_presses, int my_tokens, Button other, int other_tokens)
    {
        if (max_presses > 100)
        {
            max_presses = 100;
        }
        int min_tokens_to_win = -1;
        for (int this_presses = 0; this_presses <= max_presses; this_presses++)
        {
            int x_remaining = prize_coor.first - (x * this_presses);
            int y_remaining = prize_coor.second - (y * this_presses);
            int other_presses = other.PressesToTarget(x_remaining, y_remaining);
            if (other_presses == -1)
            {
                continue;
            }
            else
            {
                int token_cost = this_presses * my_tokens + other_presses * other_tokens;
                if (token_cost < min_tokens_to_win || min_tokens_to_win == -1)
                {
                    min_tokens_to_win = token_cost;
                }
            }
        }
        return std::max(0, min_tokens_to_win);
    }
    int PressesToTarget(int prize_x, int prize_y)
    {
        if (prize_x % x == 0 && prize_y % y == 0)
        {
            if (prize_x / x == prize_y / y)
            {
                if (prize_x / x <= 100)
                {
                    return prize_x / x;
                }
            }
        }
        return -1;
    }
};

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
    vector<Button> a_button_list;
    vector<Button> b_button_list;
    vector<pair<int, int>> prize_list;
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
            a_button_list.push_back(Button(x, y));
        }
        // Match to B Pattern
        else if (std::regex_match(input_line, kBPattern))
        {
            // Extract the two multipliers from the string
            size_t end_of_first = 0;
            int x = std::stoi(input_line.substr(12), &end_of_first);
            int y = std::stoi(input_line.substr(12 + end_of_first + 4));
            b_button_list.push_back(Button(x, y));
        }
        // Match to Prize Pattern
        else if (std::regex_match(input_line, kPrizePattern))
        {
            // Extract the two multipliers from the string
            size_t end_of_first = 0;
            int x = std::stoi(input_line.substr(9), &end_of_first);
            int y = std::stoi(input_line.substr(9 + end_of_first + 4));
            prize_list.push_back({x, y});
        }
    }

    int token_tally = 0;
    for (int claw_index = 0; claw_index < prize_list.size(); claw_index++)
    {
        // Find which button gets out of coords fastest
        int a_press_count = a_button_list[claw_index].MaxPressesInBounds(prize_list[claw_index]);
        int b_press_count = b_button_list[claw_index].MaxPressesInBounds(prize_list[claw_index]);

        if (a_press_count < b_press_count)
        {
            token_tally += a_button_list[claw_index].MinTokensToWin(prize_list[claw_index], a_press_count, kTokenPerA, b_button_list[claw_index], kTokenPerB);
        }
        else
        {
            token_tally += b_button_list[claw_index].MinTokensToWin(prize_list[claw_index], b_press_count, kTokenPerB, a_button_list[claw_index], kTokenPerA);
        }
    }

    std::cout << "Min Tokens Needed: " << token_tally << "\n";

    input_file.close();
    return 0;
}
