#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::pair;
using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input.txt";
const int kBlank = -1;

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
    vector<pair<int, int>> file_blocks;
    string input_line;
    bool is_file = true;
    int file_id = 0;
    while (getline(input_file, input_line))
    {
        // Go through each letter in the line and add it to the vecor row
        for (char input_char : input_line)
        {
            if (input_char != '0')
            {
                if (is_file)
                {
                    file_blocks.push_back({file_id, input_char - '0'});
                    file_id++;
                }
                else
                {
                    file_blocks.push_back({kBlank, input_char - '0'});
                }
            }
            is_file = !is_file;
        }
    }

    // Iterate and move end file blocks into blank file block areas
    for (int block_index = file_blocks.size() - 1; block_index > 0; block_index--)
    {
        // Block is empty, try next one
        if (file_blocks[block_index].first == kBlank)
        {
            continue;
        }
        int end_size = file_blocks[block_index].second;
        for (int target_index = 0; target_index < block_index; target_index++)
        {
            // Block is not empty, try next one
            if (file_blocks[target_index].first != kBlank || file_blocks[target_index].second < end_size)
            {
                continue;
            }
            // Fill Empty Block
            if (file_blocks[target_index].second == end_size)
            {
                file_blocks[target_index].first = file_blocks[block_index].first;
                file_blocks[block_index].first = kBlank;
            }
            // Split Empty Block
            else
            {
                int remaining_space = file_blocks[target_index].second - file_blocks[block_index].second;
                file_blocks[target_index] = file_blocks[block_index];
                file_blocks[block_index].first = kBlank;
                file_blocks.insert(file_blocks.begin() + target_index + 1, {kBlank, remaining_space});
                block_index++;
            }
            break;
        }
    }
    // Calculate checksum
    int position = 0;
    long checksum = 0;
    for (pair<int, int> block : file_blocks)
    {
        for (int i = 0; i < block.second; i++)
        {
            if (block.first != kBlank)
            {
                checksum += block.first * position;
            }
            position++;
        }
    }

    std::cout << "Checksum: " << checksum << "\n";

    input_file.close();
    return 0;
}