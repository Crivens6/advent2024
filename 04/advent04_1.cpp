#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input.txt";
const string kSearchWord = "XMAS";
const int kDirectionCoor[8][2] = {
    {-1, 0},
    {-1, 1},
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1},
};

enum Direction
{
    N = 0,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW
};

// The vector form of the search grid, avalible for all function to see after being filled out
vector<vector<char>> g_search_grid;

bool Search_Direction(int current_character_index, int current_location[2], Direction direction)
{
    // Find the next location, the character there and next needed character
    int next_location[2] = {current_location[0] + kDirectionCoor[direction][0],
                            current_location[1] + kDirectionCoor[direction][1]};
    char next_needed_character = kSearchWord[current_character_index + 1];
    char next_location_character = g_search_grid[next_location[0]][next_location[1]];

    // Check if the next location is the next needed character
    if (next_location_character == next_needed_character)
    {
        // If found full word, return true
        if (current_character_index + 1 >= kSearchWord.size() - 1)
        {
            return true;
        }
        // Else, check next letter
        return Search_Direction(current_character_index + 1, next_location, direction);
    }
    else
    {
        return false;
    }
}

int CheckSpot(int row, int col)
{
    // Check if correct starting letter, else return 0
    if (g_search_grid[row][col] != kSearchWord[0])
    {
        return 0;
    }

    // Check viable directions based on edges (assumes regular array with at least one row)
    bool check_north = true, check_east = true, check_south = true, check_west = true;
    if (row + 1 < kSearchWord.size())
    {
        check_north = false;
    }
    if (col + 1 < kSearchWord.size())
    {
        check_west = false;
    }
    if (row > (int)g_search_grid.size() - (int)kSearchWord.size())
    {
        check_south = false;
    }
    if (col > (int)g_search_grid[0].size() - (int)kSearchWord.size())
    {
        check_east = false;
    }

    // Check each viable direction
    int words_found = 0;
    int current_coor[2] = {row, col};
    if (check_north)
    {
        if (Search_Direction(0, current_coor, Direction::N))
        {
            words_found++;
        }
    }
    if (check_north && check_east)
    {
        if (Search_Direction(0, current_coor, Direction::NE))
        {
            words_found++;
        }
    }
    if (check_east)
    {
        if (Search_Direction(0, current_coor, Direction::E))
        {
            words_found++;
        }
    }
    if (check_south && check_east)
    {
        if (Search_Direction(0, current_coor, Direction::SE))
        {
            words_found++;
        }
    }
    if (check_south)
    {
        if (Search_Direction(0, current_coor, Direction::S))
        {
            words_found++;
        }
    }
    if (check_south && check_west)
    {
        if (Search_Direction(0, current_coor, Direction::SW))
        {
            words_found++;
        }
    }
    if (check_west)
    {
        if (Search_Direction(0, current_coor, Direction::W))
        {
            words_found++;
        }
    }
    if (check_north && check_west)
    {
        if (Search_Direction(0, current_coor, Direction::NW))
        {
            words_found++;
        }
    }
    return words_found;
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

    // Check each spot for words (assumes regular grid with at least 1 row)
    int word_total = 0;
    for (int row = 0; row < g_search_grid.size(); row++)
    {
        for (int col = 0; col < g_search_grid[0].size(); col++)
        {
            word_total += CheckSpot(row, col);
        }
    }

    std::cout << "Word Total: " << word_total << "\n";

    input_file.close();
    return 0;
}