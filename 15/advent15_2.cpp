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
const vector<vector<int>> kDirectionCoor = {
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

class WideObject
{
public:
    int x;
    int y;
    WideObject(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    WideObject(int x, int y, vector<int> addition)
    {
        this->x = x + addition[0];
        this->y = y + addition[1];
    }
    WideObject(pair<int, int> coord, vector<int> addition, bool right_side = false)
    {
        this->x = coord.first + addition[0];
        this->y = coord.second + addition[1];
        if (right_side)
        {
            y--;
        }
    }
    bool operator<(const WideObject &other) const
    {
        if (x != other.x)
        {
            return x < other.x;
        }
        return y < other.y;
    }
    vector<WideObject> ImpactableObjects(int dir)
    {
        vector<WideObject> return_list;
        if (dir == Direction::N || dir == Direction::S)
        {
            return_list.push_back(WideObject(x, y - 1, kDirectionCoor[dir]));
            return_list.push_back(WideObject(x, y, kDirectionCoor[dir]));
            return_list.push_back(WideObject(x, y + 1, kDirectionCoor[dir]));
        }
        else
        {
            WideObject adjacent_box = WideObject(x, y, kDirectionCoor[dir]);
            adjacent_box.Move(kDirectionCoor[dir]);
            return_list.push_back(adjacent_box);
        }
        return return_list;
    }
    void Move(vector<int> movement)
    {
        x += movement[0];
        y += movement[1];
    }
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
    pair<int, int> robot;
    set<WideObject> wall_list;
    set<WideObject> box_list;

    // row, col, direction
    vector<Direction> robot_instructions;

    // Iterate through each line and assemble the map as a 2d vector
    string input_line;
    int map_row = 0;
    int map_col = 0;
    while (getline(input_file, input_line))
    {
        bool is_map = false;
        int col = 0;
        // Go through each character in the line and apply it to the map and guard list
        for (char grid_char : input_line)
        {
            // Empty
            if (grid_char == '.')
            {
                is_map = true;
            }
            // Wall
            else if (grid_char == '#')
            {
                is_map = true;
                wall_list.insert({map_row, col * 2});
            } // Box
            else if (grid_char == 'O')
            {
                is_map = true;
                box_list.insert({map_row, col * 2});
            }
            // Robot
            else if (grid_char == '@')
            {
                is_map = true;
                robot = {map_row, col * 2};
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
                std::cerr << "Unknow Character at " << map_row << ", " << col << "\n ";
            }
            col++;
        }
        if (is_map)
        {
            map_row++;
            map_col = col;
        }
    }

    // Simulate the robot
    for (Direction dir : robot_instructions)
    {
        vector<WideObject> check_list;
        set<WideObject> blocking_box_set;
        bool blocked_flag = false;
        check_list.push_back(WideObject(robot, kDirectionCoor[dir]));
        check_list.push_back(WideObject(robot, kDirectionCoor[dir], true));
        for (int i = 0; i < check_list.size(); i++)
        {
            if (wall_list.find(check_list[i]) != wall_list.end())
            {
                blocked_flag = true;
                break;
            }
            if (box_list.find(check_list[i]) != box_list.end())
            {
                // Get rid of box to move it later and avoid repeat checks
                box_list.erase(box_list.find(check_list[i]));
                blocking_box_set.insert(check_list[i]);
                vector<WideObject> new_spots = check_list[i].ImpactableObjects(dir);
                check_list.insert(check_list.end(), new_spots.begin(), new_spots.end());
            }
        }
        // If blocked by a wall at some point
        if (blocked_flag)
        {
            // Put all the boxes back
            while (blocking_box_set.size() > 0)
            {
                WideObject box = *blocking_box_set.begin();
                blocking_box_set.erase(blocking_box_set.begin());
                box_list.insert(box);
            }
            continue;
        }
        // Move all boxes
        while (blocking_box_set.size() > 0)
        {
            WideObject box = *blocking_box_set.begin();
            blocking_box_set.erase(blocking_box_set.begin());
            box.Move(kDirectionCoor[dir]);
            box_list.insert(box);
        }

        // Move Robot
        robot.first += kDirectionCoor[dir][0];
        robot.second += kDirectionCoor[dir][1];
    }

    // Print Map
    for (int row = 0; row < map_row; row++)
    {
        for (int col = 0; col < map_col * 2; col++)
        {
            if (robot.first == row && robot.second == col)
            {
                std::cout << "@";
            }
            else if (wall_list.find(WideObject(row, col)) != wall_list.end())
            {
                std::cout << "#";
            }
            else if (wall_list.find(WideObject(row, col - 1)) != wall_list.end())
            {
                std::cout << "#";
            }
            else if (box_list.find(WideObject(row, col)) != box_list.end())
            {
                std::cout << "[";
            }
            else if (box_list.find(WideObject(row, col - 1)) != box_list.end())
            {
                std::cout << "]";
            }
            else
            {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }

    int gps_score = 0;

    while (box_list.size() > 0)
    {
        WideObject box = *box_list.begin();
        box_list.erase(box_list.begin());
        gps_score += 100 * box.x + box.y;
    }

    std::cout << "GPS Score: " << gps_score << "\n";

    input_file.close();
    return 0;
}