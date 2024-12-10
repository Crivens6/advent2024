#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input.txt";

// The vector form of the search grid, avalible for all function to see after being filled out
vector<vector<char>> g_search_grid;

// Returns true if coords are the center of an X-MAS
bool SearchX(int row, int col)
{
    // Check if correct starting letter
    if (g_search_grid[row][col] != 'A')
    {
        return false;
    }

    // Check that coor is far enough form edge to from an X
    if (row < 1)
    {
        return false;
    }
    if (col < 1)
    {
        return false;
    }
    if (row > (int)g_search_grid.size() - 2)
    {
        return false;
    }
    if (col > (int)g_search_grid[0].size() - 2)
    {
        return false;
    }

    // Check backslash diagonal
    char top_location_character = g_search_grid[row - 1][col - 1];
    char bottom_location_character = g_search_grid[row + 1][col + 1];
    if (!(top_location_character == 'M' && bottom_location_character == 'S'))
    {
        if (!(top_location_character == 'S' && bottom_location_character == 'M'))
        {
            return false;
        }
    }

    // Check forward diagonal
    top_location_character = g_search_grid[row - 1][col + 1];
    bottom_location_character = g_search_grid[row + 1][col - 1];
    if (!(top_location_character == 'M' && bottom_location_character == 'S'))
    {
        if (!(top_location_character == 'S' && bottom_location_character == 'M'))
        {
            return false;
        }
    }

    return true;
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
        vector<char> grid_row;
        // Go through each letter in the line and add it to the vecor row
        for (char grid_letter : input_line)
        {
            grid_row.push_back(grid_letter);
        }
        // Add the vector row to the grid
        g_search_grid.push_back(grid_row);
    }

    // Check each spot for X-MAS (assumes regular grid with at least 1 row)
    int xmas_total = 0;
    for (int row = 0; row < g_search_grid.size(); row++)
    {
        for (int col = 0; col < g_search_grid[0].size(); col++)
        {
            if (SearchX(row, col))
            {
                xmas_total++;
            }
        }
    }

    std::cout << "X-MAS Total: " << xmas_total << "\n";

    return 0;
}