#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
struct Task {
    string description;
    string priority;
    string status;
    string resourcesAllocated;
    int workerID;
    double budgetAllocation;
    string dueDate;
    int progress;
};
struct Document {
    string name;
};
struct Resource {
    string name;
    int quantity;
};
struct Worker {
    int id;
    string name;
    string role;
    double dailyWage;
    double monthlyWage;
};
struct Expense {
    double amount;
};
vector<Task> tasks;
vector<Resource> resources;
vector<Worker> workers;
vector<Document> documents;
vector<Expense> expenses;
double initialBudget = 10000.0;
string dataFilename = "data.txt";
string documentFolder = "documents/";
void createFileIfNonExistent(const string& filename) {
    ifstream file(filename);
    if (!file) {
        ofstream newFile(filename);
        if (newFile) {
            cout << "Created new data file: " << filename << "\n";
        } else {
            cerr << "Error creating file: " << filename << "\n";
        }
    }
}
void saveData(const string& filename) {
    ofstream file(filename);
    if (file) {
        for (const auto& task : tasks) {
            file << "TASK|" << task.description << "|"
                 << task.priority << "|"
                 << task.status << "|"
                 << task.resourcesAllocated << "|"
                 << task.workerID << "|"
                 << task.budgetAllocation << "|"
                 << task.dueDate << "|"
                 << task.progress << "\n";
        }
        for (const auto& resource : resources) {
            file << "RESOURCE|" << resource.name << "|" << resource.quantity << "\n";
        }
        for (const auto& worker : workers) {
            file << "WORKER|" << worker.id << "|"
                 << worker.name << "|"
                 << worker.role << "|"
                 << worker.dailyWage << "|"
                 << worker.monthlyWage << "\n";
        }
    }
}
void loadData(const string& filename) {
    ifstream file(filename);
    if (file) {
        tasks.clear();
        resources.clear();
        workers.clear();
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string item, type;
            getline(ss, type, '|');
            if (type == "TASK") {
                Task task;
                getline(ss, task.description, '|');
                getline(ss, task.priority, '|');
                getline(ss, task.status, '|');
                getline(ss, task.resourcesAllocated, '|');
                ss >> task.workerID;
                ss.ignore(1, '|');
                ss >> task.budgetAllocation;
                ss.ignore(1, '|');
                getline(ss, task.dueDate, '|');
                ss >> task.progress;
                tasks.push_back(task);
            } else if (type == "RESOURCE") {
                Resource resource;
                getline(ss, resource.name, '|');
                ss >> resource.quantity;
                resources.push_back(resource);
            } else if (type == "WORKER") {
                Worker worker;
                ss >> worker.id;
                ss.ignore(1, '|');
                getline(ss, worker.name, '|');
                getline(ss, worker.role, '|');
                ss >> worker.dailyWage;
                ss.ignore(1, '|');
                ss >> worker.monthlyWage;
                workers.push_back(worker);
            }
        }
    }
}
//Task Management
void addTask() {
    Task task;
    cin.ignore();
    cout << "Enter task description: ";
    getline(cin, task.description);
    cout << "Enter task priority (Low/Medium/High): ";
    cin >> task.priority;
    task.progress = 0;
    task.status = "Not started yet";
    cout << "Available resources:\n";
    for (size_t i = 0; i < resources.size(); ++i) {
        cout << i + 1 << ". " << resources[i].name << " (Quantity: " << resources[i].quantity << ")\n";
    }
    cout << "Enter resource name to allocate: ";
    string resourceName;
    cin >> task.resourcesAllocated;
    cout << "Enter quantity to allocate: ";
    int quantity;
    cin >> quantity;
    bool resourceFound = false;
    for (auto& res : resources) {
        if (res.name == task.resourcesAllocated) {
            if (quantity <= res.quantity) {
                res.quantity -= quantity;
                resourceFound = true;
            } else {
                cout << "Allocated quantity exceeds available quantity.\n";
                return;
            }
        }
    }
    if (!resourceFound) {
        cout << "Resource not found.\n";
        return;
    }
    cout << "Available workers:\n";
    for (const auto& worker : workers) {
        cout << "ID: " << worker.id << ", Name: " << worker.name << ", Role: " << worker.role << "\n";
    }
    cout << "Enter worker ID to assign to this task: ";
    cin >> task.workerID;
    bool workerFound = false;
    for (const auto& worker : workers) {
        if (worker.id == task.workerID) {
            workerFound = true;
            break;
        }
    }
    if (!workerFound) {
        cout << "Worker ID not found.\n";
        return;
    }
    cout << "Enter budget allocation for this task: ";
    cin >> task.budgetAllocation;
    if (task.budgetAllocation > initialBudget) {
        cout << "Budget allocation exceeds remaining budget.\n";
        return;
    }
    initialBudget -= task.budgetAllocation;
    cout << "Enter task due date (YYYY-MM-DD): ";
    cin >> task.dueDate;
    tasks.push_back(task);
    cout << "Task added successfully.\n";
}
void displayTasks() {
    for (size_t i = 0; i < tasks.size(); ++i) {
        const auto& task = tasks[i];
        cout << "Task " << i + 1 << ":\n";
        cout << "Description: " << task.description << "\n";
        cout << "Priority: " << task.priority << "\n";
        cout << "Status: " << task.status << "\n";
        cout << "Resources Allocated: " << task.resourcesAllocated << "\n";
        cout << "Worker Assigned: " << task.workerID << "\n";
        cout << "Budget Allocation: " << task.budgetAllocation << "\n";
        cout << "Due Date: " << task.dueDate << "\n";
        cout << "Progress: " << task.progress << "%\n";
    }
}
void updateTask() {
    displayTasks();
    size_t index;
    cout << "Enter task number to update: ";
    cin >> index;
    if (index > 0 && index <= tasks.size()) {
        Task& task = tasks[index - 1];
        int choice;
        do {
            cout << "Update Task Menu:\n";
            cout << "1. Description\n";
            cout << "2. Priority\n";
            cout << "3. Resources Allocated\n";
            cout << "4. Worker Assigned\n";
            cout << "5. Budget Allocation\n";
            cout << "6. Due Date\n";
            cout << "7. Progress\n";
            cout << "8. Back to Task Management\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();
            switch (choice) {
                case 1: {
                    cout << "Enter new task description: ";
                    getline(cin, task.description);
                    break;
                }
                case 2: {
                    cout << "Enter new task priority (Low/Medium/High): ";
                    getline(cin, task.priority);
                    break;
                }
                case 3: {
                    cout << "Available resources:\n";
                    for (size_t i = 0; i < resources.size(); ++i) {
                        cout << i + 1 << ". " << resources[i].name << " (Quantity: " << resources[i].quantity << ")\n";
                    }
                    cout << "Enter new resource name to allocate: ";
                    getline(cin, task.resourcesAllocated);
                    break;
                }
                case 4: {
                    cout << "Available workers:\n";
                    for (const auto& worker : workers) {
                        cout << "ID: " << worker.id << ", Name: " << worker.name << ", Role: " << worker.role << "\n";
                    }
                    cout << "Enter new worker ID to assign to this task: ";
                    cin >> task.workerID;
                    break;
                }
                case 5: {
                    cout << "Enter new budget allocation for this task: ";
                    cin >> task.budgetAllocation;
                    break;
                }
                case 6: {
                    cout << "Enter new due date for this task (YYYY-MM-DD): ";
                    cin >> task.dueDate;
                    break;
                }
                case 7: {
                    cout << "Enter new progress for this task (0-100): ";
                    cin >> task.progress;
                    break;
                }
                case 8: break;
                default: cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 8);

        cout << "Task updated successfully.\n";
    } else {
        cout << "Invalid task number.\n";
    }
}
void trackProgress() {
    for (size_t i = 0; i < tasks.size(); ++i) {
        const auto& task = tasks[i];
        cout << "Task " << i + 1 << " progress: " << task.progress << "%\n";
    }
}
void markTaskAsComplete() {
    displayTasks();
    size_t index;
    cout << "Enter task number to mark as complete: ";
    cin >> index;
    if (index > 0 && index <= tasks.size()) {
        tasks[index - 1].status = "Complete";
        cout << "Task marked as complete.\n";
    } else {
        cout << "Invalid task number.\n";
    }
}
void manageTasks() {
    int choice;
    do {
        cout << "Task Management Menu:\n";
        cout << "1. Add Task\n";
        cout << "2. Display Tasks\n";
        cout << "3. Update Task\n";
        cout << "4. Track Progress\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: addTask(); break;
            case 2: displayTasks(); break;
            case 3: updateTask(); break;
            case 4: trackProgress(); break;
            case 5: break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}
//Ressource management
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
//Worker Managment
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
//Document managing
void addDocument() {
    string name, description, path;
    cout << "Enter document name: ";
    cin.ignore();
    getline(cin, name);
    path = documentFolder + name + ".txt";
    ifstream checkFile(path);
    if (checkFile) {
        cout << "Document with this name already exists. Please choose a different name.\n";
        return;
    }
    cout << "Enter document description: ";
    getline(cin, description);
    ofstream docFile(path);
    docFile << description << "\n";
    docFile.close();
    cout << "Document added successfully.\n";
    cout << "To edit the document, use the following path: " << path << "\n";
}
void displayDocuments() {
    for (size_t i = 0; i < documents.size(); ++i) {
        cout << i + 1 << ". " << documents[i].name << "\n";
    }
}
void deleteDocument() {
    string name;
    cout << "Enter document name to delete: ";
    cin.ignore();
    getline(cin, name);
    string path = documentFolder + name + ".txt";
    if (remove(path.c_str()) == 0) {
        cout << "Document deleted successfully.\n";
    } else {
        cout << "Error deleting document. Make sure the document exists.\n";
    }
}
void manageDocuments() {
    int choice;
    do {
        cout << "Document Management Menu:\n";
        cout << "1. Add Document\n";
        cout << "2. Display Documents\n";
        cout << "3. Delete Document\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: addDocument(); break;
            case 2: displayDocuments(); break;
            case 3: deleteDocument(); break;
            case 4: break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);
}
//Budgeting management
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
//Role menu
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
void workerResponsibleMenu(const string& filename) {
    int choice;
    do {
        cout << "Worker Responsible Menu:\n";
        cout << "1. Display Tasks\n";
        cout << "2. Mark Task as Complete\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                displayTasks();
                break;
            case 2:
                markTaskAsComplete();
                break;
            case 3:
                saveData(filename);
                cout << "Data saved. Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 3);
}
int main() {
    string filename = "datatest4.txt";
    createFileIfNonExistent(filename);
    loadData(filename);
    int role;
    cout << "Select Role:\n";
    cout << "1. Project Manager\n";
    cout << "2. Site Supervisor\n";
    cout << "3. Worker Responsible\n";
    cout << "Enter your choice: ";
    cin >> role;
    if (role == 1) {
        projectManagerMenu(filename);
    } else if (role == 2) {
        siteSupervisorMenu(filename);
    } else if (role == 3) {
        workerResponsibleMenu(filename);
    } else {
        cout << "Invalid role selection.\n";
    }
    return 0;
}