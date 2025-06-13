#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <cmath>

using namespace std;

// ����� ��� ������ �������������� ����������
class PolynomialTerm {
private:
    long long coefficient;              // �����������
    int power;                          // �������
    char var;                           // ������ ����������
    PolynomialTerm* next;               // ��������� �� ��������� �������

public:
    // �����������
    PolynomialTerm(long long coeff = 0, int pow = 0, char varChar = '\0', PolynomialTerm* nxt = nullptr) {
        coefficient = coeff;
        power = pow;
        var = varChar;
        next = nxt;
    }

    // �������
    long long GetCoefficient() { return coefficient; }             // ������ �����.
    int GetPower() { return power; }                               // ������ �������
    char GetVariableChar() { return var; }                         // ������ ����������
    PolynomialTerm* GetNext() { return next; }                     // ������ ���������� �� ����. ����

    // �������
    void SetCoefficient(long long coeff) { coefficient = coeff; }  // ������ �����.
    void SetPower(int pow) { power = pow; }                        // ������ �������
    void SetVariableChar(char varChar) { var = varChar; }          // ������ ����������
    void SetNext(PolynomialTerm* nxt) { next = nxt; }              // ������ ��������� �� ����. ����
};

// ����� ��� ������ � ����� �����������
class Polynomial {
private:
    PolynomialTerm* head;

public:
    // �����������
    Polynomial() { head = nullptr; }

    // ����������
    ~Polynomial() { Clear(); }

    // ������ ��� ������ � �����������
    void Clear();                                                   // �������
    void AddTerm(long long coefficient, int power, char var);       // ���������� ����������
    void Print();                                                   // ����� ����������
    void CombineLikeTerms();                                        // ���������� ��������
    void Sort();                                                    // ��������������

    // ������ � ������ ���������� � ����
    bool ReadFromFile(string& filename);
    void WriteToFile(string& filename);
};

// ������� ��� ����������� �����
int GetIntInput(string& prompt);            // ��� ����� �����
double GetDoubleInput(string& prompt);      // ��� �������
string GetStringInput(string& prompt);      // ���������
