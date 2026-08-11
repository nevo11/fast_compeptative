#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Passed 'file' by reference instead of using a dangerous pointer
bool check(const string& line, string& file) {
    std::istringstream iss(line);
    std::string word;
    bool inc = false;
    
    while (iss >> word) {
        if (inc && word.starts_with('"') && word.ends_with('"')) {
            // Strip the quotes from the filename so filesystem can find it
            file = word.substr(1, word.length() - 2);
            return false;
        }
        if (word == "#include") {
            inc = true;
        }
    }
    return true;
}

void get_lines(const string& file_name) {
    std::filesystem::path p("../" + file_name);
    if (std::filesystem::exists(p)) {
        std::ifstream file(p);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << file_name << std::endl;
            return;
        }

        std::string line;
        bool start = false;
        
        // Read the file line by line until the end
        while (std::getline(file, line)) {
            if (start) {
                std::istringstream iss(line);
                std::string word;
                if (iss >> word) {
                    if (word != "#endif") {
                        // Added newline
                        cout << line << '\n'; 
                    }
                } else {
                    // Print empty lines as well to maintain formatting
                    cout << '\n';
                }
            } else {
                std::istringstream iss(line);
                std::string word;
                while (iss >> word) {
                    if (word == "#define") {
                        start = true;
                    }
                }
            }
        }
    } else {
        std::cerr << "Included file does not exist: " << file_name << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Please specify file name" << std::endl;
        return 1;
    }

    std::string choice = argv[1];
    std::filesystem::path p(choice);
    
    if (std::filesystem::exists(p)) {
        std::ifstream file(p);
        if (!file.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            return 1;
        }

        std::string line;
        std::string included_file; // properly initialized string
        
        while (std::getline(file, line)) {
            // Check now uses references
            if (check(line, included_file)) {
                cout << line << '\n'; // Added newline
            } else {
                get_lines(included_file);
            }
        }

        file.close();
    } else {
        std::cout << "File does not exist: " << choice << std::endl;
        return 1;
    }

    return 0;
}