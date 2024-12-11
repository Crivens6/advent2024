#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input.txt";

bool CheckIfEquationPossible(vector<long> equation, long total)
{
    // Check if on last digit
    if (equation.size() == 1)
    {
        if (equation[0] == total)
        {
            return true;
        }
        return false;
    }
    vector<long> shorter_equation = equation;
    shorter_equation.erase(shorter_equation.begin() + 1);

    // Try addition for current digit
    if (CheckIfEquationPossible(shorter_equation, total + equation[1]))
    {
        return true;
    }
    // Try multiplication for current digit
    if (CheckIfEquationPossible(shorter_equation, total * equation[1]))
    {
        return true;
    }
    // Try  combining current digit
    string combined_digits = std::to_string(total) + std::to_string(equation[1]);
    if (CheckIfEquationPossible(shorter_equation, stol(combined_digits)))
    {
        return true;
    }
    return false;
}

int main()
{
    // Open the input file
    std::ifstream input_file(kInputFilePath);
    if (!input_file.is_open())
    {
        std::cerr << "File " << kInputFilePath << " was not found.\n";
        return 1;
    }

    // Iterate through each line and scan for equation
    string input_line;
    vector<vector<long>> equation_list;
    while (getline(input_file, input_line))
    {
        vector<long> equation;
        int scan_index = 0;

        // Get the first num
        size_t end_of_num = 0;
        equation.push_back(std::stol(input_line.substr(scan_index), &end_of_num));
        // Increment over the number, colon, and space
        scan_index += end_of_num + 2;

        // Loop through each number until the end of the line
        while (scan_index < input_line.size())
        {
            equation.push_back(std::stol(input_line.substr(scan_index), &end_of_num));
            // Increment over the number and space
            scan_index += end_of_num + 1;
        }
        equation_list.push_back(equation);
    }

    long calibration_sum = 0;

    // Check if each eqaution is possible
    for (auto equation : equation_list)
    {
        vector<long> shorter_equation = equation;
        shorter_equation.erase(shorter_equation.begin() + 1);
        if (CheckIfEquationPossible(shorter_equation, equation[1]))
        {
            calibration_sum += equation[0];
        }
    }

    std::cout << "Calibration Sum: " << calibration_sum << "\n";

    return 0;
}