#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>

using namespace std;

// Структура для хранения данных об ученике
struct Student {
    int id;
    string fullName;
    int age;
    string direction;
    string group;
    string startDate;
};

// Глобальные переменные
vector<Student> students;
const string FILENAME = "students.txt";
int nextId = 1;

// Функции для работы с файлом
void loadData() {
    ifstream file(FILENAME);
    if (!file.is_open()) {
        cout << "Файл с данными не найден. Будет создан новый файл." << endl;
        return;
    }

    students.clear();
    string line;
    while (getline(file, line)) {
        Student student;
        size_t pos = 0;
        string token;
        int field = 0;
        
        while ((pos = line.find("|")) != string::npos) {
            token = line.substr(0, pos);
            switch(field) {
                case 0: student.id = stoi(token); break;
                case 1: student.fullName = token; break;
                case 2: student.age = stoi(token); break;
                case 3: student.direction = token; break;
                case 4: student.group = token; break;
                case 5: student.startDate = token; break;
            }
            line.erase(0, pos + 1);
            field++;
        }
        students.push_back(student);
        nextId = max(nextId, student.id + 1);
    }
    file.close();
}

void saveData() {
    ofstream file(FILENAME);
    if (!file.is_open()) {
        cout << "Ошибка при сохранении файла!" << endl;
        return;
    }

    for (const auto& student : students) {
        file << student.id << "|" << student.fullName << "|" << student.age << "|"
             << student.direction << "|" << student.group << "|" << student.startDate << endl;
    }
    file.close();
}

// Функции для работы с данными
void addStudent() {
    Student student;
    student.id = nextId++;
    
    cout << "\nВведите ФИО: ";
    cin.ignore();
    getline(cin, student.fullName);
    
    cout << "Введите возраст: ";
    cin >> student.age;
    
    cout << "Введите направление: ";
    cin.ignore();
    getline(cin, student.direction);
    
    cout << "Введите группу: ";
    getline(cin, student.group);
    
    cout << "Введите дату начала обучения (формат: ДД.ММ.ГГГГ): ";
    getline(cin, student.startDate);
    
    students.push_back(student);
    saveData();
    cout << "\nУченик успешно добавлен!" << endl;
}

void displayStudents() {
    if (students.empty()) {
        cout << "\nБаза данных пуста!" << endl;
        return;
    }

    cout << "\nСписок учеников:" << endl;
    cout << setw(5) << "ID" << " | " << setw(30) << "ФИО" << " | " << setw(8) << "Возраст"
         << " | " << setw(20) << "Направление" << " | " << setw(10) << "Группа" << " | " << setw(12) << "Дата начала" << endl;
    cout << string(100, '-') << endl;

    for (const auto& student : students) {
        cout << setw(5) << student.id << " | " << setw(30) << student.fullName << " | " 
             << setw(8) << student.age << " | " << setw(20) << student.direction << " | "
             << setw(10) << student.group << " | " << setw(12) << student.startDate << endl;
    }
}

void editStudent() {
    int id;
    cout << "\nВведите ID ученика для редактирования: ";
    cin >> id;

    auto it = find_if(students.begin(), students.end(),
                     [id](const Student& s) { return s.id == id; });

    if (it == students.end()) {
        cout << "Ученик с таким ID не найден!" << endl;
        return;
    }

    cout << "\nВведите новые данные (оставьте пустым, чтобы не изменять):" << endl;
    
    cout << "ФИО: ";
    cin.ignore();
    string input;
    getline(cin, input);
    if (!input.empty()) it->fullName = input;
    
    cout << "Возраст: ";
    getline(cin, input);
    if (!input.empty()) it->age = stoi(input);
    
    cout << "Направление: ";
    getline(cin, input);
    if (!input.empty()) it->direction = input;
    
    cout << "Группа: ";
    getline(cin, input);
    if (!input.empty()) it->group = input;
    
    cout << "Дата начала обучения: ";
    getline(cin, input);
    if (!input.empty()) it->startDate = input;

    saveData();
    cout << "\nДанные ученика успешно обновлены!" << endl;
}

void deleteStudent() {
    int id;
    cout << "\nВведите ID ученика для удаления: ";
    cin >> id;

    auto it = find_if(students.begin(), students.end(),
                     [id](const Student& s) { return s.id == id; });

    if (it == students.end()) {
        cout << "Ученик с таким ID не найден!" << endl;
        return;
    }

    students.erase(it);
    saveData();
    cout << "\nУченик успешно удален!" << endl;
}

void searchStudent() {
    cout << "\nПоиск по:" << endl;
    cout << "1. ID" << endl;
    cout << "2. ФИО" << endl;
    cout << "3. Направлению" << endl;
    cout << "4. Группе" << endl;
    cout << "Выберите критерий поиска: ";
    
    int choice;
    cin >> choice;
    cin.ignore();

    string searchTerm;
    cout << "Введите поисковый запрос: ";
    getline(cin, searchTerm);

    vector<Student> results;
    for (const auto& student : students) {
        bool match = false;
        switch(choice) {
            case 1:
                match = (to_string(student.id) == searchTerm);
                break;
            case 2:
                match = (student.fullName.find(searchTerm) != string::npos);
                break;
            case 3:
                match = (student.direction.find(searchTerm) != string::npos);
                break;
            case 4:
                match = (student.group.find(searchTerm) != string::npos);
                break;
        }
        if (match) results.push_back(student);
    }

    if (results.empty()) {
        cout << "\nНичего не найдено!" << endl;
        return;
    }

    cout << "\nРезультаты поиска:" << endl;
    cout << setw(5) << "ID" << " | " << setw(30) << "ФИО" << " | " << setw(8) << "Возраст"
         << " | " << setw(20) << "Направление" << " | " << setw(10) << "Группа" << " | " << setw(12) << "Дата начала" << endl;
    cout << string(100, '-') << endl;

    for (const auto& student : results) {
        cout << setw(5) << student.id << " | " << setw(30) << student.fullName << " | " 
             << setw(8) << student.age << " | " << setw(20) << student.direction << " | "
             << setw(10) << student.group << " | " << setw(12) << student.startDate << endl;
    }
}

void showMenu() {
    cout << "\nБаза данных учеников Кванториума" << endl;
    cout << "1. Просмотр всех учеников" << endl;
    cout << "2. Добавить ученика" << endl;
    cout << "3. Редактировать ученика" << endl;
    cout << "4. Удалить ученика" << endl;
    cout << "5. Поиск ученика" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

int main() {
    setlocale(LC_ALL, "Russian");
    loadData();

    while (true) {
        showMenu();
        int choice;
        cin >> choice;

        switch(choice) {
            case 0:
                cout << "\nДо свидания!" << endl;
                return 0;
            case 1:
                displayStudents();
                break;
            case 2:
                addStudent();
                break;
            case 3:
                editStudent();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                searchStudent();
                break;
            default:
                cout << "\nНеверный выбор! Попробуйте снова." << endl;
        }
    }

    return 0;
} 