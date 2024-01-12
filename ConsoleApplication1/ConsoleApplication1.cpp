#include <iostream>
#include "sqlite3.h"
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

// Функция для вычисления возраста на основе даты рождения
int calculateAge(std::string birthday) {
    // Получение текущей даты
    std::time_t t = std::time(0);
    std::tm now;
    localtime_s(&now, &t);
    // Разбиение даты рождения на год, месяц и день
    int birthYear = std::stoi(birthday.substr(0, 4));
    int birthMonth = std::stoi(birthday.substr(5, 2));
    int birthDay = std::stoi(birthday.substr(8, 2));
    // Вычисление возраста
    int age = now.tm_year + 1900 - birthYear;
    if (now.tm_mon + 1 < birthMonth || (now.tm_mon + 1 == birthMonth && now.tm_mday < birthDay)) {
        age--;
    }

    return age;
}

// Функция обратного вызова для вывода результатов запроса
static int callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << argv[i] << std::endl;
    }
    std::cout << std::endl;
    return 0;
}

// Функция для просмотра данных из таблицы
void viewData(sqlite3* db) {
    // Выполнение запроса на выборку данных
    std::string selectQuery = "SELECT * FROM MyTable;";
    int rc = sqlite3_exec(db, selectQuery.c_str(), callback, 0, NULL);

    if (rc != SQLITE_OK) {
        std::cout << "Error when executing a data selection request: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
}

// Функция для добавления и удаления данных в таблице
void addAndDeleteData(sqlite3* db) {
    int choice;
    do {
        std::cout << "===============" << std::endl;
        std::cout << "1. Add data" << std::endl;
        std::cout << "2. Delete data" << std::endl;
        std::cout << "3. Back to main menu" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
        {
            std::cout << "Enter the name: ";
            std::string name;
            std::cin >> name;

            std::cout << "Enter the birthday (YYYY-MM-DD): ";
            std::string birthday;
            std::cin >> birthday;

            // Вычисление возраста пользователя
            int age = calculateAge(birthday);

            // Добавление данных в таблицу
            std::string insertQuery = "INSERT INTO MyTable (Name, Birthday) VALUES ('" + name + "', '" + birthday + "');";
            int rc = sqlite3_exec(db, insertQuery.c_str(), NULL, 0, NULL);

            if (rc != SQLITE_OK) {
                std::cout << "Error when adding data to the table: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_close(db);
                return;
            }
            else {
                std::cout << "The data has been added successfully" << std::endl;
            }
            break;
        }
        case 2:
        {
            std::cout << "Enter the name to delete: ";
            std::string name;
            std::cin >> name;

            // Удаление данных из таблицы по имени
            std::string deleteQuery = "DELETE FROM MyTable WHERE Name = '" + name + "';";
            int rc = sqlite3_exec(db, deleteQuery.c_str(), NULL, 0, NULL);

            if (rc != SQLITE_OK) {
                std::cout << "Error when deleting data from the table: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_close(db);
                return;
            }
            else {
                std::cout << "The data has been deleted successfully" << std::endl;
            }
            break;
        }
        case 3:
        {
            return;
        }
        default:
        {
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }
        }
    } while (choice != 3);
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("mydb.db", &db);
        if (rc != SQLITE_OK) {
            std::cout << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
            return 0;
        }
        else {
            std::cout << "Database opened successfully" << std::endl;
        }

    int choice;
    do {
        std::cout << "===============" << std::endl;
        std::cout << "1. View data" << std::endl;
        std::cout << "2. Add and delete data" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "===============" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
        {
            viewData(db);
            break;
        }
        case 2:
        {
            addAndDeleteData(db);
            break;
        }
        case 3:
        {
            break;
        }
        default:
        {
            std::cout << "Invalid choice. Please try again." << std::endl; }
        }
    } while (choice != 3);

    sqlite3_close(db);
    return 0;
}