#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

int main(int argc, char *argv[]) {

/* FILE INPUT */

    std::string input_file_name = "../input.txt";
    std::string source;
    // The first element denotes the input file.
    // If no first element exists, then the default is "input.txt".
    if(argc < 2) {
        std::cout << "Using input file ../input.txt as default." << std::endl;
    }
    else {
        std::cout << "Using input file " << (input_file_name = argv[1]) << '.' << std::endl;
    }

    {   /*  */
        std::ifstream in(input_file_name);
        // If there is no input file with such name, exit the program.
        if(!in) {
            std::cerr << "Couldn't find file " << input_file_name << "!" << std::endl;
            return 1;
        }
        // Saves the input into source.
        in.seekg(0, std::ios::end);
        source.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&source[0], source.size());
        in.close();
    }
/* FILE INPUT END */

    std::cout << source << std::endl;
}