#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>

using std::list;
using std::pair;
using std::set;
using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input_test.txt";
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

class Garden
{
private:
    vector<vector<char>> plot_grid;

public:
    void AddRow(vector<char> plot_row)
    {
        plot_grid.push_back(plot_row);
    }
    int CalculateFencingCost()
    {
        int cost = 0;
        set<pair<int, int>> accounted_plots;
        for (int row = 0; row < plot_grid.size(); row++)
        {
            for (int col = 0; col < plot_grid[0].size(); col++)
            {
                if (accounted_plots.find({row, col}) == accounted_plots.end())
                {
                    pair<int, set<pair<int, int>>> plot_data = CalculatePlot(row, col, accounted_plots);
                    cost += plot_data.first;
                    accounted_plots.merge(plot_data.second);
                }
            }
        }
        return cost;
    }
    // Returns a pair of plot cost / set of coordinates in plot
    pair<int, set<pair<int, int>>> CalculatePlot(int start_row, int start_col, set<pair<int, int>> accounted_plots)
    {

        char plot_char = plot_grid[start_row][start_col];
        int area = 1;

        set<pair<int, int>> newly_accounted_plots;
        list<pair<int, int>> plots_to_analize;
        // Start coord / end coord
        set<pair<pair<int, int>, pair<int, int>>> plot_edges;
        newly_accounted_plots.insert({start_row, start_col});
        plots_to_analize.push_back({start_row, start_col});
        while (plots_to_analize.size() > 0)
        {
            pair<int, int> center_plot = plots_to_analize.front();
            plots_to_analize.pop_front();
            for (int dir = Direction::N; dir <= Direction::W; dir++)
            {
                pair<int, int> adjacent_plot = {center_plot.first + kDirectionCoor[dir][0],
                                                center_plot.second + kDirectionCoor[dir][1]};
                // Check that plot is on the grid
                if (0 > adjacent_plot.first || adjacent_plot.first >= plot_grid.size() ||
                    0 > adjacent_plot.second || adjacent_plot.second >= plot_grid[0].size())
                {
                    plot_edges.insert(GetSideCoords(center_plot, dir));
                    continue;
                }
                // Check that plot not already found
                if (newly_accounted_plots.find(adjacent_plot) == newly_accounted_plots.end())
                {
                    // Check that plot contains correct character
                    if (plot_grid[adjacent_plot.first][adjacent_plot.second] == plot_char)
                    {
                        area++;
                        plots_to_analize.push_back(adjacent_plot);
                        newly_accounted_plots.insert(adjacent_plot);
                    }
                    // If not correct char, add to perimeter
                    else
                    {
                        plot_edges.insert(GetSideCoords(center_plot, dir));
                    }
                }
            }
        }

        int side_count = 0;
        while (plot_edges.size() > 0)
        {
            pair<pair<int, int>, pair<int, int>> first_side_section = *plot_edges.begin();
            pair<pair<int, int>, pair<int, int>> last_side_section = first_side_section;
            plot_edges.erase(plot_edges.begin());
            int dirCoor[2];
            if (first_side_section.first.first == first_side_section.second.first)
            {
                dirCoor[0] = kDirectionCoor[Direction::W][0];
                dirCoor[1] = kDirectionCoor[Direction::W][1];
            }
            else
            {
                dirCoor[0] = kDirectionCoor[Direction::N][0];
                dirCoor[1] = kDirectionCoor[Direction::N][1];
            }

            // Check N / W for end of wall
            bool side_searching_flag = true;
            while (side_searching_flag)
            {
                pair<int, int> new_side_point = {first_side_section.first.first + dirCoor[0], first_side_section.first.second + dirCoor[1]};
                pair<pair<int, int>, pair<int, int>> prior_side_section = {new_side_point, first_side_section.first};
                // Check if reached end of side
                if (plot_edges.find(prior_side_section) == plot_edges.end())
                {
                    side_searching_flag = false;
                }
                // Move to new endpoint and remove it from the set
                else
                {
                    first_side_section = prior_side_section;
                    plot_edges.erase(plot_edges.find(prior_side_section));
                }
            }
            // Check S / E for end of wall
            side_searching_flag = true;
            while (side_searching_flag)
            {
                pair<int, int> new_side_point = {last_side_section.second.first - dirCoor[0], last_side_section.second.second - dirCoor[1]};
                pair<pair<int, int>, pair<int, int>> next_side_section = {last_side_section.second, new_side_point};
                // Check if reached end of side
                if (plot_edges.find(next_side_section) == plot_edges.end())
                {
                    side_searching_flag = false;
                }
                // Move to new endpoint and remove it from the set
                else
                {
                    last_side_section = next_side_section;
                    plot_edges.erase(plot_edges.find(next_side_section));
                }
            }

            // Count side removed from set
            side_count++;
        }

        int cost = area * side_count;
        std::cout << plot_char << ": " << area << "*" << side_count << "=" << cost << "\n";
        return {cost, newly_accounted_plots};
    }
    // Return in Top/Down or Left/Right format
    pair<pair<int, int>, pair<int, int>> GetSideCoords(pair<int, int> plot_coords, int dir)
    {
        int row = plot_coords.first;
        int col = plot_coords.second;
        if (dir == Direction::N)
        {
            return {{row, col}, {row, col + 1}};
        }
        else if (dir == Direction::S)
        {
            return {{row + 1, col}, {row + 1, col + 1}};
        }
        else if (dir == Direction::E)
        {
            return {{row, col + 1}, {row + 1, col + 1}};
        }
        else
        {
            return {{row, col}, {row + 1, col}};
        }
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

    // Iterate through each line and create a rock for each value
    Garden garden;
    string input_line;
    while (getline(input_file, input_line))
    {
        vector<char> plot_row;
        for (char plot_char : input_line)
        {
            plot_row.push_back(plot_char);
        }
        garden.AddRow(plot_row);
    }

    int cost = garden.CalculateFencingCost();

    std::cout << "Fencing Cost: " << cost << "\n";

    input_file.close();
    return 0;
}