#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

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

        if (data.empty()) {
            cerr << "Error: No data loaded from file." << endl;
        }

        return data;
    }
};

struct School {
    string name, address, city, state, county;
    School* next;

    School(string n, string a, string c, string s, string co)
        : name(n), address(a), city(c), state(s), county(co), next(nullptr) {}
};

struct TreeNode {
    School* school;
    TreeNode* left;
    TreeNode* right;
    TreeNode(School* school) : school(school), left(nullptr), right(nullptr) {}
};

class SchoolList {
private:
    School* head;

public:
    SchoolList() : head(nullptr) {}

    void insertFirst(School* school) {
        school->next = head;
        head = school;
    }

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

    School* findByName(const string& name) {
        School* temp = head;
        while (temp) {
            if (temp->name == name)
                return temp;
            temp = temp->next;
        }
        return nullptr;
    }
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

     TreeNode* findMin(TreeNode* node) {
         while (node && node->left)
             node = node->left;
         return node;
     }

     TreeNode* deleteByName(TreeNode* node, const string& name) {
         if (node == nullptr) return nullptr;
         if (name < node->school->name) {
             node->left = deleteByName(node->left, name);
         } else if (name > node->school->name) {
             node->right = deleteByName(node->right, name);
         } else {
             //Node with less than 2 children
             if (!node->left) {
                 TreeNode* temp = node->right;
                 delete node;
                 return temp;
             } else if (!node->right) {
                 TreeNode* temp = node->left;
                 delete node;
                 return temp;
             }

             //Node with 2 children
             TreeNode* successor = findMin(node->right);
             node->school = successor->school;
             node->right = deleteByName(node->right, successor->school->name);
         }
     }

     TreeNode* findByName(TreeNode* node, const string& name) {
         if (!node || node->school->name == name) {
             return node;
         } if (node->school->name > name) {
             return findByName(node->left, name);
         } return findByName(node->right, name);
     }

     public:
     SchoolBST() : root(nullptr) {}

     void insert(School* school) {
         root = insert(root, school);
     }

     TreeNode* findMin() {
         return findMin(root);
     }

     TreeNode* deleteByName(const string& name) {
         root = deleteByName(root, name);
     }

     School* findByName(const string& name) {
         TreeNode* node = findByName(root, name);
         return node ? node->school : nullptr;
     }
 };

class SchoolHashTable {
private:
    static const int TABLE_SIZE = 100;
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
};

int main() {
    SchoolList list;
    SchoolHashTable hashTable;
    vector<vector<string>> data = CSVReader::readCSV("Illinois_Schools.csv");

    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i].size() == 5) {
            list.insertLast(new School(data[i][0], data[i][1], data[i][2], data[i][3], data[i][4]));
            hashTable.insert(new School(data[i][0], data[i][1], data[i][2], data[i][3], data[i][4]));
        }
    }

    return 0;

}
