#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// Class to handle reading CSV files (Task 3)
class CSVReader {
public:
    static vector<vector<string>> readCSV(const string& filename) {
        ifstream file("cmake-build-debug/SchoolLists/"+filename);
        vector<vector<string>> data;
        string line, word;

        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return data;
        }

        while (getline(file, line)) {
            stringstream ss(line);
            vector<string> row;
            while (getline(ss, word, ',')) {
                row.push_back(word);
            }
            data.push_back(row);
        }
        file.close();
        return data;
    }
};

// Structure to store school data (Task 1)
struct School {
    string name, address, city, state, county;
    School* next;

    // Constructor to initialize school object
    School(string n, string a, string c, string s, string co)
        : name(n), address(a), city(c), state(s), county(co), next(nullptr) {}
};

class SchoolHashTable {
private:
    static const int TABLE_SIZE = 10;
    vector<School*> table;

    int hashFunction(string key, int tableSize = TABLE_SIZE) {
        int hash = 0;
        for (char ch : key) {
            hash += ch;
        }
        return hash % tableSize;
    }

public:
    SchoolHashTable() : table(TABLE_SIZE, nullptr) {}

    void insert(School* School) {
        int index = hashFunction(School->name);
        School->next = table[index];
        table[index] = School;
    }

    void deleteByName(const string& name) {
        int index = hashFunction(name);
        School* temp = table[index];
        School* prev = nullptr;

        while (temp && temp->name != name) {
            prev = temp;
            temp = temp->next;
        }

        if (!temp) {
            cout << "School not found.\n";
            return;
        }

        if (!prev)
            table[index] = temp->next;
        else
            prev->next = temp->next;

        delete temp;
    }


    School* findByName(const string& name) {
        int index = hashFunction(name);
        School* School = table[index];
        while (School) {
            if (School->name == name) {
                return School;
            }
            School = School->next;
        }
        return nullptr;
    }

    void display() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i]) {
                School* temp = table[i];
                cout << "Index " << i << ": ";
                while (temp) {
                    cout << temp->name << ", " << temp->address << " " << temp->city << ", " << temp->state << ", " << temp->county << endl;
                    temp = temp->next;
                }
            }
        }
    }
};

// Main function to load data, manage the linked list, and allow user interaction (Task 4)
int main() {
    SchoolHashTable hashTable;
    vector<vector<string>> data = CSVReader::readCSV("Illinois_Schools.csv");

    // Load data into linked list, skipping the header row
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i].size() == 5)
            hashTable.insert(new School(data[i][0], data[i][1], data[i][2], data[i][3], data[i][4]));
    }

    int choice;
    string name;
    do {
        cout << "1. Display Schools\n2. Find School\n3. Delete School\n4. Exit\nChoice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                hashTable.display();
                break;
            case 2:
                cout << "Enter school name: ";
                getline(cin, name);
                if (School* school = hashTable.findByName(name))
                    cout << school->name << ", " << school->address << "\n";
                else
                    cout << "School not found.\n";
                break;
            case 3:
                cout << "Enter school name: ";
                getline(cin, name);
                hashTable.deleteByName(name);
                break;
        }
    } while (choice != 4);
    return 0;
}
