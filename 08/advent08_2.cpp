#include <cmath>
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

// Return the same slope with smallest ints possible
pair<int, int> MinimizeStep(pair<int, int> step_vector)
{
    int greatest_common_denominator = std::min(abs(step_vector.first), abs(step_vector.second));
    // Find the greatest common denominator by tring the smaller number, then tring 1 smaller until GCD is found
    while (greatest_common_denominator > 0)
    {
        if (step_vector.first % greatest_common_denominator == 0 && step_vector.second % greatest_common_denominator == 0)
        {
            break;
        }
        greatest_common_denominator--;
    }
    // Set greates common denominator to other number if one of them is 0
    if (greatest_common_denominator == 0)
    {
        greatest_common_denominator = step_vector.first + step_vector.second;
    }
    // Return the step vecotr divided by the greatest common denominator
    return {step_vector.first / greatest_common_denominator, step_vector.second / greatest_common_denominator};
}

// Return true if position is on the map grid space
bool PositionOnMapCheck(pair<int, int> position, int row_count, int col_count)
{
    return (0 <= position.first && position.first < row_count) && (0 <= position.second && position.second < col_count);
}

// Return a position on the edge of the map along a slope intersection with point
pair<int, int> FindSlopeEdge(pair<int, int> point, pair<int, int> slope, int row_count, int col_count)
{
    pair<int, int> start_point = point;
    // Step to as close to the edge as the origin point and slope allow
    while (true)
    {
        // Check if next step would be off the map
        pair<int, int> new_start_point = {start_point.first - slope.first, start_point.second - slope.second};
        if (!PositionOnMapCheck(new_start_point, row_count, col_count))
        {
            return start_point;
        }
        start_point = new_start_point;
    }
    return point;
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
                pair<int, int> vector_difference = {location_list[location_a].first - location_list[location_b].first,
                                                    location_list[location_a].second - location_list[location_b].second};
                pair<int, int> slope = MinimizeStep(vector_difference);
                pair<int, int> position = FindSlopeEdge(location_list[location_a], slope, map_row, map_col);
                // Check everypoint on the slope interesection the towers
                while (PositionOnMapCheck(position, map_row, map_col))
                {
                    if (antinode_locations.count(position) == 0)
                    {
                        antinode_locations.insert(position);
                        antinode_count++;
                    }
                    position.first += slope.first;
                    position.second += slope.second;
                }
            }
        }
    }

    std::cout << "Antinodes: " << antinode_count << "\n";
    return 0;
}