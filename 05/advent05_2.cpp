#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using std::regex;
using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input.txt";
const regex kRulePattern("[0-9]+\\|[0-9]+");
const regex kUpdatePattern("([0-9]+,)*[0-9]+");

int main()
{
    // Open the input file
    std::ifstream input_file(kInputFilePath);
    if (!input_file.is_open())
    {
        std::cerr << "File " << kInputFilePath << " was not found.\n";
        return 1;
    }

    // Iterate through each line and assemble the rules and updates
    string input_line;
    vector<vector<int>> rule_list;
    vector<vector<int>> update_list;
    while (getline(input_file, input_line))
    {

        // If is rule, get the separate numbers from the rule and add to rule list
        if (std::regex_match(input_line, kRulePattern))
        {
            size_t end_of_first_num = 0;
            vector<int> rule;
            rule.push_back(std::stoi(input_line, &end_of_first_num));
            rule.push_back(std::stoi(input_line.substr(end_of_first_num + 1)));
            rule_list.push_back(rule);
        }
        // If is Update, extract all numbers and add to update list
        else if (std::regex_match(input_line, kUpdatePattern))
        {
            vector<int> update;
            int scan_index = 0;
            // Loop through each number until the end of the line
            while (scan_index < input_line.size())
            {
                size_t end_of_num = 0;
                update.push_back(std::stoi(input_line.substr(scan_index), &end_of_num));
                // Increment over the number and comma
                scan_index += end_of_num + 1;
            }
            update_list.push_back(update);
        }
    }

    // Check for rules violations
    // Go through each update
    for (int update_index = 0; update_index < update_list.size(); update_index++)
    {
        int page_index = 0;
        bool fixed_update_flag = false;

        while (page_index < update_list[update_index].size())
        {
            int second_page = page_index;
            bool fixed_page_flag = false;
            // Go through each rule
            for (int rule_index = 0; rule_index < rule_list.size(); rule_index++)
            {
                if (update_list[update_index][page_index] == rule_list[rule_index][1])
                {
                    for (int first_page = second_page + 1; first_page < update_list[update_index].size(); first_page++)
                    {
                        if (update_list[update_index][first_page] == rule_list[rule_index][0])
                        {
                            // If the update is in violation, try to fix
                            if (first_page > second_page)
                            {
                                fixed_page_flag = true;
                                fixed_update_flag = true;
                                update_list[update_index].erase(update_list[update_index].begin() + first_page);
                                update_list[update_index].insert(update_list[update_index].begin() + second_page, rule_list[rule_index][0]);
                                second_page++;
                            }
                        }
                    }
                }
            }
            // Only increment if paged was good and not a new page at the index
            if (!fixed_page_flag)
            {
                page_index++;
            }
        }
        // If didn't need fixing, remove
        if (!fixed_update_flag)
        {
            update_list.erase(update_list.begin() + update_index);
            update_index--;
        }
    }

    // Get the sum of the middle numbers
    int middle_total = 0;
    for (int update_index = 0; update_index < update_list.size(); update_index++)
    {
        middle_total += update_list[update_index][update_list[update_index].size() / 2];
    }

    std::cout << "Middle Total: " << middle_total << "\n";
    return 0;
}