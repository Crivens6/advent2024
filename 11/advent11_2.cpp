#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::pair;
using std::string;
using std::vector;

// Constants
const string kInputFilePath = "input.txt";
const int kTotalBlinks = 75;

class Rock;

map<long, int> g_rock_map;
vector<Rock> g_rock_list;

class Rock
{
public:
    long value;
    vector<int> decendents_index;
    vector<long> size_history;
    int age;

    // Initalize rock with a value
    Rock(long value)
    {
        this->value = value;
        this->decendents_index = {};
        this->size_history.push_back(1);
        this->age = 0;
    }

    // Special Rock created for inital set of rocks
    Rock(vector<long> rock_set)
    {
        this->value = -1;
        this->decendents_index = {};
        for (long rock : rock_set)
        {
            MakeDecendentRock(rock);
        }
        this->age = 0;
        CountDecendents();
        this->age++;
    }

    // Return the size of rock's family at age
    long Size(int age_inquiry)
    {
        if (age_inquiry > age || age_inquiry < 0)
        {
            std::cerr << value << " OUT OF RANGE AT AGE " << age_inquiry << "\n";
            return -1;
        }
        return size_history[age_inquiry];
    }
    // Return the current full sized family
    long SizeFull()
    {
        return Size(this->age - 1);
    }

    // Count the next itereation of rock family size
    void Blink()
    {
        if (this->age == 0 && this->value != -1)
        {
            string value_string = std::to_string(value);
            if (value == 0)
            {
                MakeDecendentRock(1);
            }
            // Fill Empty Block
            else if (value_string.size() % 2 == 0)
            {
                int half_length = value_string.size() / 2;
                long valueA = std::stol(value_string.substr(0, half_length));
                long valueB = std::stol(value_string.substr(half_length));
                MakeDecendentRock(valueA);
                MakeDecendentRock(valueB);
            }
            else
            {
                MakeDecendentRock(value * 2024);
            }
        }
        CountDecendents();
        this->age++;
    }
    // Get the current age of the rock
    int GetAge()
    {
        return this->age;
    }

private:
    // Create a new decendent rock
    void MakeDecendentRock(long new_value)
    {
        // A new value
        if (g_rock_map.find(new_value) == g_rock_map.end())
        {
            int new_index = g_rock_list.size();
            decendents_index.push_back(new_index);
            g_rock_map[new_value] = new_index;
            g_rock_list.push_back(Rock(new_value));
        }
        // An existing rock
        else
        {
            decendents_index.push_back(g_rock_map[new_value]);
        }
    }
    void CountDecendents()
    {
        long family_size = 0;
        for (int i = 0; i < decendents_index.size(); i++)
        {
            // Get the family size of the desendent at one less blink than current rock
            family_size += g_rock_list[decendents_index[i]].Size(this->age);
        }
        // Store the new family size at this rock's age
        size_history.push_back(family_size);
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

    // Iterate through each line and get the inital rock values
    vector<long> start_list;
    string input_line;
    while (getline(input_file, input_line))
    {
        int scan_index = 0;
        size_t end_of_num = 0;

        // Loop through each number until the end of the line
        while (scan_index < input_line.size())
        {
            // Add value to list for starting rock set
            start_list.push_back(std::stol(input_line.substr(scan_index), &end_of_num));

            // Increment over the number and space
            scan_index += end_of_num + 1;
        }
    }

    // Create the starting rock set
    Rock start_rock = Rock(start_list);
    std::cout << "Blink #0 Stone Count: " << start_rock.Size(0) << "\n";

    // Blink kTotalBlinks times
    for (int blink_count = 0; blink_count < kTotalBlinks; blink_count++)
    {
        // Go through each rock and calculate a new size after blinking
        for (int target_rock = g_rock_list.size() - 1; target_rock >= 0; target_rock--)
        {
            Rock this_rock = g_rock_list[target_rock];
            this_rock.Blink();
            g_rock_list[target_rock] = this_rock;
        }
        // Get the size of the rock set
        start_rock.Blink();
        std::cout << "Blink #" << (blink_count + 1) << " Stone Count: " << start_rock.SizeFull() << "\n";
    }

    input_file.close();
    return 0;
}