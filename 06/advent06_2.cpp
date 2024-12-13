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
    Start,
    Walked,
    Crate
};

vector<vector<int>> g_map_grid;
// row, col, direction
vector<vector<int>> g_guard_positions;

bool CheckIfCreatesLoop(int row, int col)
{
    // Simulate Guards
    for (vector<int> guard : g_guard_positions)
    {
        // Keep track of guard movment on each space to track a loop
        bool guard_path[g_map_grid.size()][g_map_grid[0].size()][4];
        for (int i = 0; i < g_map_grid.size(); ++i)
        {
            for (int j = 0; j < g_map_grid[0].size(); ++j)
            {
                for (int k = 0; k < 4; ++k)
                {
                    guard_path[i][j][k] = false;
                }
            }
        }
        int pos[2] = {guard[0], guard[1]};
        int dir = guard[2];
        int count = 0;
        while (true)
        {
            int next_pos[2] = {pos[0] + kDirectionCoor[dir][0], pos[1] + kDirectionCoor[dir][1]};

            // Finish if off the row
            if (0 > next_pos[0] || next_pos[0] > g_map_grid[0].size() - 1)
            {
                break;
            }
            // Finish if off the col
            if (0 > next_pos[1] || next_pos[1] > g_map_grid.size() - 1)
            {
                break;
            }
            // Has looped
            if (guard_path[pos[0]][pos[1]][dir])
            {
                return true;
            }
            // Mark the path for loop checking
            guard_path[pos[0]][pos[1]][dir] = true;

            // If blocked (or possible new crate), turn
            if (g_map_grid[next_pos[0]][next_pos[1]] == GridSquare::Crate || (next_pos[0] == row && next_pos[1] == col))
            {
                dir += 1;
                if (dir >= 4)
                {
                    dir -= 4;
                }
            }
            // If clear, proceed
            else
            {
                pos[0] = next_pos[0];
                pos[1] = next_pos[1];
            }
        }
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
            // Clear
            if (grid_char == '.')
            {
                grid_row.push_back(GridSquare::Empty);
            }
            // Crate
            else if (grid_char == '#')
            {
                grid_row.push_back(GridSquare::Crate);
            }
            // Guards starts
            else if (grid_char == '^')
            {
                grid_row.push_back(GridSquare::Start);
                vector<int> guard = {row, col, Direction::N};
                g_guard_positions.push_back(guard);
            }
            else if (grid_char == '>')
            {
                grid_row.push_back(GridSquare::Start);
                vector<int> guard = {row, col, Direction::E};
                g_guard_positions.push_back(guard);
            }
            else if (grid_char == '<')
            {
                grid_row.push_back(GridSquare::Start);
                vector<int> guard = {row, col, Direction::W};
                g_guard_positions.push_back(guard);
            }
            else if (grid_char == 'v')
            {
                grid_row.push_back(GridSquare::Start);
                vector<int> guard = {row, col, Direction::S};
                g_guard_positions.push_back(guard);
            }
            else
            {
                std::cerr << "Unknow Character at " << row << ", " << col << "\n ";
            }
            col++;
        }
        // Add the vector row to the grid
        g_map_grid.push_back(grid_row);
        row++;
    }

    // Simulate Guards normal path
    for (vector<int> guard : g_guard_positions)
    {
        int pos[2] = {guard[0], guard[1]};
        int dir = guard[2];
        // Simulate until guard is off the grid
        while (true)
        {
            int next_pos[2] = {pos[0] + kDirectionCoor[dir][0], pos[1] + kDirectionCoor[dir][1]};
            // Finish if off the row
            if (0 > next_pos[0] || next_pos[0] > g_map_grid[0].size() - 1)
            {
                break;
            }
            // Finish if off the col
            if (0 > next_pos[1] || next_pos[1] > g_map_grid.size() - 1)
            {
                break;
            }

            // If crate, then turn
            if (g_map_grid[next_pos[0]][next_pos[1]] == GridSquare::Crate)
            {
                dir += 1;
                if (dir >= 4)
                {
                    dir -= 4;
                }
            }
            // if clear, proceed
            else
            {
                pos[0] = next_pos[0];
                pos[1] = next_pos[1];
                if (g_map_grid[pos[0]][pos[1]] == GridSquare::Empty)
                {
                    g_map_grid[pos[0]][pos[1]] = GridSquare::Walked;
                }
            }

            // Finish if return to same spot
            if (next_pos[0] == guard[0] && next_pos[1] == guard[1] && dir == guard[2])
            {
                break;
            }
        }
    }

    // Check each interceptable spot for if a crate causes a loop
    int loops_created = 0;
    for (int row = 0; row < g_map_grid.size(); row++)
    {
        for (int col = 0; col < g_map_grid[0].size(); col++)
        {
            if (g_map_grid[row][col] == GridSquare::Walked)
            {
                if (CheckIfCreatesLoop(row, col))
                {
                    loops_created++;
                }
            }
        }
    }

    std::cout << "Loops Created: " << loops_created << "\n";

    input_file.close();
    return 0;
}