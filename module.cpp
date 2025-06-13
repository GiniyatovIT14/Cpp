#include "header.h"

using namespace std;

// очистка многочлена
void Polynomial::Clear() {
    while (head != nullptr) {
        PolynomialTerm* temp = head;
        head = head->GetNext();
        delete temp;
    }
}

// добавление нового слагаемого
void Polynomial::AddTerm(long long coefficient, int power, char var) {
    PolynomialTerm* newTerm = new PolynomialTerm(coefficient, power, var, head);
    head = newTerm;
}

// вывод многочлена
void Polynomial::Print() {
    if (head == nullptr) {
        cout << "0";
        return;
    }

    bool first = true;
    PolynomialTerm* current = head;
    while (current != nullptr) {
        if (current->GetCoefficient() != 0) {
            // вывод знака
            if (!first) {
                cout << (current->GetCoefficient() > 0 ? " + " : " - ");
            }
            // вывод коэффициента
            if (abs(current->GetCoefficient()) != 1 || current->GetPower() == 0) {
                cout << abs(current->GetCoefficient());
            }
            // вывод переменной и степени
            if (current->GetPower() > 0) {
                cout << current->GetVariableChar();
                if (current->GetPower() > 1) {
                    cout << "^" << current->GetPower();
                }
            }
            first = false;
        }
        current = current->GetNext();
    }
    if (first) cout << "0";
    cout << endl;
}

// приводит подобные члены
void Polynomial::CombineLikeTerms() {
    if (head == nullptr) return;

    Sort(); // сначала сортируем текущий многочлен

    Polynomial newPoly; // создаем временный многочлен для сборки нового списка
    PolynomialTerm* current = head;

    while (current != nullptr) {
        long long combinedCoeff = current->GetCoefficient();
        int currentPower = current->GetPower();
        char currentVar = current->GetVariableChar();

        // проходим по следующим членам, чтобы найти подобные и объединить
        PolynomialTerm* runner = current->GetNext();
        while (runner != nullptr && 
               currentPower == runner->GetPower() && 
               currentVar == runner->GetVariableChar()) {
            
            // проверка переполнения при сложении
            if (runner->GetCoefficient() > 0) {
                if (combinedCoeff > LLONG_MAX - runner->GetCoefficient()) {
                    cout << "Ошибка: переполнение при сложении коэффициентов" << endl;
                    return;
                }
            } else {
                if (combinedCoeff < LLONG_MIN - runner->GetCoefficient()) {
                    cout << "Ошибка: переполнение при сложении коэффициентов" << endl;
                    return;
                }
            }
            
            combinedCoeff += runner->GetCoefficient();
            runner = runner->GetNext();
        }

        // если объединенный коэффициент не равен нулю, добавляем член в новый многочлен
        if (combinedCoeff != 0) {
            newPoly.AddTerm(combinedCoeff, currentPower, currentVar);
        }

        // переходим к первому члену, который еще не был обработан (т.е. к 'runner')
        current = runner; 
    }

    // очищаем старый многочлен
    Clear();

    // переносим элементы из временного многочлена в основной многочлен
    current = newPoly.head;
    newPoly.head = nullptr; // отсоединяем список от newPoly, чтобы его деструктор не удалил узлы

    while (current != nullptr) {
        // добавляем узлы в 'this' в том же порядке, в котором они были в newPoly
        AddTerm(current->GetCoefficient(), current->GetPower(), current->GetVariableChar());

        PolynomialTerm* temp = current;
        current = current->GetNext();
        delete temp; // удаляем узел после того, как его данные были использованы
    }
    Sort();
}

// упорядочивание членов многочлена по убыванию степени
void Polynomial::Sort() {
    if (head == nullptr || head->GetNext() == nullptr) return;

    // создаем массив указателей на члены многочлена
    const int MAX_TERMS = 10000;
    PolynomialTerm* terms[MAX_TERMS];
    int count = 0;

    // заполняем массив указателями на члены
    PolynomialTerm* current = head;
    while (current != nullptr && count < MAX_TERMS) {
        terms[count++] = current;
        current = current->GetNext();
    }

    if (count >= MAX_TERMS) {
        cout << "Ошибка: превышено максимальное количество членов многочлена" << endl;
        return;
    }

    // сортируем массив по степени (убывание)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (terms[j]->GetPower() < terms[j + 1]->GetPower()) {
                swap(terms[j], terms[j + 1]);
            }
        }
    }

    // перестраиваем список в новом порядке
    head = terms[0];
    for (int i = 0; i < count - 1; i++) {
        terms[i]->SetNext(terms[i + 1]);
    }
    terms[count - 1]->SetNext(nullptr);
}

// чтение многочлена из файла
bool Polynomial::ReadFromFile(string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла! Убедитесь, что файл существует и доступен." << endl;
        return false;
    }

    string line; 
    // проверка на пустые строки/строки комментарии
    while (getline(file, line)) {

        // пропускаем пустые строки
        if (line.empty()) {
            continue;
        }

        if (line.length() >= 2 && line[0] == '/' && line[1] == '/') {
            continue;
        }

        // проверяем, является ли строка началом допустимого члена многочлена
        // допустимые символы: цифры, знаки (+, -), или буквы (переменные)
        char firstChar = line[0];
        if (!isdigit(firstChar) && firstChar != '+' && firstChar != '-' && !isalpha(firstChar)) {
            cout << "Предупреждение: Пропущена строка, начинающаяся с неизвестного символа '"
                << firstChar << "' (ASCII: " << firstChar << ")." << endl;
            continue;
        }
        break;
    }

    file.close();

    Clear();
    
    int i = 0;
    bool parsingError = false;

    while (i < line.length()) {
        int startOfTermParse = i;

        // пропускаем пробелы
        while (i < line.length() && (line[i] == ' ' || line[i] == '\t')) {
            i++;
        }
        if (i >= line.length()) break;

        // читаем знак
        int sign = 1;
        char currentChar = line[i];
        // обработка операнда +/-
        if (currentChar == '-' || currentChar == 150) { 
            sign = -1;
            i++;
        } else if (currentChar == '+') {
            sign = 1;
            i++;
        }

        // пропускаем пробелы
        while (i < line.length() && (line[i] == ' ' || line[i] == '\t')) {
            i++;
        }

        // читаем коэффициент
        long long coefficient = 0;
        bool hasExplicitCoefficient = false; 
        
        if (i < line.length() && isdigit(line[i])) {
            while (i < line.length() && isdigit(line[i])) {
                // проверка переполнения
                if (coefficient > LLONG_MAX / 10) {
                    cout << "Ошибка: коэффициент превышает допустимый диапазон" << endl;
                    parsingError = true;
                    break;
                }
                coefficient = coefficient * 10 + (line[i] - '0');
                i++;
            }
            hasExplicitCoefficient = true;
        } else {
            coefficient = 1; 
        }
        
        // проверка переполнения после умножения на знак
        if (sign == -1 && coefficient > LLONG_MAX) {
            cout << "Ошибка: коэффициент превышает допустимый диапазон" << endl;
            parsingError = true;
            break;
        }
        coefficient *= sign;

        // пропускаем пробелы
        while (i < line.length() && (line[i] == ' ' || line[i] == '\t')) {
            i++;
        }

        // читаем переменную
        int power = 0;
        char termVariableChar = '\0';
        if (i < line.length() && isalpha(line[i])) {
            termVariableChar = line[i];
            i++;
            power = 1;

            // пропускаем пробелы
            while (i < line.length() && (line[i] == ' ' || line[i] == '\t')) {
                i++;
            }

            // читаем степень
            if (i < line.length() && line[i] == '^') {
                i++; 

                // пропускаем пробелы
                while (i < line.length() && (line[i] == ' ' || line[i] == '\t')) {
                    i++;
                }

                // читаем значение степени
                int explicitPowerValue = 0;
                if (i < line.length() && isdigit(line[i])) {
                    while (i < line.length() && isdigit(line[i])) {
                        explicitPowerValue = explicitPowerValue * 10 + (line[i] - '0');
                        i++;
                    }
                    power = explicitPowerValue;
                } else {
                    cout << "Ошибка: некорректное значение степени в члене "
                        << startOfTermParse << ". Повторите ввод." << endl;
                    parsingError = true;
                    break; 
                }
            }
        } 
        // обработка на 0
        else if (i < line.length() && line[i] == '0' && hasExplicitCoefficient && i + 1 >= line.length()) {
            coefficient = 0;
            power = 0;
            termVariableChar = '\0';
            i++;
        } else if (i < line.length() && line[i] == '0') {
            coefficient = 0;
            power = 0;
            termVariableChar = '\0';
            i++;
        }else if (i < line.length() && !hasExplicitCoefficient && !isalpha(line[i]) && line[i] != '\0') {
            cout << "\nОшибка: некорректный член многочлена в позиции "
                << startOfTermParse << ".\nПовторите ввод." << endl;
            cout << "Ошибка: неизвестный символ \'" << line[startOfTermParse] << "\' (ASCII: "
                << (int)line[startOfTermParse] << ").\nПовторите ввод." << endl;
            parsingError = true;
            break; 
        }

        // проверка на пустоту члена (если мы ничего не прочитали из текущего терма)
        if (i == startOfTermParse) {
            cout << "Ошибка: пустой член многочлена в позиции "
                << startOfTermParse << ". Повторите ввод." << endl;
            parsingError = true;
            break; 
        }

        // добавляем член в многочлен
        AddTerm(coefficient, power, termVariableChar);
        cout << "Добавлен член: Коэффициент=" << coefficient << ", Степень=" << power
            << ", Переменная='" << termVariableChar << "'" << endl;

        // если есть ошибка парсинга, выходим из цикла
        if (parsingError) {
            break;
        }
    }

    return !parsingError;
}

// запись многочлена в файл
void Polynomial::WriteToFile(string& filename) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для записи!" << endl;
        return;
    }

    // проверяем, был ли файл пуст до текущей записи
    if (file.tellp() > 0) {
        file << endl << endl;; // строка-разделитель
    }

    PolynomialTerm* current = head;
    bool isFirstTerm = true;

    while (current != nullptr) {
        long long coeff = current->GetCoefficient();
        int power = current->GetPower();
        char var = current->GetVariableChar();

        // вывод знака
        if (isFirstTerm) {
            if (coeff < 0) file << "-";
            isFirstTerm = false;
        } else {
            file << (coeff >= 0 ? " + " : " - ");
        }

        // вывод коэффициента
        if (abs(coeff) != 1 || power == 0) {
            file << abs(coeff);
        }

        // вывод переменной и степени
        if (power > 0) {
            file << var;
            if (power > 1) {
                file << "^" << power;
            }
        }

        current = current->GetNext();

    }
    file << endl; // Добавляем новую строку после многочлена
    file.close();
}

// безопасный ввод целого числа
int GetIntInput(string& input) {
    string value;
    bool isValid = false;
    int result;

    while (!isValid) {
        cout << input;
        getline(cin, value);
        try {
            result = stoi(value);
            isValid = true;
        } catch (...) {
            cout << "Ошибка: введите корректное целое число!\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    return result;
}

// безопасный ввод вещественного числа
double GetDoubleInput(string& input) {
    string value;
    bool isValid = false;
    double result;

    while (!isValid) {
        cout << input;
        getline(cin, value);
        try {
            result = stod(value);
            isValid = true;
        } catch (...) {
            cout << "Ошибка: введите корректное вещественное число!\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    return result;
}

// безопасный ввод строки
string GetStringInput(string& input) {
    string value;
    cout << input;
    getline(cin, value);
    while (value.empty()) {
        cout << "Ошибка: строка не должна быть пустой!\n";
        cout << input; getline(cin, value);
    }
    return value;
}