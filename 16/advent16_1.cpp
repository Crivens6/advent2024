#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using std::deque;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input_small.txt";
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
    Empty = 0,
    Wall,
    Start,
    End
};

class MazeNode;

vector<vector<int>> g_map_grid;
map<pair<int, int>, int> g_node_map;
vector<MazeNode> g_node_list;

class SimpleNode
{
private:
    int connected_nodes[3];
    int connection_score[3];
    int distance;

public:
    SimpleNode(int node_a_index, int node_a_dist, int node_b_index, int node_b_dist, int node_c_index, bool is_end = false)
    {
        std::cout << "    " << node_a_index << ", " << node_b_index << "\n";
        connected_nodes[0] = node_a_index;
        connection_score[0] = node_a_dist;
        connected_nodes[1] = node_b_index;
        connection_score[1] = node_b_dist;
        connected_nodes[2] = node_c_index;
        if (!is_end)
        {
            connection_score[2] = kTurnScore;
        }
        else
        {
            connection_score[2] = 0;
        }
        distance = -1;
    }
    int GetMinDistance()
    {
        return distance;
    }
    void SetMinDistance(int dist)
    {
        if (distance == -1 || distance > dist)
        {
            distance = dist;
        }
    }
    pair<int, int> GetNeighbor(int index)
    {
        return {connected_nodes[index], connection_score[index]};
    }
};

class MazeNode
{
private:
    int self_index;
    int connected_nodes[4];
    int connection_score[4];
    Direction connected_return_direction[4];
    int SimpleNodeIndex(int index)
    {
        if (connected_nodes[index] == -1)
        {
            return -1;
        }
        return (connected_nodes[index] * 2) + (((int)connected_return_direction[index] + 1) % 2);
    }

public:
    MazeNode(int index)
    {
        connected_nodes[0] = -1;
        connected_nodes[1] = -1;
        connected_nodes[2] = -1;
        connected_nodes[3] = -1;
        self_index = index;
    }
    MazeNode(int index, Direction node_dir, int other_node, int path_score, Direction other_node_return_dir)
    {
        connected_nodes[0] = -1;
        connected_nodes[1] = -1;
        connected_nodes[2] = -1;
        connected_nodes[3] = -1;
        self_index = index;
        connection_score[node_dir] = path_score;
        connected_nodes[node_dir] = other_node;
        connected_return_direction[node_dir] = other_node_return_dir;
    }

    void CreateConnection(Direction node_dir, int other_node, int path_score, Direction other_node_return_dir)
    {
        connection_score[node_dir] = path_score;
        connected_nodes[node_dir] = other_node;
        connected_return_direction[node_dir] = other_node_return_dir;
    }

    vector<SimpleNode> SplitToSimpleNodes()
    {
        std::cout << self_index << ": \n";
        std::cout << "    " << connected_nodes[1] << ", " << connected_nodes[3] << "\n"
                  << "    " << connected_nodes[0] << ", " << connected_nodes[2] << "\n    ----\n";
        SimpleNode horizontal = SimpleNode(SimpleNodeIndex(1), connection_score[1], SimpleNodeIndex(3), connection_score[3], (self_index * 2) + 1);
        SimpleNode vertical = SimpleNode(SimpleNodeIndex(0), connection_score[0], SimpleNodeIndex(2), connection_score[2], (self_index * 2));
        return {horizontal, vertical};
    }
    vector<SimpleNode> SplitEndNode()
    {
        std::cout << self_index << ": \n";
        std::cout << "    " << connected_nodes[1] << ", " << connected_nodes[3] << "\n"
                  << "    " << connected_nodes[0] << ", " << connected_nodes[2] << "\n    ----\n";
        SimpleNode horizontal = SimpleNode(SimpleNodeIndex(1), connection_score[1], SimpleNodeIndex(3), connection_score[3], (self_index * 2) + 1, true);
        SimpleNode vertical = SimpleNode(SimpleNodeIndex(0), connection_score[0], SimpleNodeIndex(2), connection_score[2], (self_index * 2), true);
        return {horizontal, vertical};
    }
    vector<SimpleNode> SplitStartNode()
    {
        std::cout << self_index << ": \n";
        std::cout << "    " << connected_nodes[1] << ", " << connected_nodes[3] << "\n"
                  << "    " << connected_nodes[0] << ", " << connected_nodes[2] << "\n    ----\n";
        SimpleNode horizontal = SimpleNode(SimpleNodeIndex(1), connection_score[1], SimpleNodeIndex(3), connection_score[3], (self_index * 2) + 1);
        SimpleNode vertical = SimpleNode(SimpleNodeIndex(0), connection_score[0], SimpleNodeIndex(2), connection_score[2], (self_index * 2));
        horizontal.SetMinDistance(0);
        return {horizontal, vertical};
    }
};
class PathFinder
{
private:
    int x;
    int y;
    int score;
    Direction departed_dir;
    Direction dir;
    int path_score;
    int parent_node;
    void SetSpot(pair<int, int> coord)
    {
        x = coord.first;
        y = coord.second;
    }
    pair<int, int> GetNextCoord(Direction next_dir)
    {
        return {x + kMoveDir[next_dir][0], y + kMoveDir[next_dir][1]};
    }

public:
    PathFinder(pair<int, int> coords, int parent_node, Direction dir, int starting_distance = 0)
    {
        this->x = coords.first;
        this->y = coords.second;
        this->parent_node = parent_node;
        this->dir = dir;
        this->departed_dir = dir;
        this->path_score = kMoveScore + starting_distance;
    }
    static bool CheckSpot(pair<int, int> coord)
    {
        int check_x = coord.first;
        int check_y = coord.second;
        if (0 > check_x || g_map_grid.size() <= check_x)
        {
            return false;
        }
        if (0 > check_y || g_map_grid[0].size() <= check_y)
        {
            return false;
        }
        return g_map_grid[check_x][check_y] != MazeSquare::Wall;
    }
    deque<PathFinder> Explore()
    {
        deque<PathFinder> new_paths;
        bool path_explored = false;
        while (!path_explored)
        {
            // Check current spot against known nodes
            if (g_node_map.find({x, y}) != g_node_map.end())
            {
                Direction return_dir = Direction(((int)dir + 2) % 4);
                // Create connection in new node
                g_node_list[g_node_map[{x, y}]].CreateConnection(return_dir, parent_node, path_score, departed_dir);

                // Give node information to parent node
                g_node_list[parent_node].CreateConnection(departed_dir, g_node_map[{x, y}], path_score, return_dir);

                // Mark path as done
                path_explored = true;
                break;
            }

            // Right
            Direction right_dir = Direction(((int)dir + 1) % 4);
            pair<int, int> right = GetNextCoord(right_dir);
            // Forward
            pair<int, int> forward = GetNextCoord(dir);
            // Left
            Direction left_dir = (Direction)(((int)dir + 3) % 4);
            pair<int, int> left = GetNextCoord(left_dir);

            int possible_steps = 0;
            if (CheckSpot(right))
            {
                possible_steps++;
            }
            if (CheckSpot(forward))
            {
                possible_steps++;
            }
            if (CheckSpot(left))
            {
                possible_steps++;
            }

            // New Node
            if (possible_steps > 1)
            {
                Direction return_dir = Direction(((int)dir + 2) % 4);
                // Create New Node
                int new_node_index = g_node_list.size();
                MazeNode new_node(new_node_index, return_dir, parent_node, path_score, departed_dir);
                g_node_list.push_back(new_node);
                g_node_map[{x, y}] = new_node_index;

                // Give node information to parent node
                g_node_list[parent_node].CreateConnection(departed_dir, new_node_index, path_score, return_dir);

                // Create new pathfinders
                if (CheckSpot(right))
                {
                    new_paths.push_back(PathFinder(right, new_node_index, right_dir));
                }
                if (CheckSpot(forward))
                {
                    new_paths.push_back(PathFinder(forward, new_node_index, dir));
                }
                if (CheckSpot(left))
                {
                    new_paths.push_back(PathFinder(left, new_node_index, left_dir));
                }
                // Mark path as done
                path_explored = true;
            }
            // Continue Path
            else if (possible_steps == 1)
            {
                // Go to next spot
                if (CheckSpot(right))
                {
                    SetSpot(right);
                    dir = right_dir;
                    path_score += kMoveScore + kTurnScore;
                }
                if (CheckSpot(forward))
                {
                    SetSpot(forward);
                    path_score += kMoveScore;
                }
                if (CheckSpot(left))
                {
                    SetSpot(left);
                    dir = left_dir;
                    path_score += kMoveScore + kTurnScore;
                }
            }
            // Dead End
            else
            {
                // Mark path as done
                path_explored = true;
            }
        }
        return new_paths;
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
    pair<int, int> end;

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
                end = {g_map_grid.size(), grid_row.size()};
                grid_row.push_back(MazeSquare::End);
            }
        }
        // Add the vector row to the grid
        g_map_grid.push_back(grid_row);
    }
    input_file.close();

    // Create  start and end nodes
    g_node_list.push_back(MazeNode(0));
    g_node_list.push_back(MazeNode(1));
    g_node_map[start] = 0;
    g_node_map[end] = 1;

    // Set inital paths from start
    deque<PathFinder> path_list;
    for (int dir = 0; dir < 4; dir++)
    {
        pair<int, int> path_start = {start.first + kMoveDir[dir][0], start.second + kMoveDir[dir][1]};
        if (PathFinder::CheckSpot(path_start))
        {
            path_list.push_back(PathFinder(path_start, 0, (Direction)dir, (dir == Direction::W) ? kTurnScore : 0));
        }
    }

    // Explore all paths and make a node map
    while (path_list.size() > 0)
    {
        PathFinder path = path_list.front();
        path_list.pop_front();
        deque<PathFinder> new_paths = path.Explore();
        path_list.insert(path_list.end(), new_paths.begin(), new_paths.end());
    }
    std::cout << "Node Map Compleate: " << g_node_list.size() << "\n";

    // Split the maze nodes into simple nodes
    vector<SimpleNode> simple_nodes;
    for (int maze_node = 0; maze_node < g_node_list.size(); maze_node++)
    {
        vector<SimpleNode> new_nodes;
        if (maze_node == 0)
        {
            new_nodes = g_node_list[maze_node].SplitStartNode();
        }
        else if (maze_node == 1)
        {
            new_nodes = g_node_list[maze_node].SplitEndNode();
        }
        else
        {
            new_nodes = g_node_list[maze_node].SplitToSimpleNodes();
        }
        simple_nodes.insert(simple_nodes.end(), new_nodes.begin(), new_nodes.end());
    }

    // Run Dijkstra on simple nodes
    int end_node = 2;
    set<int> visited_nodes;
    int count = 0;
    while (visited_nodes.find(end_node) == visited_nodes.end())
    {
        count++;
        // Find min
        int min_index = -1;
        int min_distance = -1;
        for (int i = 0; i < simple_nodes.size(); i++)
        {
            if (visited_nodes.find(i) == visited_nodes.end() && simple_nodes[i].GetMinDistance() != -1)
            {
                if (min_index == -1 || simple_nodes[i].GetMinDistance() < min_distance)
                {
                    std::cout << i << " Picked: " << simple_nodes[i].GetMinDistance() << " < " << min_distance << "\n";
                    min_index = i;
                    min_distance = simple_nodes[i].GetMinDistance();
                }
            }
        }
        if (min_index == -1)
        {
            break;
        }
        std::cout << min_index << ": " << min_distance << "\n";
        visited_nodes.insert(min_index);
        // Visit Neighbors
        for (int i = 0; i < 3; i++)
        {
            pair<int, int> neighbor = simple_nodes[min_index].GetNeighbor(i);
            if (neighbor.first != -1 && visited_nodes.find(neighbor.first) == visited_nodes.end())
            {
                std::cout << "    " << neighbor.first << ", " << neighbor.second << "\n";
                simple_nodes[neighbor.first].SetMinDistance(min_distance + neighbor.second);
            }
        }
    }

    std::cout << "Min Score: " << simple_nodes[end_node].GetMinDistance() << "\n";

    return 0;
}