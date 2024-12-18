#include <fstream>
#include <iostream>
#include <string>

using std::string;

// Constants
const string kInputFilePath = "input.txt";
const int kSimulationTime = 100;
const int kRoomWidth = 101;
const int kRoomHeight = 103;

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
    int quadrant_count[4] = {0, 0, 0, 0};
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

        // Simulate to find endpoint
        int end_x = (start_x + vel_x * kSimulationTime) % kRoomWidth;
        if (end_x < 0)
        {
            end_x += kRoomWidth;
        }
        int end_y = (start_y + vel_y * kSimulationTime) % kRoomHeight;
        if (end_y < 0)
        {
            end_y += kRoomHeight;
        }

        // Ignor if robot ends in middle
        if (end_x == kRoomWidth / 2 || end_y == kRoomHeight / 2)
        {
            continue;
        }
        int quad = 0;
        // Select left or right half
        quad += end_x < kRoomWidth / 2 ? 0 : 1;
        // Select top or bottom half
        quad += end_y < kRoomHeight / 2 ? 0 : 2;
        // Increment aproprate quad
        quadrant_count[quad]++;
    }
    input_file.close();

    int saftey_factor = quadrant_count[0] * quadrant_count[1] * quadrant_count[2] * quadrant_count[3];

    std::cout << "Safety Factor: " << saftey_factor << "\n";
    return 0;
}