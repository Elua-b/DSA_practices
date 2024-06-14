
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <limits>

class Employee {
public:
    int id;
    std::string name;
    int age;
    std::string department;

    Employee(int id, const std::string& name, int age, const std::string& department)
        : id(id), name(name), age(age), department(department) {}
};

std::vector<Employee> loadEmployeesFromCSV(const std::string& filename) {
    std::vector<Employee> employees;
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
                Employee employee(stoi(row[0]), row[1], stoi(row[2]), row[3]);
                employees.push_back(employee);
            }
        }
        file.close();
    }
    return employees;
}

void saveEmployeeToCSV(const std::string& filename, const Employee& employee) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << employee.id << "," << employee.name << "," << employee.age << "," << employee.department << "\n";
        file.close();
    }
}

bool validateName(const std::string& name) {
    const std::regex namePattern(R"(^[A-Za-z ]+$)");
    return !name.empty() && std::regex_match(name, namePattern);
}

bool validateAge(int age) {
    return age >= 18 && age <= 65; // assuming valid employee age range
}

bool validateInput(int id, const std::string& name, int age, const std::string& department) {
    return id >= 0 && validateName(name) && validateAge(age) && !department.empty();
}

void showHelp() {
    std::cout << "Commands:\n";
    std::cout << "addemployee - Add a new employee\n";
    std::cout << "listemployees - List all employees\n";
    std::cout << "updateemployee - Update an employee's details\n";
    std::cout << "removeemployee - Remove an employee\n";
    std::cout << "help - Show this help message\n";
    std::cout << "exit - Exit the program\n";
}

void listEmployees(const std::vector<Employee>& employees) {
    for (const auto& employee : employees) {
        std::cout << "ID: " << employee.id << ", Name: " << employee.name << ", Age: " << employee.age << ", Department: " << employee.department << "\n";
    }
}

void updateEmployee(std::vector<Employee>& employees) {
    int id;
    std::cout << "Enter the ID of the employee to update: ";
    std::cin >> id;
    
    for (auto& employee : employees) {
        if (employee.id == id) {
            std::string name, department;
            int age;

            std::cin.ignore(); // Ignore newline character
            std::cout << "Enter new name: ";
            std::getline(std::cin, name);
            while (!validateName(name)) {
                std::cout << "Invalid name. Enter a valid name (alphabetical characters only): ";
                std::getline(std::cin, name);
            }

            std::cout << "Enter new age: ";
            while (!(std::cin >> age) || !validateAge(age)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid age. Enter a valid age (18-65): ";
            }

            std::cin.ignore(); // Ignore newline character
            std::cout << "Enter new department: ";
            std::getline(std::cin, department);
            while (department.empty()) {
                std::cout << "Department cannot be empty. Enter department: ";
                std::getline(std::cin, department);
            }

            employee.name = name;
            employee.age = age;
            employee.department = department;

            std::ofstream file("employees.csv");
            for (const auto& emp : employees) {
                saveEmployeeToCSV("employees.csv", emp);
            }

            std::cout << "Employee updated successfully.\n";
            return;
        }
    }
    std::cout << "Employee not found.\n";
}

void removeEmployee(std::vector<Employee>& employees) {
    int id;
    std::cout << "Enter the ID of the employee to remove: ";
    std::cin >> id;

    auto it = std::remove_if(employees.begin(), employees.end(), [id](const Employee& employee) {
        return employee.id == id;
    });

    if (it != employees.end()) {
        employees.erase(it, employees.end());

        std::ofstream file("employees.csv");
        for (const auto& employee : employees) {
            saveEmployeeToCSV("employees.csv", employee);
        }

        std::cout << "Employee removed successfully.\n";
    } else {
        std::cout << "Employee not found.\n";
    }
}

int main() {
    std::vector<Employee> employees = loadEmployeesFromCSV("employees.csv");
    std::string command;
    showHelp();

    while (true) {
        std::cout << "\nEnter command: ";
        std::cin >> command;

        if (command == "addemployee") {
            int id, age;
            std::string name, department;

            std::cout << "Enter ID: ";
            std::cin >> id;

            std::cin.ignore(); // Ignore newline character
            std::cout << "Enter name: ";
            std::getline(std::cin, name);
            while (!validateName(name)) {
                std::cout << "Invalid name. Enter a valid name (alphabetical characters only): ";
                std::getline(std::cin, name);
            }

            std::cout << "Enter age: ";
            while (!(std::cin >> age) || !validateAge(age)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid age. Enter a valid age (18-65): ";
            }

            std::cin.ignore(); // Ignore newline character
            std::cout << "Enter department: ";
            std::getline(std::cin, department);
            while (department.empty()) {
                std::cout << "Department cannot be empty. Enter department: ";
                std::getline(std::cin, department);
            }

            if (validateInput(id, name, age, department)) {
                Employee newEmployee(id, name, age, department);
                saveEmployeeToCSV("employees.csv", newEmployee);
                employees.push_back(newEmployee);
                std::cout << "Employee added successfully.\n";
            } else {
                std::cout << "Invalid input. Please try again.\n";
            }
        } else if (command == "listemployees") {
            listEmployees(employees);
        } else if (command == "updateemployee") {
            updateEmployee(employees);
        } else if (command == "removeemployee") {
            removeEmployee(employees);
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
