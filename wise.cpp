#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <limits>

class Item {
public:
    int id;
    int quantity;
    std::string name;
    std::string date;

    Item(int id, int quantity, const std::string& name, const std::string& date)
        : id(id), quantity(quantity), name(name), date(date) {}
};

std::vector<Item> loadItemsFromCSV(const std::string& filename) {
    std::vector<Item> items;
    std::ifstream file(filename);
    std::string line, word;

    if (file.is_open()) {
        while (getline(file, line)) {
            std::stringstream ss(line);
            std::vector<std::string> row;
            while (getline(ss, word, ',')) {
                row.push_back(word);
            }
            if (row.size() == 4) {
                Item item(stoi(row[0]), stoi(row[1]), row[2], row[3]);
                items.push_back(item);
            }
        }
        file.close();
    }
    return items;
}

void saveItemToCSV(const std::string& filename, const Item& item) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << item.id << "," << item.quantity << "," << item.name << "," << item.date << "\n";
        file.close();
    }
}

bool validateDate(const std::string& date) {
    const std::regex datePattern(R"(^\d{4}-\d{2}-\d{2}$)");
    return std::regex_match(date, datePattern);
}

bool validateQuantity(int quantity) {
    return quantity >= 0;
}

bool validateName(const std::string& name) {
    const std::regex namePattern(R"(^[A-Za-z0-9 ]+$)");
    return !name.empty() && std::regex_match(name, namePattern);
}

bool validateInput(int id, int quantity, const std::string& name, const std::string& date) {
    if (id < 0 || !validateQuantity(quantity) || !validateName(name) || !validateDate(date)) {
        return false;
    }
    return true;
}

void showHelp() {
    std::cout << "Commands:\n";
    std::cout << "additem - Add a new item\n";
    std::cout << "listitems - List all items\n";
    std::cout << "help - Show this help message\n";
    std::cout << "exit - Exit the program\n";
}

void listItems(const std::vector<Item>& items) {
    for (const auto& item : items) {
        std::cout << "ID: " << item.id << ", Quantity: " << item.quantity << ", Name: " << item.name << ", Date: " << item.date << "\n";
    }
}

int main() {
    std::vector<Item> items = loadItemsFromCSV("items.csv");
    std::string command;
    showHelp();

    while (true) {
        std::cout << "\nEnter command: ";
        std::cin >> command;

        if (command == "additem") {
            int id, quantity;
            std::string name, date;

            std::cout << "Enter ID: ";
            std::cin >> id;

            std::cout << "Enter quantity: ";
            while (!(std::cin >> quantity) || !validateQuantity(quantity)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid quantity. Enter a non-negative integer: ";
            }

            std::cin.ignore();  // Ignore the newline character
            std::cout << "Enter name: ";
            std::getline(std::cin, name);
            while (!validateName(name)) {
                std::cout << "Invalid name. Enter a valid name (alphanumeric and spaces only): ";
                std::getline(std::cin, name);
            }

            std::cout << "Enter date (YYYY-MM-DD): ";
            std::getline(std::cin, date);
            while (!validateDate(date)) {
                std::cout << "Invalid date format. Enter date (YYYY-MM-DD): ";
                std::getline(std::cin, date);
            }

            if (validateInput(id, quantity, name, date)) {
                Item newItem(id, quantity, name, date);
                saveItemToCSV("items.csv", newItem);
                items.push_back(newItem);
                std::cout << "Item added successfully.\n";
            } else {
                std::cout << "Invalid input. Please try again.\n";
            }
        } else if (command == "listitems") {
            listItems(items);
        } else if (command == "help") {
            showHelp();
        } else if (command == "exit") {
            break;
        } else {
            std::cout << "Unknown command. Type 'help' for a list of commands.\n";
        }
    }

    return 0;
}
