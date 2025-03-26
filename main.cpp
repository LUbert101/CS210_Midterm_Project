#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// Class to handle reading CSV files (Task 3)
class CSVReader {
public:
    static vector<vector<string>> readCSV(const string& filename) {
        ifstream file("SchoolLists/"+filename);
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

struct TreeNode {
    School* school;
    TreeNode* left;
    TreeNode* right;
    TreeNode(School* school) : school(school), left(nullptr), right(nullptr) {}
};

class SchoolBST {
    private:
    TreeNode* root;

    TreeNode* insert(TreeNode* node, School* school) {
        if (node == nullptr) {
            return new TreeNode(school);
        } if (school->name < node->school->name) {
            node->left = insert(node->left, school);
        } else {
            node->right = insert(node->right, school);
        } return node;
    }

    void displayInOrder(TreeNode* node) {
        if (!node) return;
        displayInOrder(node->left);
        cout << node->school->name << "," << node->school->address << endl;
        displayInOrder(node->right);
    }

    public:
    SchoolBST() : root(nullptr) {}

    void insert(School* school) {
        root = insert(root, school);
    }

    void displayInOrder() {
        displayInOrder(root);
    }
};

// Class to manage linked list of schools (Task 2)
class SchoolList {
private:
    School* head; // Pointer to head of list

public:
    SchoolList() : head(nullptr) {}

    // Insert school at the beginning of the list (insertFirst function)
    void insertFirst(School* school) {
        school->next = head;
        head = school;
    }

    // Insert school at the end of the list (insertLast function)
    void insertLast(School* school) {
        if (!head) {
            head = school;
            return;
        }
        School* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = school;
    }

    // Delete a school by name (deleteByName function)
    void deleteByName(const string& name) {
        School* temp = head, *prev = nullptr;
        while (temp && temp->name != name) {
            prev = temp;
            temp = temp->next;
        }
        if (!temp) {
            cout << "School not found.\n";
            return;
        }
        if (!prev)
            head = temp->next;
        else
            prev->next = temp->next;
        delete temp;
    }

    // Find and return a school by name (findByName function)
    School* findByName(const string& name) {
        School* temp = head;
        while (temp) {
            if (temp->name == name)
                return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    // Display all schools in the list (display function)
    void display() {
        School* temp = head;
        while (temp) {
            cout << temp->name << ", " << temp->address << ", "
                 << temp->city << ", " << temp->state << ", "
                 << temp->county << endl;
            temp = temp->next;
        }
    }
};

// Main function to load data, manage the linked list, and allow user interaction (Task 4)
int main() {
    SchoolBST bst;
    SchoolList list;
    vector<vector<string>> data = CSVReader::readCSV("Illinois_Schools.csv");

    // Load data into linked list, skipping the header row
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i].size() == 5)
            bst.insert(new School(data[i][0], data[i][1], data[i][2], data[i][3], data[i][4]));
    }

    int choice;
    string name;
    do {
        cout << "1. Display Schools\n2. Find School\n3. Delete School\n4. Exit\nChoice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                bst.displayInOrder();
                break;
            case 2:
                cout << "Enter school name: ";
                getline(cin, name);
                if (School* school = list.findByName(name))
                    cout << school->name << ", " << school->address << "\n";
                else
                    cout << "School not found.\n";
                break;
            case 3:
                cout << "Enter school name: ";
                getline(cin, name);
                list.deleteByName(name);
                break;
        }
    } while (choice != 4);
    return 0;
}
