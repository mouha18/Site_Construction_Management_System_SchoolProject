#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
//Struct initialization
struct Task {
    int id;
    string description;
    string status;
};
struct Document {
    int id;
    string name;
};
struct Expense {
    double amount;
};
//Data initialization
vector<Task> tasks;
vector<Document> documents;
vector<Expense> expenses;
double initialBudget = 10000.0;
//Data Managemet
void createFileIfNonExistent(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        ofstream outfile(filename);
        outfile << "";
    }
}
void loadData(const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error opening file\n";
        return;
    }
    string line;
    string section;
    while (getline(infile, line)) {
        if (line == "TASKS") {
            section = "TASKS";
        } else if (line == "DOCUMENTS") {
            section = "DOCUMENTS";
        } else if (line == "EXPENSES") {
            section = "EXPENSES";
        } else if (line == "BUDGET") {
            section = "BUDGET";
        } else {
            if (section == "TASKS") {
                Task task;
                size_t pos1 = line.find(';');
                size_t pos2 = line.find(';', pos1 + 1);
                task.id = stoi(line.substr(0, pos1));
                task.description = line.substr(pos1 + 1, pos2 - pos1 - 1);
                task.status = line.substr(pos2 + 1);
                tasks.push_back(task);
            } else if (section == "DOCUMENTS") {
                Document doc;
                size_t pos = line.find(';');
                doc.id = stoi(line.substr(0, pos));
                doc.name = line.substr(pos + 1);
                documents.push_back(doc);
            } else if (section == "EXPENSES") {
                Expense expense;
                expense.amount = stod(line);
                expenses.push_back(expense);
            } else if (section == "BUDGET") {
                initialBudget = stod(line);
            }
        }
    }
    infile.close();
}
void saveData(const string& filename) {
    ofstream outfile(filename);
    outfile << "TASKS\n";
    for (const auto& task : tasks) {
        outfile << task.id << ";" << task.description << ";" << task.status << "\n";
    }
    outfile << "DOCUMENTS\n";
    for (const auto& doc : documents) {
        outfile << doc.id << ";" << doc.name << "\n";
    }
    outfile << "EXPENSES\n";
    for (const auto& expense : expenses) {
        outfile << expense.amount << "\n";
    }
    outfile << "BUDGET\n";
    outfile << initialBudget << "\n";
    outfile.close();
}
//Task Management
void addTask() {
    Task task;
    cout << "Enter task ID: ";
    cin >> task.id;
    cin.ignore();
    cout << "Enter task description: ";
    getline(cin, task.description);
    task.status = "in progress";
    tasks.push_back(task);
    cout << "Task added successfully.\n";
}
void displayTasks() {
    for (const auto& task : tasks) {
        cout << "Task ID: " << task.id << ", Description: " << task.description << ", Status: " << task.status << "\n";
    }
}
void updateTask() {
    int id;
    cout << "Enter task ID to update: ";
    cin >> id;
    for (auto& task : tasks) {
        if (task.id == id) {
            cin.ignore();
            cout << "Enter new description: ";
            getline(cin, task.description);
            cout << "Enter new status: ";
            getline(cin, task.status);
            cout << "Task updated successfully.\n";
            return;
        }
    }
    cout << "Task not found.\n";
}
void manageTasks() {
    int choice;
    do {
        cout << "Manage Tasks:\n";
        cout << "1. Add Task\n";
        cout << "2. Display Tasks\n";
        cout << "3. Update Task\n";
        cout << "4. Back\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                displayTasks();
                break;
            case 3:
                updateTask();
                break;
            case 4:
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);
}
//Document Management
void addDocument() {
    Document doc;
    cout << "Enter document ID: ";
    cin >> doc.id;
    cin.ignore(); // Ignore newline character
    cout << "Enter document name: ";
    getline(cin, doc.name);
    documents.push_back(doc);
    cout << "Document added successfully.\n";
}
void displayDocuments() {
    for (const auto& doc : documents) {
        cout << "Document ID: " << doc.id << ", Name: " << doc.name << "\n";
    }
}
void deleteDocument() {
    int id;
    cout << "Enter document ID to delete: ";
    cin >> id;
    for (auto it = documents.begin(); it != documents.end(); ++it) {
        if (it->id == id) {
            documents.erase(it);
            cout << "Document deleted successfully.\n";
            return;
        }
    }
    cout << "Document not found.\n";
}
void manageDocuments() {
    int choice;
    do {
        cout << "Manage Documents:\n";
        cout << "1. Add Document\n";
        cout << "2. Display Documents\n";
        cout << "3. Delete Document\n";
        cout << "4. Back\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                addDocument();
                break;
            case 2:
                displayDocuments();
                break;
            case 3:
                deleteDocument();
                break;
            case 4:
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);
}
//Budget management
void addExpense() {
    Expense expense;
    cout << "Enter expense amount: ";
    cin >> expense.amount;
    expenses.push_back(expense);
    initialBudget -= expense.amount;
    cout << "Expense added. Remaining budget: " << initialBudget << "\n";
}
void viewBudget() {
    cout << "Remaining budget: " << initialBudget << "\n";
}
void manageBudget() {
    int choice;
    do {
        cout << "Manage Budget:\n";
        cout << "1. Add Expense\n";
        cout << "2. View Budget\n";
        cout << "3. Back\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                addExpense();
                break;
            case 2:
                viewBudget();
                break;
            case 3:
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 3);
}
//Main Program
int main() {
    string filename = "datatest1.txt";
    createFileIfNonExistent(filename);
    loadData(filename);
    // Project Manager Menu
    int choice;
    do {
        cout << "Project Manager Menu:\n";
        cout << "1. Manage Tasks\n";
        cout << "2. Manage Documents\n";
        cout << "3. Manage Budget\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                manageTasks();
                break;
            case 2:
                manageDocuments();
                break;
            case 3:
                manageBudget();
                break;
            case 4:
                saveData(filename);
                cout << "Data saved. Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);
    return 0;
}