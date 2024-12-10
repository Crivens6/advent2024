#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

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

    // Itterate through each report and check if it's safe
    int safe_report_count = 0;
    string report_line;
    while (getline(input_file, report_line))
    {
        size_t scan_index = 0;
        int previous_number = std::stoi(report_line.substr(scan_index), &scan_index);
        bool first_number = true;
        bool increasing = false;
        bool safe_flag = true;
        // Scan through all numbers in report
        while (scan_index < report_line.length())
        {
            // Get current number and difference with previous number
            size_t number_length = 0;
            int current_number = std::stoi(report_line.substr(scan_index), &number_length);
            scan_index += number_length;
            int difference = previous_number - current_number;
            // Break if incompatible difference in readings
            if (difference == 0 || abs(difference) > 3)
            {
                safe_flag = false;
                break;
            }
            // Set increasing or decreasing if first number
            else if (first_number)
            {
                if (difference < 0)
                {
                    increasing = true;
                }
                else
                {
                    increasing = false;
                }
                first_number = false;
            }
            // Break if direction changes
            else if (difference > 0 && increasing)
            {
                safe_flag = false;
                break;
            }
            // Break if direction changes
            else if (difference < 0 && !increasing)
            {
                safe_flag = false;
                break;
            }
            // Set previous to current
            previous_number = current_number;
        }
        // Increment if report is safe
        if (safe_flag)
        {
            safe_report_count++;
        }
    }

    std::cout << "Safe Reports: " << safe_report_count << "\n";
    return 0;
}