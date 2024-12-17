#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input.txt";
const int kTotalBlinks = 25;

int main()
{
    // Open the input file
    std::ifstream input_file(kInputFilePath);
    if (!input_file.is_open())
    {
        std::cerr << "File " << kInputFilePath << " was not found.\n";
        return 1;
    }

    // Iterate through each line and track the file blocks and blank blocks
    vector<long> stone_list;
    string input_line;
    while (getline(input_file, input_line))
    {
        int scan_index = 0;

        // Get the first num
        size_t end_of_num = 0;

        // Loop through each number until the end of the line
        while (scan_index < input_line.size())
        {
            stone_list.push_back(std::stol(input_line.substr(scan_index), &end_of_num));
            // Increment over the number and space
            scan_index += end_of_num + 1;
        }
    }

    // Iterate and move end file blocks into blank file block areas
    for (int blink_count = 0; blink_count < kTotalBlinks; blink_count++)
    {
        for (int target_stone = 0; target_stone < stone_list.size(); target_stone++)
        {
            string stone_string = std::to_string(stone_list[target_stone]);
            // Block is not empty, try next one
            if (stone_list[target_stone] == 0)
            {
                stone_list[target_stone] = 1;
            }
            // Fill Empty Block
            else if (stone_string.size() % 2 == 0)
            {
                int half_length = stone_string.size() / 2;
                stone_list[target_stone] = std::stol(stone_string.substr(0, half_length));
                stone_list.insert(stone_list.begin() + target_stone, std::stol(stone_string.substr(half_length)));
                target_stone++;
            }
            else
            {
                stone_list[target_stone] *= 2024;
            }
        }
    }

    std::cout << "Stone count: " << stone_list.size() << "\n";

    input_file.close();
    return 0;
}