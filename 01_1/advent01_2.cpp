#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

using std::string;

// Constants
const int kListLength = 1000;
const string kInputFilePath = "input.txt";
const string kIntegers = "0123456789";

int main()
{
    // Open the input file
    std::ifstream input_file(kInputFilePath);
    if (!input_file.is_open())
    {
        std::cerr << "File " << kInputFilePath << " was not found.\n";
        return 1;
    }

    // Instanciate lists to hold both sets of IDs
    int list_a[kListLength];
    int list_b[kListLength];

    // Itterate through each line and grab the IDs
    int file_line_number = 0;
    string file_line_string;
    string location_ID_a;
    string location_ID_b;
    while (getline(input_file, file_line_string))
    {
        // Separate the location IDs
        size_t first_ID_end = file_line_string.find(" ");
        size_t second_ID_start = file_line_string.find_first_of(kIntegers, first_ID_end + 1);
        location_ID_a = file_line_string.substr(0, first_ID_end);
        location_ID_b = file_line_string.substr(second_ID_start);

        // Add the location IDs to their respective lists
        list_a[file_line_number] = std::stoi(location_ID_a);
        list_b[file_line_number] = std::stoi(location_ID_b);

        file_line_number++;
    }

    // Sort both lists
    std::sort(std::begin(list_a), std::end(list_a));
    std::sort(std::begin(list_b), std::end(list_b));

    // Calculate the sum total difference
    int similarity_score = 0;
    int index_min_b = 0;

    for (int index_a = 0; index_a < kListLength; index_a++)
    {
        for (int index_b = index_min_b; list_a[index_a] >= list_b[index_b] && index_b < kListLength; index_b++)
        {
            if (list_a[index_a] > list_b[index_b])
            {
                index_min_b = index_b;
            }
            else
            {
                similarity_score += list_a[index_a];
            }
        }
    }

    std::cout << "Similarity Score: " << similarity_score << "\n";

    input_file.close();
    return 0;
}