#include <fstream>
#include <iostream>
#include <string>
#include <list>

using std::list;
using std::pair;
using std::string;
// Constants
const string kInputFilePath = "input.txt";

int main()
{
    // Open the input file
    std::ifstream input_file(kInputFilePath);
    if (!input_file.is_open())
    {
        std::cerr << "File " << kInputFilePath << " was not found.\n";
        return 1;
    }

    std::cout << "FILL FILES\n";

    // Iterate through each line and track the file blocks and blank blocks
    list<pair<int, int>> file_blocks;
    list<int> blank_blocks;
    string input_line;
    bool is_file = true;
    int file_id = 0;
    while (getline(input_file, input_line))
    {
        // Go through each letter in the line and add it to the vecor row
        for (char input_char : input_line)
        {
            if (is_file)
            {
                pair<int, int> new_file = {file_id, input_char - '0'};
                file_blocks.push_back(new_file);
                file_id++;
            }
            else
            {
                blank_blocks.push_back(input_char - '0');
            }
        }
    }

    std::cout << "FILL BLANK\n";
    // Iterate and move end file blocks into blank file block areas
    list<list<pair<int, int>>> filled_blocks;
    int blank_block_index = 0;
    while (filled_blocks.size() < file_blocks.size())
    {
        // Create at filled block vector at the current blank block index if one does not exist
        if (filled_blocks.size() == blank_block_index)
        {
            filled_blocks.push_back({{}});
        }
        // Check out the end file
        pair<int, int> end_block = file_blocks.back();
        file_blocks.pop_back();
        // Check out the front blank block
        int empty_space = blank_blocks.front();
        blank_blocks.pop_front();
        // Blank block smaller than end file
        if (empty_space < end_block.second)
        {
            // Fill empty block completly
            filled_blocks.back().push_back({end_block.first, empty_space});
            // Create reduce the remaining file size in end file and put back
            pair<int, int> reduced_end_block = end_block;
            reduced_end_block.second -= empty_space;
            file_blocks.push_back(reduced_end_block);
        }
        // Blank block larger than end file
        else if (empty_space > end_block.second)
        {
            // Put remain file block into blank block
            filled_blocks.back().push_back(end_block);
            // Calculate the remaining size of the remaining blank block and put back
            blank_blocks.push_front(empty_space - end_block.second);
        }
        // Blank block exact size of file
        else
        {
            // Put remain file block into blank block
            filled_blocks.back().push_back(end_block);
        }
    }
    std::cout << "CHECKSUM\n";
    // Calculate checksum
    int position = 0;
    long checksum = 0;
    while (file_blocks.size() > 0)
    {
        for (int i = 0; i < file_blocks.front().second; i++)
        {
            checksum += position * file_blocks.front().first;
            position++;
        }
        file_blocks.pop_front();
        while (filled_blocks.front().size() > 0)
        {
            pair<int, int> sub_block = filled_blocks.front().front();
            for (int i = 0; i < sub_block.second; i++)
            {
                checksum += position * sub_block.first;
                position++;
            }
            filled_blocks.front().pop_front();
        }
        filled_blocks.pop_front();
    }

    std::cout << "Checksum: " << checksum << "\n";

    input_file.close();
    return 0;
}