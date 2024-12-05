#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <cctype>
#include <sstream>

using namespace std;

// ������� ��� ��������, �������� �� ������ ���������� �����
bool is_valid_tag(const string& tag) {
    // �������� �� ������� ��� ���������� �������� '<' ��� '>'
    if (tag.empty() || tag[0] != '<' || tag[tag.length() - 1] != '>') {
        return false;  // ��� ������ ���������� � '<' � ������������� �� '>'
    }
    // �������� �� ������������ ����� ���� (�����, ����� ��� '/')
    for (size_t i = 1; i < tag.length() - 1; ++i) {
        if (!isalnum(tag[i]) && tag[i] != '/') {
            return false;  // ����������� ������ �����, ����� � '/'
        }
    }
    return true;
}

// ������� ��� ���������� ����� ���� �� ������
string extract_tag_name(const string& tag) {
    size_t start = (tag[1] == '/') ? 2 : 1;  // ���������� ������ '<' ��� '</'
    size_t end = tag.find_first_of(" >", start);  // ���� ����� ����
    return tag.substr(start, end - start);  // ��������� ��� ����
}

// ������� ��� �������� ���������� HTML-�����
bool validate_html(const string& file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "������: �� ������� ������� ����: " << file_path << endl;
        return false;
    }

    stack<string> tag_stack;  // ���� ��� �������� �������� �����
    string line;
    int line_number = 0;

    while (getline(file, line)) {
        line_number++;

        size_t pos = 0;
        while ((pos = line.find('<', pos)) != string::npos) {
            size_t end_pos = line.find('>', pos);  // ������� ����� ����
            if (end_pos == string::npos) {
                cerr << "������ � ������ " << line_number << ": ������������ ��� (��� ������� '>')." << endl;
                return false;
            }

            string tag = line.substr(pos, end_pos - pos + 1);  // ��������� ���
            pos = end_pos + 1;  // ��������� � ��������� ����� ������

            // �������� �� ���������� ����
            if (!is_valid_tag(tag)) {
                cerr << "������ � ������ " << line_number << ": ���������� ���: " << tag << endl;
                return false;
            }

            string tag_name = extract_tag_name(tag);  // ��������� ��� ����

            // ���� ��� ����������� ���, ��������� ��� � ����
            if (tag[1] != '/') {
                tag_stack.push(tag_name);
            }
            else {
                // ���� ��� ����������� ���, ��������� ��� � ������� ��������� �����
                if (tag_stack.empty() || tag_stack.top() != tag_name) {
                    cerr << "������ � ������ " << line_number << ": ����������������� ����������� ���: </" << tag_name << ">" << endl;
                    return false;
                }
                tag_stack.pop();
            }
        }
    }

    // ���� ���� �� ����, ������, ���� ���������� ����
    if (!tag_stack.empty()) {
        cerr << "������: ���������� HTML. ��������� ����������� ����." << endl;
        return false;
    }

    return true;
}

int main() {
    string file_path;

    cout << "������� ���� � HTML-�����: ";
    cin >> file_path;

    if (validate_html(file_path)) {
        cout << "���� " << file_path << " �������." << endl;
    }
    else {
        cout << "���� " << file_path << " �� �������." << endl;
    }

    return 0;
}