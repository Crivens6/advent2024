#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input.txt";

// Return true if position is on the map grid space
bool PositionOnMapCheck(pair<int, int> position, int row_count, int col_count)
{
    return (0 <= position.first && position.first < row_count) && (0 <= position.second && position.second < col_count);
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

    // Iterate through each line and assemble the map as a 2d vector
    map<char, int> char_to_freq_map;
    // [Frequency][Instance][Row,Col]
    vector<vector<pair<int, int>>> all_frequency_location_list;
    string input_line;
    // Count current row/col and keep map size after
    int map_row = 0;
    int map_col = 0;
    while (getline(input_file, input_line))
    {
        vector<int> grid_row;
        map_col = 0;
        // Go through each character in the line and record any towers
        for (char grid_char : input_line)
        {
            if (grid_char != '.')
            {
                map<char, int>::iterator frequency_char = char_to_freq_map.find(grid_char);
                // Check if new frequency
                if (frequency_char == char_to_freq_map.end())
                {
                    // Add location as a new location list
                    int frequency = all_frequency_location_list.size();
                    vector<pair<int, int>> new_location_list = {{map_row, map_col}};
                    char_to_freq_map[grid_char] = frequency;
                    all_frequency_location_list.push_back(new_location_list);
                }
                // frequency already been encountered
                else
                {
                    // Add location to existing location list for the frequency
                    int frequency = char_to_freq_map[grid_char];
                    pair<int, int> new_location = {map_row, map_col};
                    all_frequency_location_list[frequency].push_back(new_location);
                }
            }
            map_col++;
        }
        map_row++;
    }

    /// Calculate antinodes
    set<pair<int, int>> antinode_locations;
    int antinode_count = 0;
    // Iterate through each frequency
    for (auto location_list : all_frequency_location_list)
    {
        // Go through each location in the list
        for (int location_a = 0; location_a < location_list.size(); location_a++)
        {
            // Go through each subsequent location on the list
            for (int location_b = location_a + 1; location_b < location_list.size(); location_b++)
            {
                // Get the distance between points and convert it into a slope
                pair vector_difference = {location_list[location_a].first - location_list[location_b].first,
                                          location_list[location_a].second - location_list[location_b].second};
                pair antinode_a = {location_list[location_a].first + vector_difference.first,
                                   location_list[location_a].second + vector_difference.second};
                pair antinode_b = {location_list[location_b].first - vector_difference.first,
                                   location_list[location_b].second - vector_difference.second};
                // Check that antinode_a doesn't already exist and is a position on the map
                if (antinode_locations.count(antinode_a) == 0 && PositionOnMapCheck(antinode_a, map_row, map_col))
                {
                    antinode_locations.insert(antinode_a);
                    antinode_count++;
                }
                // Check that antinode_b doesn't already exist and is a position on the map
                if (antinode_locations.count(antinode_b) == 0 && PositionOnMapCheck(antinode_b, map_row, map_col))
                {
                    antinode_locations.insert(antinode_b);
                    antinode_count++;
                }
            }
        }
    }

    std::cout << "Antinodes: " << antinode_count << "\n";
    input_file.close();
    return 0;
}