#include <iostream>
#include <vector>
#include <string>
#include <fstream>
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
string dataFilename = "datatest5.txt";
string documentFolder = "documents/";
string role;
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
        for (const auto& document : documents) {
            file << "DOCUMENT|" << document.name << "\n";
        }
    }
}
void loadData(const string& filename) {
    ifstream file(filename);
    if (file) {
        tasks.clear();
        resources.clear();
        workers.clear();
        documents.clear();
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
            } else if (type == "DOCUMENT") {
                Document document;
                getline(ss, document.name, '|');
                documents.push_back(document);
            }
        }
    }
}
// Task Management
void addTask() {
    Task task;
    cin.ignore();
    cout << "Enter task description: ";
    getline(cin, task.description);
    cout << "Enter task priority (Low/Medium/High): ";
    getline(cin, task.priority);
    task.progress = 0;
    task.status = "Not started yet";
    cout << "Available resources:\n";
    for (size_t i = 0; i < resources.size(); ++i) {
        cout << i + 1 << ". " << resources[i].name << " (Quantity: " << resources[i].quantity << ")\n";
    }
    cout << "Enter resource name to allocate: ";
    getline(cin, task.resourcesAllocated);
    cout << "Enter quantity to allocate: ";
    int quantity;
    cin >> quantity;
    bool resourceFound = false;
    for (size_t i = 0; i < resources.size(); ++i) {
        if (resources[i].name == task.resourcesAllocated) {
            if (quantity <= resources[i].quantity) {
                resources[i].quantity -= quantity;
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
    for (size_t i = 0; i < workers.size(); ++i) {
        cout << "ID: " << workers[i].id << ", Name: " << workers[i].name << ", Role: " << workers[i].role << "\n";
    }
    cout << "Enter worker ID to assign to this task: ";
    cin >> task.workerID;
    bool workerFound = false;
    for (size_t i = 0; i < workers.size(); ++i) {
        if (workers[i].id == task.workerID) {
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
                    for (size_t i = 0; i < workers.size(); ++i) {
                        cout << "ID: " << workers[i].id << ", Name: " << workers[i].name << ", Role: " << workers[i].role << "\n";
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
                    cout << "Enter new progress for this task (0-100%): ";
                    cin >> task.progress;
                    break;
                }
                case 8: {
                    cout << "Returning to Task Management menu.\n";
                    break;
                }
                default: {
                    cout << "Invalid choice. Please try again.\n";
                    break;
                }
            }
        } while (choice != 8);
    } else {
        cout << "Invalid task number.\n";
    }
}
void deleteTask() {
    displayTasks();
    size_t index;
    cout << "Enter task number to delete: ";
    cin >> index;
    if (index > 0 && index <= tasks.size()) {
        tasks.erase(tasks.begin() + index - 1);
        cout << "Task deleted successfully.\n";
    } else {
        cout << "Invalid task number.\n";
    }
}
void trackProgress() {
    cout << "\n=====================================\n";
    cout << "           Track Progress            \n";
    cout << "=====================================\n";
    if (tasks.empty()) {
        cout << "No tasks available.\n";
        return;
    }
    for (size_t i = 0; i < tasks.size(); ++i) {
        const Task& task = tasks[i];
        cout << "Task " << i + 1 << ":\n";
        cout << "Description: " << task.description << "\n";
        cout << "Progress: " << task.progress << "%\n";
        cout << "-------------------------------------\n";
    }
}
void markTaskAsComplete() {
    int taskNumber;
    cout << "\nEnter the task number to mark as complete: ";
    cin >> taskNumber;
    if (taskNumber > 0 && taskNumber <= tasks.size()) {
        Task& task = tasks[taskNumber - 1];
        task.status = "Complete";
        task.progress = 100;

        cout << "Task " << taskNumber << " marked as complete.\n";
    } else {
        cout << "Invalid task number. Please try again.\n";
    }
}
void manageTasks() {
    int choice;
    do {
        cout << "\n=====================================\n";
        cout << "        Task Management Menu         \n";
        cout << "=====================================\n";
        cout << " 1. Add Task\n";
        cout << " 2. Display Tasks\n";
        cout << " 3. Update Task\n";
        cout << " 4. Track Progress\n";
        cout << " 5. Mark Task as Complete\n";
        cout << " 6. Back to Role Menu\n";
        cout << "=====================================\n";
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
                trackProgress();
                break;
            case 5:
                markTaskAsComplete();
                break;
            case 6:
                cout << "Returning to Role Menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 6);
}
// Document Management
void addDocument() {
    Document document;
    cin.ignore();
    cout << "Enter document name: ";
    getline(cin, document.name);
    documents.push_back(document);
    cout << "Document added successfully.\n";
}
void displayDocuments() {
    for (size_t i = 0; i < documents.size(); ++i) {
        cout << "Document " << i + 1 << ": " << documents[i].name << "\n";
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
        cout << "\n=====================================\n";
        cout << "        Document Management          \n";
        cout << "=====================================\n";
        cout << " 1. Add Document\n";
        cout << " 2. Display Documents\n";
        cout << " 3. Delete Document\n";
        cout << " 4. Back to Role Menu\n";
        cout << "=====================================\n";
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
                cout << "Returning to Role Menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);
}
//Worker management
void addWorker() {
    Worker worker;
    cin.ignore();
    cout << "Enter worker name: ";
    getline(cin, worker.name);
    cout << "Enter worker role: ";
    getline(cin, worker.role);
    cout << "Enter worker daily wage: ";
    cin >> worker.dailyWage;
    cout << "Enter worker monthly wage: ";
    cin >> worker.monthlyWage;
    worker.id = workers.empty() ? 1 : workers.back().id + 1;
    workers.push_back(worker);
    cout << "Worker added successfully with ID " << worker.id << ".\n";
}
void displayWorkers() {
    if (workers.empty()) {
        cout << "No workers available.\n";
        return;
    }
    cout << "Workers List:\n";
    for (const auto& worker : workers) {
        cout << "ID: " << worker.id << "\n";
        cout << "Name: " << worker.name << "\n";
        cout << "Role: " << worker.role << "\n";
        cout << "Daily Wage: $" << worker.dailyWage << "\n";
        cout << "Monthly Wage: $" << worker.monthlyWage << "\n\n";
    }
}
void removeWorker() {
    if (workers.empty()) {
        cout << "No workers available to remove.\n";
        return;
    }
    int idToRemove;
    cout << "Enter the ID of the worker to remove: ";
    cin >> idToRemove;
    auto it = find_if(workers.begin(), workers.end(), [idToRemove](const Worker& worker) {
        return worker.id == idToRemove;
    });
    if (it != workers.end()) {
        workers.erase(it);
        cout << "Worker removed successfully.\n";
    } else {
        cout << "Worker with ID " << idToRemove << " not found.\n";
    }
}
void updateWorker() {
    if (workers.empty()) {
        cout << "No workers available to update.\n";
        return;
    }
    int idToUpdate;
    cout << "Enter the ID of the worker to update: ";
    cin >> idToUpdate;
    auto it = find_if(workers.begin(), workers.end(), [idToUpdate](const Worker& worker) {
        return worker.id == idToUpdate;
    });
    if (it != workers.end()) {
        Worker& workerToUpdate = *it;
        cout << "Select what you want to update:\n";
        cout << "1. Name\n";
        cout << "2. Role\n";
        cout << "3. Daily Wage\n";
        cout << "4. Monthly Wage\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: {
                cout << "Enter new name: ";
                getline(cin, workerToUpdate.name);
                break;
            }
            case 2: {
                cout << "Enter new role: ";
                getline(cin, workerToUpdate.role);
                break;
            }
            case 3: {
                cout << "Enter new daily wage: ";
                cin >> workerToUpdate.dailyWage;
                break;
            }
            case 4: {
                cout << "Enter new monthly wage: ";
                cin >> workerToUpdate.monthlyWage;
                break;
            }
            default:
                cout << "Invalid choice. No changes made.\n";
                return;
        }
        cout << "Worker updated successfully.\n";
    } else {
        cout << "Worker with ID " << idToUpdate << " not found.\n";
    }
}
void manageWorkers() {
    int choice;
    do {
        cout << "\n=====================================\n";
        cout << "        Manage Workers Menu         \n";
        cout << "=====================================\n";
        cout << " 1. Add Worker\n";
        cout << " 2. Display Workers\n";
        cout << " 3. Remove Worker\n";
        cout << " 4. Back to Role Menu\n";
        cout << "=====================================\n";
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
                cout << "Returning to Role Menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);
}
// Resource Management
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
    cout << "\n=====================================\n";
    cout << "          Display Resources           \n";
    cout << "=====================================\n";
    if (resources.empty()) {
        cout << "No resources available.\n";
    } else {
        cout << "Available resources:\n";
        for (const auto& resource : resources) {
            cout << "Name: " << resource.name << ", Quantity: " << resource.quantity << "\n";
        }
    }
}
void removeResource() {
    if (resources.empty()) {
        cout << "No resources available to remove.\n";
        return;
    }
    cout << "Available resources to remove:\n";
    for (size_t i = 0; i < resources.size(); ++i) {
        cout << i + 1 << ". " << resources[i].name << " (Quantity: " << resources[i].quantity << ")\n";
    }
    size_t index;
    cout << "Enter the index of the resource to remove: ";
    cin >> index;
    if (index >= 1 && index <= resources.size()) {
        cout << "Enter the quantity to remove: ";
        int quantityToRemove;
        cin >> quantityToRemove;

        if (quantityToRemove <= 0) {
            cout << "Invalid quantity. Please enter a positive quantity.\n";
            return;
        }

        if (quantityToRemove >= resources[index - 1].quantity) {
            resources.erase(resources.begin() + index - 1);
            cout << "Resource removed successfully.\n";
        } else {
            resources[index - 1].quantity -= quantityToRemove;
            cout << "Quantity updated successfully.\n";
        }
    } else {
        cout << "Invalid index. Please enter a valid index.\n";
    }
}
void manageResources() {
    int choice;
    do {
        cout << "\n=====================================\n";
        cout << "       Manage Resources Menu         \n";
        cout << "=====================================\n";
        cout << " 1. Add Resource\n";
        cout << " 2. Display Resources\n";
        cout << " 3. Remove Resource\n";
        cout << " 4. Back to Role Menu\n";
        cout << "=====================================\n";
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
                cout << "Returning to Role Menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);
}
// Budget Management
void addExpense() {
    Expense expense;
    cout << "Enter expense amount: ";
    cin >> expense.amount;
    expenses.push_back(expense);
    initialBudget -= expense.amount;
    cout << "Expense added successfully. Remaining budget: " << initialBudget << "\n";
}
void viewBudget() {
    cout << "\n=====================================\n";
    cout << "             View Budget             \n";
    cout << "=====================================\n";
    cout << "Remaining budget: " << initialBudget << "\n";
}
void manageBudget() {
    int choice;
    do {
        cout << "\n=====================================\n";
        cout << "         Manage Budget Menu          \n";
        cout << "=====================================\n";
        cout << " 1. Add Expense\n";
        cout << " 2. View Budget\n";
        cout << " 3. Back to Role Menu\n";
        cout << "=====================================\n";
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
                cout << "Returning to Role Menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 3);
}
// Role menus
void siteSupervisorMenu(const string& filename) {
    int choice;
    do {
        cout << "\n=====================================\n";
        cout << "         Site Supervisor Menu        \n";
        cout << "=====================================\n";
        cout << " 1. Manage Tasks\n";
        cout << " 2. Manage Workers\n";
        cout << " 3. Manage Resources\n";
        cout << " 4. Exit\n";
        cout << "=====================================\n";
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
        cout << "\n=====================================\n";
        cout << "         Project Manager Menu        \n";
        cout << "=====================================\n";
        cout << " 1. Manage Tasks\n";
        cout << " 2. Manage Documents\n";
        cout << " 3. Manage Budget\n";
        cout << " 4. Exit\n";
        cout << "=====================================\n";
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
        cout << "\n=====================================\n";
        cout << "        Worker Responsible Menu      \n";
        cout << "=====================================\n";
        cout << " 1. Display Tasks\n";
        cout << " 2. Mark Task as Complete\n";
        cout << " 3. Exit\n";
        cout << "=====================================\n";
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
void displayWelcome() {
    cout << "\n=====================================\n";
    cout << "                KRiTA                \n";
    cout << "=====================================\n";
    cout << "       Construction Management       \n";
    cout << "=====================================\n";
    cout << "Enter your role (Project Manager/Site Supervisor/Worker Responsible): ";
    getline(cin, role);
}
void mainMenu(const string& role, const string& filename) {
    if (role == "Project Manager") {
        projectManagerMenu(filename);
    } else if (role == "Site Supervisor") {
        siteSupervisorMenu(filename);
    } else if (role == "Worker Responsible") {
        workerResponsibleMenu(filename);
    } else {
        cout << "Invalid role. Exiting...\n";
    }
}
int main() {
    createFileIfNonExistent(dataFilename);
    loadData(dataFilename);
    displayWelcome();
    mainMenu(role, dataFilename);
    saveData(dataFilename);
    return 0;
}