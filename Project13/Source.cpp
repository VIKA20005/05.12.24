#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <cctype>
#include <sstream>

using namespace std;

// Функция для проверки, является ли строка допустимым тегом
bool is_valid_tag(const string& tag) {
    // Проверка на пустоту или отсутствие символов '<' или '>'
    if (tag.empty() || tag[0] != '<' || tag[tag.length() - 1] != '>') {
        return false;  // Тег должен начинаться с '<' и заканчиваться на '>'
    }
    // Проверка на корректность имени тега (буквы, цифры или '/')
    for (size_t i = 1; i < tag.length() - 1; ++i) {
        if (!isalnum(tag[i]) && tag[i] != '/') {
            return false;  // Допускаются только буквы, цифры и '/'
        }
    }
    return true;
}

// Функция для извлечения имени тега из строки
string extract_tag_name(const string& tag) {
    size_t start = (tag[1] == '/') ? 2 : 1;  // Пропускаем символ '<' или '</'
    size_t end = tag.find_first_of(" >", start);  // Ищем конец тега
    return tag.substr(start, end - start);  // Извлекаем имя тега
}

// Функция для проверки валидности HTML-файла
bool validate_html(const string& file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Ошибка: не удается открыть файл: " << file_path << endl;
        return false;
    }

    stack<string> tag_stack;  // Стек для хранения открытых тегов
    string line;
    int line_number = 0;

    while (getline(file, line)) {
        line_number++;

        size_t pos = 0;
        while ((pos = line.find('<', pos)) != string::npos) {
            size_t end_pos = line.find('>', pos);  // Находим конец тега
            if (end_pos == string::npos) {
                cerr << "Ошибка в строке " << line_number << ": Некорректный тег (нет символа '>')." << endl;
                return false;
            }

            string tag = line.substr(pos, end_pos - pos + 1);  // Извлекаем тег
            pos = end_pos + 1;  // Переходим к следующей части строки

            // Проверка на валидность тега
            if (!is_valid_tag(tag)) {
                cerr << "Ошибка в строке " << line_number << ": Невалидный тег: " << tag << endl;
                return false;
            }

            string tag_name = extract_tag_name(tag);  // Извлекаем имя тега

            // Если это открывающий тег, добавляем его в стек
            if (tag[1] != '/') {
                tag_stack.push(tag_name);
            }
            else {
                // Если это закрывающий тег, проверяем его с верхним элементом стека
                if (tag_stack.empty() || tag_stack.top() != tag_name) {
                    cerr << "Ошибка в строке " << line_number << ": Несоответствующий закрывающий тег: </" << tag_name << ">" << endl;
                    return false;
                }
                tag_stack.pop();
            }
        }
    }

    // Если стек не пуст, значит, есть незакрытые теги
    if (!tag_stack.empty()) {
        cerr << "Ошибка: Невалидный HTML. Пропущены закрывающие теги." << endl;
        return false;
    }

    return true;
}

int main() {
    string file_path;

    cout << "Введите путь к HTML-файлу: ";
    cin >> file_path;

    if (validate_html(file_path)) {
        cout << "Файл " << file_path << " валиден." << endl;
    }
    else {
        cout << "Файл " << file_path << " не валиден." << endl;
    }

    return 0;
}