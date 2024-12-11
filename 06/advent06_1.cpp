#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input.txt";
const int kDirectionCoor[4][2] = {
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

enum GridSquare
{
    Empty = 0,
    Walked,
    Crate
};

int main()
{
    // Open the input file
    std::ifstream input_file(kInputFilePath);
    if (!input_file.is_open())
    {
        std::cerr << "File " << kInputFilePath << " was not found.\n";
        return 1;
    }

    // Array map of room with each containing a GridSquare
    vector<vector<int>> map_grid;
    // row, col, direction
    vector<vector<int>> guard_positions;

    // Iterate through each line and assemble the map as a 2d vector
    string input_line;
    int row = 0;
    while (getline(input_file, input_line))
    {
        vector<int> grid_row;
        int col = 0;
        // Go through each character in the line and apply it to the map and guard list
        for (char grid_char : input_line)
        {
            // Empty
            if (grid_char == '.')
            {
                grid_row.push_back(GridSquare::Empty);
            }
            // Crate
            else if (grid_char == '#')
            {
                grid_row.push_back(GridSquare::Crate);
            }
            // Guard Start
            else if (grid_char == '^')
            {
                grid_row.push_back(GridSquare::Walked);
                vector<int> guard = {row, col, Direction::N};
                guard_positions.push_back(guard);
            }
            else if (grid_char == '>')
            {
                grid_row.push_back(GridSquare::Walked);
                vector<int> guard = {row, col, Direction::E};
                guard_positions.push_back(guard);
            }
            else if (grid_char == '<')
            {
                grid_row.push_back(GridSquare::Walked);
                vector<int> guard = {row, col, Direction::W};
                guard_positions.push_back(guard);
            }
            else if (grid_char == 'v')
            {
                grid_row.push_back(GridSquare::Walked);
                vector<int> guard = {row, col, Direction::S};
                guard_positions.push_back(guard);
            }
            else
            {
                std::cerr << "Unknow Character at " << row << ", " << col << "\n ";
            }
            col++;
        }
        // Add the vector row to the grid
        map_grid.push_back(grid_row);
        row++;
    }

    // Simulate Guards
    for (vector<int> guard : guard_positions)
    {
        int pos[2] = {guard[0], guard[1]};
        int dir = guard[2];
        std::cout << "G:  " << guard[0] << "," << guard[1] << "\n";
        // Simulate until guard is off the map
        while (true)
        {
            int next_pos[2] = {pos[0] + kDirectionCoor[dir][0], pos[1] + kDirectionCoor[dir][1]};
            std::cout << "NG:  " << next_pos[0] << "," << pos[1] + kDirectionCoor[dir][1] << "\n";
            // Finish if off the row
            if (0 > next_pos[0] || next_pos[0] > map_grid[0].size() - 1)
            {
                std::cout << "Row " << next_pos[0] << "\n";
                break;
            }
            // Finish if off the col
            if (0 > next_pos[1] || next_pos[1] > map_grid.size() - 1)
            {
                std::cout << "Col\n";
                break;
            }

            // If facing box turn
            if (map_grid[next_pos[0]][next_pos[1]] == GridSquare::Crate)
            {
                dir += 1;
                if (dir >= 4)
                {
                    dir -= 4;
                }
            }
            // If clear, then proceed
            else
            {
                pos[0] = next_pos[0];
                pos[1] = next_pos[1];
                map_grid[pos[0]][pos[1]] = GridSquare::Walked;
            }
        }
    }

    // Count number of walked on spots
    int total_walked_space = 0;
    for (vector<int> row : map_grid)
    {
        for (int square : row)
        {
            if (square == GridSquare::Walked)
            {
                total_walked_space++;
            }
        }
    }

    std::cout << "Total Walked Spaces: " << total_walked_space << "\n";

    return 0;
}