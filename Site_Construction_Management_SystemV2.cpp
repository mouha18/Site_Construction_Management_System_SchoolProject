#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
//Struct Initializaton
struct Task {
    string description;
    string status;
};
struct Document {
    string name;
};
struct Expense {
    double amount;
};
struct Worker {
    int id;
    string name;
    string role;
};
struct Resource {
    string name;
    int quantity;
};
//Data Initialization
vector<Task> tasks;
vector<Document> documents;
vector<Expense> expenses;
vector<Worker> workers;
vector<Resource> resources;
double initialBudget = 0.0;
void createFileIfNonExistent(const string& filename) {
    ifstream infile(filename);
    if (!infile.good()) {
        ofstream outfile(filename);
        outfile.close();
    }
}
//Data Management
void saveData(const string& filename) {
    ofstream outfile(filename);
    outfile << "TASKS\n";
    for (const auto& task : tasks) {
        outfile << task.description << ";" << task.status << "\n";
    }
    outfile << "DOCUMENTS\n";
    for (const auto& doc : documents) {
        outfile << doc.name << "\n";
    }
    outfile << "EXPENSES\n";
    for (const auto& expense : expenses) {
        outfile << expense.amount << "\n";
    }
    outfile << "WORKERS\n";
    for (const auto& worker : workers) {
        outfile << worker.id << ";" << worker.name << ";" << worker.role << "\n";
    }
    outfile << "RESOURCES\n";
    for (const auto& resource : resources) {
        outfile << resource.name << ";" << resource.quantity << "\n";
    }
    outfile << "BUDGET\n";
    outfile << initialBudget << "\n";  
    outfile.close();
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
        } else if (line == "WORKERS") {
            section = "WORKERS";
        } else if (line == "RESOURCES") {
            section = "RESOURCES";
        } else if (line == "BUDGET") {
            section = "BUDGET";
        } else {
            if (section == "TASKS") {
                Task task;
                size_t pos = line.find(';');
                task.description = line.substr(0, pos);
                task.status = line.substr(pos + 1);
                tasks.push_back(task);
            } else if (section == "DOCUMENTS") {
                Document doc;
                doc.name = line;
                documents.push_back(doc);
            } else if (section == "EXPENSES") {
                Expense expense;
                expense.amount = stod(line);
                expenses.push_back(expense);
            } else if (section == "WORKERS") {
                Worker worker;
                size_t pos1 = line.find(';');
                size_t pos2 = line.find(';', pos1 + 1);
                worker.id = stoi(line.substr(0, pos1));
                worker.name = line.substr(pos1 + 1, pos2 - pos1 - 1);
                worker.role = line.substr(pos2 + 1);
                workers.push_back(worker);
            } else if (section == "RESOURCES") {
                Resource resource;
                size_t pos = line.find(';');
                resource.name = line.substr(0, pos);
                resource.quantity = stoi(line.substr(pos + 1));
                resources.push_back(resource);
            } else if (section == "BUDGET") {
                initialBudget = stod(line);
            }
        }
    }
    infile.close();
}
void addTask() {
    Task task;
    cin.ignore();
    cout << "Enter task description: ";
    getline(cin, task.description);
    task.status = "Incomplete";
    tasks.push_back(task);
    cout << "Task added successfully.\n";
}
void displayTasks() {
    for (size_t i = 0; i < tasks.size(); ++i) {
        cout << i + 1 << ". " << tasks[i].description << " - " << tasks[i].status << "\n";
    }
}
void updateTask() {
    displayTasks();
    size_t index;
    cout << "Enter task number to update: ";
    cin >> index;
    if (index > 0 && index <= tasks.size()) {
        cin.ignore();
        cout << "Enter new task description: ";
        getline(cin, tasks[index - 1].description);
        cout << "Enter new task status (Incomplete/Complete): ";
        getline(cin, tasks[index - 1].status);
        cout << "Task updated successfully.\n";
    } else {
        cout << "Invalid task number.\n";
    }
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
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);
}
//Document Management
void addDocument() {
    Document doc;
    cin.ignore();
    cout << "Enter document name: ";
    getline(cin, doc.name);
    documents.push_back(doc);
    cout << "Document added successfully.\n";
}
void displayDocuments() {
    for (size_t i = 0; i < documents.size(); ++i) {
        cout << i + 1 << ". " << documents[i].name << "\n";
    }
}
void deleteDocument() {
    displayDocuments();
    size_t index;
    cout << "Enter document number to delete: ";
    cin >> index;
    if (index > 0 && index <= documents.size()) {
        documents.erase(documents.begin() + index - 1);
        cout << "Document deleted successfully.\n";
    } else {
        cout << "Invalid document number.\n";
    }
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
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);
}
//Budget Management
void addExpense() {
    Expense expense;
    cout << "Enter expense amount: ";
    cin >> expense.amount;
    expenses.push_back(expense);
    initialBudget -= expense.amount;
    cout << "Expense added successfully. Remaining budget: " << initialBudget << "\n";
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
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 3);
}
//Worker Management
void addWorker() {
    Worker worker;
    worker.id = workers.empty() ? 1 : workers.back().id + 1;
    cin.ignore();
    cout << "Enter worker name: ";
    getline(cin, worker.name);
    cout << "Enter worker role: ";
    getline(cin, worker.role);
    workers.push_back(worker);
    cout << "Worker added successfully with ID " << worker.id << ".\n";
}
void displayWorkers() {
    for (const auto& worker : workers) {
        cout << "ID: " << worker.id << ", Name: " << worker.name << ", Role: " << worker.role << "\n";
    }
}
void removeWorker() {
    displayWorkers();
    int id;
    cout << "Enter worker ID to remove: ";
    cin >> id;
    auto it = find_if(workers.begin(), workers.end(), [id](const Worker& worker) {
        return worker.id == id;
    });
    if (it != workers.end()) {
        workers.erase(it);
        cout << "Worker removed successfully.\n";
    } else {
        cout << "Worker ID not found.\n";
    }
}
void manageWorkers() {
    int choice;
    do {
        cout << "Manage Workers:\n";
        cout << "1. Add Worker\n";
        cout << "2. Display Workers\n";
        cout << "3. Remove Worker\n";
        cout << "4. Back\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                addWorker();
                break;
            case 2:
                displayWorkers();
                break;
            case 3:
                removeWorker();
                break;
            case 4:
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);
}
//Ressource Management
void addResource() {
    Resource resource;
    cin.ignore();
    cout << "Enter resource name: ";
    getline(cin, resource.name);
    cout << "Enter resource quantity: ";
    cin >> resource.quantity;
    resources.push_back(resource);
    cout << "Resource added successfully.\n";
}
void displayResources() {
    for (const auto& resource : resources) {
        cout << "Name: " << resource.name << ", Quantity: " << resource.quantity << "\n";
    }
}
void removeResource() {
    displayResources();
    string name;
    cin.ignore();
    cout << "Enter resource name to remove: ";
    getline(cin, name);
    auto it = find_if(resources.begin(), resources.end(), [name](const Resource& resource) {
        return resource.name == name;
    });
    if (it != resources.end()) {
        resources.erase(it);
        cout << "Resource removed successfully.\n";
    } else {
        cout << "Resource name not found.\n";
    }
}
void manageResources() {
    int choice;
    do {
        cout << "Manage Resources:\n";
        cout << "1. Add Resource\n";
        cout << "2. Display Resources\n";
        cout << "3. Remove Resource\n";
        cout << "4. Back\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                addResource();
                break;
            case 2:
                displayResources();
                break;
            case 3:
                removeResource();
                break;
            case 4:
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);
}
//Role Menu
void siteSupervisorMenu(const string& filename) {
    int choice;
    do {
        cout << "Site Supervisor Menu:\n";
        cout << "1. Manage Tasks\n";
        cout << "2. Manage Workers\n";
        cout << "3. Manage Resources\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                manageTasks();
                break;
            case 2:
                manageWorkers();
                break;
            case 3:
                manageResources();
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
}
void projectManagerMenu(const string& filename) {
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
}
//Main Program
int main() {
    string filename = "datatest2.txt";
    createFileIfNonExistent(filename);
    loadData(filename);
    int role;
    cout << "Select Role:\n";
    cout << "1. Project Manager\n";
    cout << "2. Site Supervisor\n";
    cout << "Enter your choice: ";
    cin >> role;
    if (role == 1) {
        projectManagerMenu(filename);
    } else if (role == 2) {
        siteSupervisorMenu(filename);
    } else {
        cout << "Invalid role selection.\n";
    }
    return 0;
}