#include <deque>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using std::deque;
using std::pair;
using std::set;
using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input_other.txt";
const int kMoveScore = 1;
const int kTurnScore = 1000;
const int kMoveDir[4][2] = {
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

enum MazeSquare
{
    Empty = -1,
    Wall = -2,
    Start = 0,
    End = -3
};

vector<vector<int>> g_map_grid;

class PathFinder
{
private:
    int x;
    int y;
    int score;
    Direction dir;
    set<pair<int, int>> history;
    pair<int, int> GetNextCoord(Direction next_dir)
    {
        return {x + kMoveDir[next_dir][0], y + kMoveDir[next_dir][1]};
    }

public:
    PathFinder(pair<int, int> coords, Direction dir = Direction::E, int score = 0, set<pair<int, int>> history = {})
    {
        this->x = coords.first;
        this->y = coords.second;
        this->score = score;
        this->dir = dir;
        this->history = history;
        std::cout << "    <" << x << "," << y << ">\n";
    }
    deque<PathFinder> NextGen()
    {
        deque<PathFinder> next_gen_list;
        history.insert({x, y});
        // Forward
        next_gen_list.push_back(PathFinder(GetNextCoord(dir), dir, score + kMoveScore, history));
        // Left
        Direction left_dir = (Direction)(((int)dir + 3) % 4);
        next_gen_list.push_back(PathFinder(GetNextCoord(left_dir), left_dir, score + kMoveScore + kTurnScore, history));
        // Right
        Direction right_dir = Direction(((int)dir + 1) % 4);
        next_gen_list.push_back(PathFinder(GetNextCoord(right_dir), right_dir, score + kMoveScore + kTurnScore, history));

        return next_gen_list;
    }
    MazeSquare CheckSpot(int lowest_score)
    {
        if (score >= lowest_score && lowest_score != -1)
        {
            return MazeSquare::Wall;
        }
        if (history.find({x, y}) != history.end())
        {
            return MazeSquare::Wall;
        }
        if (g_map_grid[x][y] > score + kTurnScore)
        {
            return MazeSquare::Wall;
        }
        MazeSquare my_spot = (MazeSquare)g_map_grid[x][y];
        g_map_grid[x][y] = std::min(score, g_map_grid[x][y]);
        return my_spot;
    }
    int GetScore()
    {
        return score;
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
    pair<int, int> start;

    // Iterate through each line and assemble the map as a 2d vector
    string input_line;
    while (getline(input_file, input_line))
    {
        vector<int> grid_row;
        // Go through each character in the line and apply it to the map and guard list
        for (char grid_char : input_line)
        {
            // Wall
            if (grid_char == '#')
            {
                grid_row.push_back(MazeSquare::Wall);
            }
            // Empty
            else if (grid_char == '.')
            {
                grid_row.push_back(MazeSquare::Empty);
            }
            // Start
            else if (grid_char == 'S')
            {
                start = {g_map_grid.size(), grid_row.size()};
                grid_row.push_back(MazeSquare::Start);
            }
            // End
            else if (grid_char == 'E')
            {
                grid_row.push_back(MazeSquare::End);
            }
        }
        // Add the vector row to the grid
        g_map_grid.push_back(grid_row);
    }
    input_file.close();

    deque<PathFinder> scout_list;
    scout_list.push_back(PathFinder(start));
    if (g_map_grid[start.first][start.second - 1] != MazeSquare::Wall)
    {
        scout_list.push_back(PathFinder({start.first, start.second - 1}, Direction::W, 2 * kTurnScore + kMoveScore));
    }
    int min_score = -1;
    while (scout_list.size() > 0)
    {
        PathFinder scout = scout_list.front();
        scout_list.pop_front();
        MazeSquare scout_square = scout.CheckSpot(min_score);
        if (scout_square == MazeSquare::Empty || scout_square == MazeSquare::Start)
        {
            std::cout << "NEW!:\n";
            deque<PathFinder> next_gen = scout.NextGen();
            scout_list.insert(scout_list.end(), next_gen.begin(), next_gen.end());
        }
        else if (scout_square == MazeSquare::End)
        {
            std::cout << "END!: " << scout.GetScore() << "\n";
            min_score = scout.GetScore();
        }
        else
        {

            std::cout << "DEAD!\n";
        }
    }

    std::cout << "Reindeer Score: " << min_score << "\n";
    return 0;
}