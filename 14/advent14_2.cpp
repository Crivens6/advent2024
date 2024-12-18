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
const int kPrintLimit = 1;
const int kRoomWidth = 101;
const int kRoomHeight = 103;

class Robot
{
private:
    int start_x;
    int start_y;
    int vel_x;
    int vel_y;

public:
    Robot(int x, int y, int vx, int vy)
    {
        start_x = x;
        start_y = y;
        vel_x = vx;
        vel_y = vy;
    }
    pair<int, int> Simulate(int sec)
    {
        // Simulate to find endpoint
        int end_x = (start_x + vel_x * sec) % kRoomWidth;
        if (end_x < 0)
        {
            end_x += kRoomWidth;
        }
        int end_y = (start_y + vel_y * sec) % kRoomHeight;
        if (end_y < 0)
        {
            end_y += kRoomHeight;
        }
        return {end_x, end_y};
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

    string input_line;
    vector<Robot> robo_list;
    while (getline(input_file, input_line))
    {
        // Get robot data
        size_t num_length = 0;
        int string_pos = 2;
        int start_x = std::stoi(input_line.substr(string_pos), &num_length);
        string_pos += num_length + 1;
        int start_y = std::stoi(input_line.substr(string_pos), &num_length);
        string_pos += num_length + 3;
        int vel_x = std::stoi(input_line.substr(string_pos), &num_length);
        string_pos += num_length + 1;
        int vel_y = std::stoi(input_line.substr(string_pos), &num_length);

        robo_list.push_back(Robot(start_x, start_y, vel_x, vel_y));
    }
    input_file.close();
    int print_count = 0;
    int t = 0;
    while (print_count < kPrintLimit)
    {
        // Get all robot positions at time t
        set<pair<int, int>> robo_positions;
        for (Robot bot : robo_list)
        {
            robo_positions.insert(bot.Simulate(t));
        }

        bool print_flag = false;
        // Trigger a print if a triangle is found
        for (int y = 0; y < kRoomHeight - 3; y++)
        {
            for (int x = 3; x < kRoomWidth - 3; x++)
            {
                if (robo_positions.find({x, y}) != robo_positions.end())
                {
                    bool l1 = robo_positions.find({x - 1, y + 1}) != robo_positions.end();
                    bool l2 = robo_positions.find({x - 2, y + 2}) != robo_positions.end();
                    bool l3 = robo_positions.find({x - 3, y + 3}) != robo_positions.end();
                    bool r1 = robo_positions.find({x + 1, y + 1}) != robo_positions.end();
                    bool r2 = robo_positions.find({x + 2, y + 2}) != robo_positions.end();
                    bool r3 = robo_positions.find({x + 3, y + 3}) != robo_positions.end();
                    if (l1 && l2 && l3 && r1 && r2 && r3)
                    {
                        print_flag = true;
                        print_count++;
                    }
                }
            }
        }
        // Print the board if triggered
        if (print_flag)
        {
            std::cout << "T:" << t << "\n";
            for (int y = 0; y < kRoomHeight; y++)
            {
                for (int x = 0; x < kRoomWidth; x++)
                {
                    if (robo_positions.find({x, y}) == robo_positions.end())
                    {
                        std::cout << " ";
                    }
                    else
                    {
                        std::cout << "O";
                    }
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
        t++;
    }
    return 0;
}