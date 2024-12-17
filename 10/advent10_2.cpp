#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using std::pair;
using std::set;
using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input.txt";
const int kTrailHead = 0;
const int kTrailEnd = 9;
const int kDirectionCoor[8][2] = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1},
};
enum Direction
{
    N = 0,
    E,
    S,
    W,
};

// The vector form of the search grid, avalible for all function to see after being filled out
vector<vector<int>> g_map_grid;

int FindTrails(int row, int col, int target_elevation)
{
    if (target_elevation > kTrailEnd)
    {
        return 1;
    }
    int trail_count = 0;
    for (int dir = Direction::N; dir <= Direction::W; dir++)
    {
        int next_row = row + kDirectionCoor[dir][0];
        int next_col = col + kDirectionCoor[dir][1];
        if (0 <= next_row && next_row < g_map_grid.size() &&
            0 <= next_col && next_col < g_map_grid[0].size())
        {
            if (g_map_grid[next_row][next_col] == target_elevation)
            {
                trail_count += FindTrails(next_row, next_col, target_elevation + 1);
            }
        }
    }
    return trail_count;
}

int CheckTrailheadScore(int row, int col)
{
    // Check if is a trailhead
    if (g_map_grid[row][col] == kTrailHead)
    {
        return FindTrails(row, col, kTrailHead + 1);
    }

    return 0;
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

    // Iterate through each line and assemble the wordsearch as a 2d vector in g_search_grid
    string input_line;
    while (getline(input_file, input_line))
    {
        vector<int> grid_row;
        // Go through each letter in the line and add it to the vecor row
        for (char grid_letter : input_line)
        {
            grid_row.push_back(grid_letter - '0');
        }
        // Add the vector row to the grid
        g_map_grid.push_back(grid_row);
    }

    // Check each spot for words (assumes regular grid with at least 1 row)
    int trail_score_total = 0;
    for (int row = 0; row < g_map_grid.size(); row++)
    {
        for (int col = 0; col < g_map_grid[0].size(); col++)
        {
            trail_score_total += CheckTrailheadScore(row, col);
        }
    }

    std::cout << "Trailhead Total: " << trail_score_total << "\n";

    input_file.close();
    return 0;
}