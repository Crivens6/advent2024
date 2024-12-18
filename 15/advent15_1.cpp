#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::pair;
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
    Wall,
    Box
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

    // Array map of room
    vector<vector<int>> map_grid;
    pair<int, int> robot_coords;
    // row, col, direction
    vector<Direction> robot_instructions;

    // Iterate through each line and assemble the map as a 2d vector
    string input_line;
    int row = 0;
    while (getline(input_file, input_line))
    {
        vector<int> grid_row;
        int col = 0;
        bool is_map_flag = false;
        // Go through each character in the line and apply it to the map and guard list
        for (char grid_char : input_line)
        {
            // Empty
            if (grid_char == '.')
            {
                grid_row.push_back(GridSquare::Empty);
                is_map_flag = true;
            }
            // Wall
            else if (grid_char == '#')
            {
                grid_row.push_back(GridSquare::Wall);
                is_map_flag = true;
            } // Box
            else if (grid_char == 'O')
            {
                grid_row.push_back(GridSquare::Box);
                is_map_flag = true;
            }
            // Robot
            else if (grid_char == '@')
            {
                grid_row.push_back(GridSquare::Empty);
                robot_coords = {row, col};
                is_map_flag = true;
            }
            // Robot Instructions
            else if (grid_char == '^')
            {
                robot_instructions.push_back(Direction::N);
            }
            else if (grid_char == '>')
            {
                robot_instructions.push_back(Direction::E);
            }
            else if (grid_char == '<')
            {
                robot_instructions.push_back(Direction::W);
            }
            else if (grid_char == 'v')
            {
                robot_instructions.push_back(Direction::S);
            }
            else
            {
                std::cerr << "Unknow Character at " << row << ", " << col << "\n ";
            }
            col++;
        }
        // Add the vector row to the grid
        if (is_map_flag)
        {
            map_grid.push_back(grid_row);
        }
        row++;
    }

    // Simulate the robot
    for (Direction dir : robot_instructions)
    {
        // Check what is in that direction
        pair<int, int> new_position = {robot_coords.first + kDirectionCoor[dir][0], robot_coords.second + kDirectionCoor[dir][1]};
        // Check not trying to walk off map (treat as a wall)
        if (0 > new_position.first || new_position.first >= map_grid.size() || 0 > new_position.second || new_position.second >= map_grid[0].size())
        {
            continue;
        }
        GridSquare new_square = (GridSquare)map_grid[new_position.first][new_position.second];
        if (new_square == GridSquare::Empty)
        {
            robot_coords = new_position;
        }
        else if (new_square == GridSquare::Box)
        {
            pair<int, int> next_spot = {new_position.first + kDirectionCoor[dir][0], new_position.second + kDirectionCoor[dir][1]};

            while (0 <= next_spot.first && next_spot.first < map_grid.size() && 0 <= next_spot.second && next_spot.second < map_grid[0].size())
            {

                GridSquare next_square = (GridSquare)map_grid[next_spot.first][next_spot.second];
                if (next_square == GridSquare::Wall)
                {
                    break;
                }
                else if (next_square == GridSquare::Box)
                {
                    next_spot = {next_spot.first + kDirectionCoor[dir][0], next_spot.second + kDirectionCoor[dir][1]};
                }
                else
                {
                    map_grid[new_position.first][new_position.second] = GridSquare::Empty;
                    map_grid[next_spot.first][next_spot.second] = GridSquare::Box;
                    robot_coords = new_position;
                    break;
                }
            }
        }
    }

    int gps_score = 0;

    for (int row = 0; row < map_grid.size(); row++)
    {
        for (int col = 0; col < map_grid[0].size(); col++)
        {
            if (map_grid[row][col] == GridSquare::Box)
            {
                gps_score += 100 * row + col;
            }
        }
    }

    std::cout << "GPS Score: " << gps_score << "\n";

    input_file.close();
    return 0;
}