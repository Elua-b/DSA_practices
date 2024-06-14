#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cctype>

using namespace std;

struct Item {
    int item_id;
    string item_name;
    int item_quantity;
    string item_registration_date;
};

bool compareItems(const Item& item1, const Item& item2) {
    // Convert item names to lowercase for case-insensitive comparison
    string item1Name = item1.item_name;
    string item2Name = item2.item_name;
    transform(item1Name.begin(), item1Name.end(), item1Name.begin(), [](unsigned char c) { return std::tolower(c); });
    transform(item2Name.begin(), item2Name.end(), item2Name.begin(), [](unsigned char c) { return std::tolower(c); });

    return item1Name < item2Name;
}

bool isInteger(const std::string& input) {
    std::istringstream iss(input);
    int value;
    return (iss >> value) && (iss.eof());
}

bool isString(const std::string& input) {
    for (char c : input) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}

bool isDate(const std::string& input) {
    std::istringstream iss(input);
    int year, month, day;
    char delimiter;
    if (iss >> year >> delimiter >> month >> delimiter >> day) {
        // Check if the stream is valid and there are no extra characters
        return (iss.eof() && (delimiter == '-') && (month >= 1 && month <= 12) && (day >= 1 && day <= 31));
    }
    return false;
}



void addItem(int item_id, const string& item_name, int item_quantity, const string& item_registration_date) {
    ifstream file("items.csv");
    string line;

    if (file.is_open()) {
        // Check if item_id already exists in the CSV file
        while (getline(file, line)) {
            stringstream ss(line);
            string token;

            getline(ss, token, ',');
            int existing_item_id = std::stoi(token);

            if (existing_item_id == item_id) {
                cout << "Item ID already exists!\n";
                file.close();
                return;
            }
        }

        file.close(); // Close the file

        // Open the CSV file in append mode
        ofstream outputFile("items.csv", ios::app);

        if (outputFile.is_open()) {
            Item item;
            item.item_id = item_id;
            item.item_name = item_name;
            item.item_quantity = item_quantity;
            item.item_registration_date = item_registration_date;

            // Write the item details to the CSV file
            outputFile << item.item_id << "," << item.item_name << "," << item.item_quantity << "," << item.item_registration_date << "\n";

            outputFile.close(); // Close the file
            cout << "Item added successfully!\n";
        } else {
            cout << "Unable to open file for writing!\n";
        }
    } else {
        cout << "Unable to open file for reading!\n";
    }
}



void listItems() {
    ifstream file("items.csv");

    if (file.is_open()) {
        string line;
        vector<Item> items;

        // Read each line from the CSV file
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            Item item;

            // Extract item details from the CSV line
            getline(ss, token, ',');
            item.item_id = std::stoi(token);
            getline(ss, item.item_name, ',');
            getline(ss, token, ',');
            item.item_quantity = stoi(token);
            getline(ss, item.item_registration_date, ',');

            items.push_back(item);
        }

        // Sort items based on item_name in case-insensitive ascending order
        sort(items.begin(), items.end(), compareItems);

        // Display the sorted items on the console
        for (const auto& item : items) {
            cout << "Item ID: " << item.item_id << "\n";
            cout << "Item Name: " << item.item_name << "\n";
            cout << "Quantity: " << item.item_quantity << "\n";
            cout << "Reg Date: " << item.item_registration_date << "\n";
            cout << "----------------------\n";
        }

        file.close(); // Close the file
    } else {
        cout << "Unable to open file!\n";
    }
}




int help() {
    std::string command;
    std::string* splitedCommands = nullptr;
    int commandCount = 0;

    do {
        std::cout << "RIS >";
        std::getline(std::cin, command);

        std::string commandCopy = command;
        std::string space_delimiter = " ";

        // Release memory from previous command if any
        if (splitedCommands != nullptr) {
            delete[] splitedCommands;
            splitedCommands = nullptr;
        }

        size_t pos = 0;
        while ((pos = commandCopy.find(space_delimiter)) != std::string::npos) {
            commandCount++;
            commandCopy.erase(0, pos + space_delimiter.length());
        }
        commandCount++; // Count the last token

        // Allocate memory for the dynamic array
        splitedCommands = new std::string[commandCount];
        
        for_each(splitedCommands[0].begin(), splitedCommands[0].end(), [](char& c) {
            c = ::tolower(c);
        });

        pos = 0;
        int index = 0;
        while ((pos = command.find(space_delimiter)) != std::string::npos) {
            splitedCommands[index++] = command.substr(0, pos);
            command.erase(0, pos + space_delimiter.length());
        }
        splitedCommands[index] = command;

        if (splitedCommands[0].compare("itemadd") == 0) {
        	if (!isInteger(splitedCommands[1])) {
        		std::cout << "Item ID should be an integer!\n";
			} else if (!isString(splitedCommands[2])) {
				std::cout << "Item Name should be a string!\n";
			} else if (!isInteger(splitedCommands[3])) {
				std::cout << "Quantity should be an integer!\n";
			} else if (!isDate(splitedCommands[4])) {
				std::cout << "Registration date should be in date format (YYYY-MM-DD)!\n";
			} else {
				addItem(stoi(splitedCommands[1]), splitedCommands[2], stoi(splitedCommands[3]), splitedCommands[4]);
			}
        } else if (splitedCommands[0].compare("itemslist") == 0) {
            listItems();
        } else if (splitedCommands[0].compare("help") == 0) {
            std::cout << "-----------------------------------------\n";
            std::cout << "*               Commands syntaxes        *\n";
            std::cout << "-----------------------------------------\n";
            std::cout << "itemadd <item_id> <item_name> <quantity> <registration_date>    :Adding an item to the file\n";
            std::cout << "itemslist                                                       :Listing items in the file\n";
            std::cout << "exit                                                            :exits the program>\n";
            std::cout << "help                                                            :Prints user manual\n";
        } else if (splitedCommands[0].compare("exit") == 0) {
            delete[] splitedCommands; // Release memory before exiting
            return 0;
        } else {
            std::cout << "Invalid command!" << std::endl;
        }

    } while (command.compare("exit") != 0);

    delete[] splitedCommands; // Release memory before returning
    return 0;
}

int main() {
	system("color F1");
	cout << "-----------------------------------------" <<endl;
    cout << "     Welcome to RCA Inventory system!    "<<endl;
    cout << "  Type 'help' and press enter for help.  "<< endl;
    
    help();
 
    cout << "-----------------------------------------"<<endl;
    cout << "    Thank you for using the system!      "<<endl;
    cout << "-----------------------------------------"<<endl;
    return 0;
}