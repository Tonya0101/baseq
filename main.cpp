/**
 * База данных учеников Кванториума
 * Консольное приложение для управления базой данных учеников
 */

// Стандартные библиотеки C++
#include <iostream>    // Для ввода/вывода в консоли
#include <fstream>     // Для работы с файлами (чтение и запись)
#include <string>      // Для работы со строками (std::string, std::wstring)
#include <vector>      // Для использования динамических массивов (std::vector)
#include <locale>      // Для работы с локалью, поддержка разных языков и кодировок
#include <codecvt>     // Для конвертации между различными кодировками (например, UTF-8 и UTF-16)

// Библиотеки Windows для поддержки русского языка в консоли
#include <windows.h>   // Для работы с консолью Windows (например, установка кодировки)
#include <fcntl.h>     // Для управления режимом ввода/вывода (например, для Unicode)
#include <io.h>        // Для работы с низкоуровневыми операциями ввода/вывода

using namespace std;

/**
 * Структура для хранения данных об ученике
 */
struct Uchenik {
    int id;              // Уникальный идентификатор ученика
    wstring fio;         // ФИО ученика
    int vozrast;         // Возраст ученика
    wstring napravlenie; // Направление обучения
    wstring gruppa;      // Группа ученика
    wstring dataNachala; // Дата начала обучения
};

// Глобальные переменные
vector<Uchenik> spisokUchenikov;  // Список всех учеников
const string imyaFaila = "students.txt";  // Имя файла для сохранения данных
int sleduyushiyId = 1;            // Следующий доступный ID для нового ученика

/**
 * Загружает данные из файла в память
 */
void zagruzitDannye() {
    wifstream fail(imyaFaila);  // Открытие файла для чтения
    if (!fail.is_open()) {  // Если файл не найден, выводим сообщение
        wcout << L"Файл с данными не найден. Будет создан новый файл." << endl;
        return;
    }
    
    fail.imbue(locale(fail.getloc(), new codecvt_utf8<wchar_t>));  // Установка кодировки для работы с UTF-8

    spisokUchenikov.clear();  // Очищаем список перед загрузкой данных
    wstring stroka;
    while (getline(fail, stroka)) {  // Чтение файла построчно
        Uchenik uchenik;
        size_t poz = 0;
        wstring chast;
        int pole = 0;
        
        // Разделение строки на части по символу "|"
        while ((poz = stroka.find(L"|")) != wstring::npos) {
            chast = stroka.substr(0, poz);  // Извлекаем часть строки
            switch(pole) {
                case 0: uchenik.id = stoi(chast); break;  // Преобразуем строку в целое число и присваиваем ID
                case 1: uchenik.fio = chast; break;  // Присваиваем ФИО
                case 2: uchenik.vozrast = stoi(chast); break;  // Присваиваем возраст
                case 3: uchenik.napravlenie = chast; break;  // Присваиваем направление
                case 4: uchenik.gruppa = chast; break;  // Присваиваем группу
                case 5: uchenik.dataNachala = chast; break;  // Присваиваем дату начала обучения
            }
            stroka.erase(0, poz + 1);  // Удаляем уже обработанную часть строки
            pole++;
        }
        spisokUchenikov.push_back(uchenik);  // Добавляем ученика в список
        if (uchenik.id >= sleduyushiyId) {  // Обновляем значение следующего ID
            sleduyushiyId = uchenik.id + 1;
        }
    }
    fail.close();  // Закрываем файл
}

/**
 * Сохраняет данные из памяти в файл
 */
void sohranitDannye() {
    wofstream fail(imyaFaila);  // Открытие файла для записи
    if (!fail.is_open()) {  // Если файл не может быть открыт
        wcout << L"Ошибка при сохранении файла!" << endl;
        return;
    }
    
    fail.imbue(locale(fail.getloc(), new codecvt_utf8<wchar_t>));  // Установка кодировки для работы с UTF-8

    // Запись всех учеников в файл
    for (const auto& uchenik : spisokUchenikov) {
        fail << uchenik.id << L"|" 
             << uchenik.fio << L"|" 
             << uchenik.vozrast << L"|"
             << uchenik.napravlenie << L"|" 
             << uchenik.gruppa << L"|" 
             << uchenik.dataNachala << L"|" << endl;
    }
    fail.close();  // Закрываем файл
}



/**
 * Добавляет нового ученика в базу данных
 */
void dobavitUchenika() {
    Uchenik uchenik;
    uchenik.id = sleduyushiyId++;  // Присваиваем новому ученику уникальный ID
    
    wcin.ignore();  // Очистка буфера ввода перед получением строк

    // Ввод данных ученика
    wcout << L"\nВведите ФИО: ";
    getline(wcin, uchenik.fio);
    
    wcout << L"Введите возраст: ";
    wcin >> uchenik.vozrast;
    wcin.ignore();
    
    wcout << L"Введите направление: ";
    getline(wcin, uchenik.napravlenie);
    
    wcout << L"Введите группу: ";
    getline(wcin, uchenik.gruppa);
    
    wcout << L"Введите дату начала обучения: ";
    getline(wcin, uchenik.dataNachala);
    
    spisokUchenikov.push_back(uchenik);  // Добавляем ученика в список
    sohranitDannye();  // Сохраняем данные в файл
    wcout << L"\nУченик успешно добавлен!" << endl;
}

/**
 * Выводит список всех учеников
 */
void pokazatUchenikov() {
    if (spisokUchenikov.empty()) {  // Если база данных пуста
        wcout << L"\nБаза данных пуста!" << endl;
        return;
    }

    wcout << L"\nСписок учеников:" << endl;
    wcout << L"ID | ФИО | Возраст | Направление | Группа | Дата начала" << endl;
    wcout << L"------------------------------------------------" << endl;

    // Вывод всех учеников
    for (const auto& uchenik : spisokUchenikov) {
        wcout << uchenik.id << L" | " 
              << uchenik.fio << L" | " 
              << uchenik.vozrast << L" | "
              << uchenik.napravlenie << L" | "
              << uchenik.gruppa << L" | "
              << uchenik.dataNachala << endl;
    }
}

/**
 * Редактирует данные об ученике по ID
 */
void redaktirovatUchenika() {
    int id;
    wcout << L"\nВведите ID ученика для редактирования: ";
    wcin >> id;
    wcin.ignore();

    bool naiden = false;  // Флаг, указывающий, найден ли ученик
    // Поиск ученика по ID и редактирование его данных
    for (auto& uchenik : spisokUchenikov) {
        if (uchenik.id == id) {
            naiden = true;
            
            wcout << L"\nВведите новые данные:" << endl;
            
            wcout << L"ФИО: ";
            getline(wcin, uchenik.fio);
            
            wcout << L"Возраст: ";
            wcin >> uchenik.vozrast;
            wcin.ignore();
            
            wcout << L"Направление: ";
            getline(wcin, uchenik.napravlenie);
            
            wcout << L"Группа: ";
            getline(wcin, uchenik.gruppa);
            
            wcout << L"Дата начала обучения: ";
            getline(wcin, uchenik.dataNachala);
            
            sohranitDannye();  // Сохраняем обновленные данные
            wcout << L"\nДанные ученика успешно обновлены!" << endl;
            break;
        }
    }
    
    if (!naiden) {  // Если ученик с таким ID не найден
        wcout << L"Ученик с таким ID не найден!" << endl;
    }
}

/**
 * Удаляет ученика из базы данных по ID
 */
void udalitUchenika() {
    int id;
    wcout << L"\nВведите ID ученика для удаления: ";
    wcin >> id;

    // Поиск ученика и его удаление
    for (int i = 0; i < spisokUchenikov.size(); i++) {
        if (spisokUchenikov[i].id == id) {
            spisokUchenikov.erase(spisokUchenikov.begin() + i);  // Удаление ученика из списка
            sohranitDannye();  // Сохраняем обновленный список в файл
            wcout << L"\nУченик успешно удален!" << endl;
            return;
        }
    }
    
    wcout << L"Ученик с таким ID не найден!" << endl;
}

/**
 * Выводит главное меню программы
 */
void pokazatMenu() {
    wcout << L"\nБаза данных учеников Кванториума" << endl;
    wcout << L"1. Просмотр всех учеников" << endl;
    wcout << L"2. Добавить ученика" << endl;
    wcout << L"3. Редактировать ученика" << endl;
    wcout << L"4. Удалить ученика" << endl;
    wcout << L"0. Выход" << endl;
    wcout << L"Выберите действие: ";
}



/**
 * Главная функция программы
 */
int main() {
    SetConsoleCP(CP_UTF8);         // Устанавливаем кодировку ввода в UTF-8
    SetConsoleOutputCP(CP_UTF8);   // Устанавливаем кодировку вывода в UTF-8
    
    _setmode(_fileno(stdin), _O_U16TEXT);   // Устанавливаем режим для чтения Unicode
    _setmode(_fileno(stdout), _O_U16TEXT);  // Устанавливаем режим для вывода Unicode
    
    zagruzitDannye();  // Загружаем данные из файла

    int vybor;
    while (true) {
        pokazatMenu();  // Отображаем меню
        wcin >> vybor;

        // Обработка выбора пользователя
        switch(vybor) {
            case 0: return 0;  // Выход из программы
            case 1: pokazatUchenikov(); break;  // Просмотр всех учеников
            case 2: dobavitUchenika(); break;  // Добавление нового ученика
            case 3: redaktirovatUchenika(); break;  // Редактирование данных ученика
            case 4: udalitUchenika(); break;  // Удаление ученика
            default: wcout << L"\nНеверный выбор! Попробуйте снова." << endl;  // Неверный выбор
        }
    }
}
