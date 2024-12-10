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
const regex kMultPattern("mul\\([0-9]{1,3},[0-9]{1,3}\\)");

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
    int total_product = 0;
    string input_line;
    while (getline(input_file, input_line))
    {
        // Find instruction matches with regex
        regex_iterator<string::iterator> r_iterator(input_line.begin(), input_line.end(), kMultPattern);
        regex_iterator<string::iterator> r_end;

        // Go through all matches
        while (r_iterator != r_end)
        {
            // Extract the instruction string
            string mult_match = r_iterator->str();

            // Extract the two multipliers from the string
            size_t end_of_first_mult = 0;
            int mult_a = std::stoi(mult_match.substr(4), &end_of_first_mult);
            int mult_b = std::stoi(mult_match.substr(end_of_first_mult + 5));

            // Increment by the product
            total_product += mult_a * mult_b;

            // Go to next match
            ++r_iterator;
        }
    }

    std::cout << "Total Product: " << total_product << "\n";

    return 0;
}