#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

int main () {
    std::string name;
    std::string age;

    std::cout <<"Enter your name: ";
    std::cin >> name;
    std::cout << "Enter your age: ";
    std::cin >> age;

    std::ofstream file;
    file.open("data.txt");
    file << name << std::endl;
    file << age << std::endl;
    file.close();

    std::ifstream file2;
    file2.open("data.txt");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file2, line)) {
        lines.push_back(line);
    }
    file2.close();

    std::cout << "Name: " << lines[0] << std::endl;
    std::cout << "Age: " << lines[1] << std::endl;

    return 0;
}