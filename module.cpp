#include "header.h"

using namespace std;

// ������� ����������
void Polynomial::Clear() {
    while (head != nullptr) {
        PolynomialTerm* temp = head;
        head = head->GetNext();
        delete temp;
    }
}

// ���������� ������ ����������
void Polynomial::AddTerm(long long coefficient, int power, char var) {
    PolynomialTerm* newTerm = new PolynomialTerm(coefficient, power, var, head);
    head = newTerm;
}

// ����� ����������
void Polynomial::Print() {
    if (head == nullptr) {
        cout << "0";
        return;
    }

    bool first = true;
    PolynomialTerm* current = head;
    while (current != nullptr) {
        if (current->GetCoefficient() != 0) {
            // ����� �����
            if (!first) {
                cout << (current->GetCoefficient() > 0 ? " + " : " - ");
            }
            // ����� ������������
            if (abs(current->GetCoefficient()) != 1 || current->GetPower() == 0) {
                cout << abs(current->GetCoefficient());
            }
            // ����� ���������� � �������
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

// �������� �������� �����
void Polynomial::CombineLikeTerms() {
    if (head == nullptr) return;

    Sort(); // ������� ��������� ������� ���������

    Polynomial newPoly; // ������� ��������� ��������� ��� ������ ������ ������
    PolynomialTerm* current = head;

    while (current != nullptr) {
        long long combinedCoeff = current->GetCoefficient();
        int currentPower = current->GetPower();
        char currentVar = current->GetVariableChar();

        // �������� �� ��������� ������, ����� ����� �������� � ����������
        PolynomialTerm* runner = current->GetNext();
        while (runner != nullptr && 
               currentPower == runner->GetPower() && 
               currentVar == runner->GetVariableChar()) {
            
            // �������� ������������ ��� ��������
            if (runner->GetCoefficient() > 0) {
                if (combinedCoeff > LLONG_MAX - runner->GetCoefficient()) {
                    cout << "������: ������������ ��� �������� �������������" << endl;
                    return;
                }
            } else {
                if (combinedCoeff < LLONG_MIN - runner->GetCoefficient()) {
                    cout << "������: ������������ ��� �������� �������������" << endl;
                    return;
                }
            }
            
            combinedCoeff += runner->GetCoefficient();
            runner = runner->GetNext();
        }

        // ���� ������������ ����������� �� ����� ����, ��������� ���� � ����� ���������
        if (combinedCoeff != 0) {
            newPoly.AddTerm(combinedCoeff, currentPower, currentVar);
        }

        // ��������� � ������� �����, ������� ��� �� ��� ��������� (�.�. � 'runner')
        current = runner; 
    }

    // ������� ������ ���������
    Clear();

    // ��������� �������� �� ���������� ���������� � �������� ���������
    current = newPoly.head;
    newPoly.head = nullptr; // ����������� ������ �� newPoly, ����� ��� ���������� �� ������ ����

    while (current != nullptr) {
        // ��������� ���� � 'this' � ��� �� �������, � ������� ��� ���� � newPoly
        AddTerm(current->GetCoefficient(), current->GetPower(), current->GetVariableChar());

        PolynomialTerm* temp = current;
        current = current->GetNext();
        delete temp; // ������� ���� ����� ����, ��� ��� ������ ���� ������������
    }
    Sort();
}

// �������������� ������ ���������� �� �������� �������
void Polynomial::Sort() {
    if (head == nullptr || head->GetNext() == nullptr) return;

    // ������� ������ ���������� �� ����� ����������
    const int MAX_TERMS = 10000;
    PolynomialTerm* terms[MAX_TERMS];
    int count = 0;

    // ��������� ������ ����������� �� �����
    PolynomialTerm* current = head;
    while (current != nullptr && count < MAX_TERMS) {
        terms[count++] = current;
        current = current->GetNext();
    }

    if (count >= MAX_TERMS) {
        cout << "������: ��������� ������������ ���������� ������ ����������" << endl;
        return;
    }

    // ��������� ������ �� ������� (��������)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (terms[j]->GetPower() < terms[j + 1]->GetPower()) {
                swap(terms[j], terms[j + 1]);
            }
        }
    }

    // ������������� ������ � ����� �������
    head = terms[0];
    for (int i = 0; i < count - 1; i++) {
        terms[i]->SetNext(terms[i + 1]);
    }
    terms[count - 1]->SetNext(nullptr);
}

// ������ ���������� �� �����
bool Polynomial::ReadFromFile(string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "������ �������� �����! ���������, ��� ���� ���������� � ��������." << endl;
        return false;
    }

    string line; 
    // �������� �� ������ ������/������ �����������
    while (getline(file, line)) {

        // ���������� ������ ������
        if (line.empty()) {
            continue;
        }

        if (line.length() >= 2 && line[0] == '/' && line[1] == '/') {
            continue;
        }

        // ���������, �������� �� ������ ������� ����������� ����� ����������
        // ���������� �������: �����, ����� (+, -), ��� ����� (����������)
        char firstChar = line[0];
        if (!isdigit(firstChar) && firstChar != '+' && firstChar != '-' && !isalpha(firstChar)) {
            cout << "��������������: ��������� ������, ������������ � ������������ ������� '"
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

        // ���������� �������
        while (i < line.length() && (line[i] == ' ' || line[i] == '\t')) {
            i++;
        }
        if (i >= line.length()) break;

        // ������ ����
        int sign = 1;
        char currentChar = line[i];
        // ��������� �������� +/-
        if (currentChar == '-' || currentChar == 150) { 
            sign = -1;
            i++;
        } else if (currentChar == '+') {
            sign = 1;
            i++;
        }

        // ���������� �������
        while (i < line.length() && (line[i] == ' ' || line[i] == '\t')) {
            i++;
        }

        // ������ �����������
        long long coefficient = 0;
        bool hasExplicitCoefficient = false; 
        
        if (i < line.length() && isdigit(line[i])) {
            while (i < line.length() && isdigit(line[i])) {
                // �������� ������������
                if (coefficient > LLONG_MAX / 10) {
                    cout << "������: ����������� ��������� ���������� ��������" << endl;
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
        
        // �������� ������������ ����� ��������� �� ����
        if (sign == -1 && coefficient > LLONG_MAX) {
            cout << "������: ����������� ��������� ���������� ��������" << endl;
            parsingError = true;
            break;
        }
        coefficient *= sign;

        // ���������� �������
        while (i < line.length() && (line[i] == ' ' || line[i] == '\t')) {
            i++;
        }

        // ������ ����������
        int power = 0;
        char termVariableChar = '\0';
        if (i < line.length() && isalpha(line[i])) {
            termVariableChar = line[i];
            i++;
            power = 1;

            // ���������� �������
            while (i < line.length() && (line[i] == ' ' || line[i] == '\t')) {
                i++;
            }

            // ������ �������
            if (i < line.length() && line[i] == '^') {
                i++; 

                // ���������� �������
                while (i < line.length() && (line[i] == ' ' || line[i] == '\t')) {
                    i++;
                }

                // ������ �������� �������
                int explicitPowerValue = 0;
                if (i < line.length() && isdigit(line[i])) {
                    while (i < line.length() && isdigit(line[i])) {
                        explicitPowerValue = explicitPowerValue * 10 + (line[i] - '0');
                        i++;
                    }
                    power = explicitPowerValue;
                } else {
                    cout << "������: ������������ �������� ������� � ����� "
                        << startOfTermParse << ". ��������� ����." << endl;
                    parsingError = true;
                    break; 
                }
            }
        } 
        // ��������� �� 0
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
            cout << "\n������: ������������ ���� ���������� � ������� "
                << startOfTermParse << ".\n��������� ����." << endl;
            cout << "������: ����������� ������ \'" << line[startOfTermParse] << "\' (ASCII: "
                << (int)line[startOfTermParse] << ").\n��������� ����." << endl;
            parsingError = true;
            break; 
        }

        // �������� �� ������� ����� (���� �� ������ �� ��������� �� �������� �����)
        if (i == startOfTermParse) {
            cout << "������: ������ ���� ���������� � ������� "
                << startOfTermParse << ". ��������� ����." << endl;
            parsingError = true;
            break; 
        }

        // ��������� ���� � ���������
        AddTerm(coefficient, power, termVariableChar);
        cout << "�������� ����: �����������=" << coefficient << ", �������=" << power
            << ", ����������='" << termVariableChar << "'" << endl;

        // ���� ���� ������ ��������, ������� �� �����
        if (parsingError) {
            break;
        }
    }

    return !parsingError;
}

// ������ ���������� � ����
void Polynomial::WriteToFile(string& filename) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "������ �������� ����� ��� ������!" << endl;
        return;
    }

    // ���������, ��� �� ���� ���� �� ������� ������
    if (file.tellp() > 0) {
        file << endl << endl;; // ������-�����������
    }

    PolynomialTerm* current = head;
    bool isFirstTerm = true;

    while (current != nullptr) {
        long long coeff = current->GetCoefficient();
        int power = current->GetPower();
        char var = current->GetVariableChar();

        // ����� �����
        if (isFirstTerm) {
            if (coeff < 0) file << "-";
            isFirstTerm = false;
        } else {
            file << (coeff >= 0 ? " + " : " - ");
        }

        // ����� ������������
        if (abs(coeff) != 1 || power == 0) {
            file << abs(coeff);
        }

        // ����� ���������� � �������
        if (power > 0) {
            file << var;
            if (power > 1) {
                file << "^" << power;
            }
        }

        current = current->GetNext();

    }
    file << endl; // ��������� ����� ������ ����� ����������
    file.close();
}

// ���������� ���� ������ �����
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
            cout << "������: ������� ���������� ����� �����!\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    return result;
}

// ���������� ���� ������������� �����
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
            cout << "������: ������� ���������� ������������ �����!\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    return result;
}

// ���������� ���� ������
string GetStringInput(string& input) {
    string value;
    cout << input;
    getline(cin, value);
    while (value.empty()) {
        cout << "������: ������ �� ������ ���� ������!\n";
        cout << input; getline(cin, value);
    }
    return value;
}