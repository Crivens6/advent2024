#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;

// Constants
const string kInputFilePath = "input.txt";
const int kDampenerRange = 2;

int CheckReport(std::vector<int> report_list)
{
    bool first_number = true;
    bool increasing = false;
    // Scan through all numbers in report
    for (int list_index = 1; list_index < report_list.size(); list_index++)
    {
        // Get current number and difference with previous number
        int previous_number = report_list[list_index - 1];
        int current_number = report_list[list_index];
        int difference = previous_number - current_number;
        // Break if incompatible difference in readings
        if (difference == 0 || abs(difference) > 3)
        {
            return list_index;
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
            return list_index;
        }
        // Break if direction changes
        else if (difference < 0 && !increasing)
        {
            return list_index;
        }
    }
    return -1;
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

    // Itterate through each report and check if it's safe
    int safe_report_count = 0;
    string report_line;
    while (getline(input_file, report_line))
    {
        std::vector<int> report_list;
        size_t scan_index = 0;
        // Scan through all numbers in report
        while (scan_index < report_line.length())
        {
            size_t number_length = 0;
            report_list.push_back(std::stoi(report_line.substr(scan_index), &number_length));
            scan_index += number_length;
        }

        int error_index = CheckReport(report_list);
        if (error_index == -1)
        {
            safe_report_count++;
            continue;
        }

        // Problem Dampener
        for (int test_bad_index = std::max(0, error_index - kDampenerRange); test_bad_index <= error_index; test_bad_index++)
        {
            std::vector<int> altered_list(report_list);
            altered_list.erase(altered_list.begin() + test_bad_index);
            int error_index = CheckReport(altered_list);
            if (error_index == -1)
            {
                safe_report_count++;
                for (auto v : report_list)
                    std::cout << v << " ";
                std::cout << "-> ";
                for (auto v : altered_list)
                    std::cout << v << " ";
                std::cout << "\n";
                break;
            }
        }
    }

    std::cout << "Safe Reports: " << safe_report_count << "\n";
    return 0;
}
