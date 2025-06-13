#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <cmath>

using namespace std;

// класс для работы отдельночленом многочлена
class PolynomialTerm {
private:
    long long coefficient;              // коэффициент
    int power;                          // степень
    char var;                           // символ переменной
    PolynomialTerm* next;               // указатель на следующий элемент

public:
    // конструктор
    PolynomialTerm(long long coeff = 0, int pow = 0, char varChar = '\0', PolynomialTerm* nxt = nullptr) {
        coefficient = coeff;
        power = pow;
        var = varChar;
        next = nxt;
    }

    // геттеры
    long long GetCoefficient() { return coefficient; }             // геттер коэфф.
    int GetPower() { return power; }                               // геттер степени
    char GetVariableChar() { return var; }                         // геттер переменной
    PolynomialTerm* GetNext() { return next; }                     // геттер указаталея на след. член

    // сеттеры
    void SetCoefficient(long long coeff) { coefficient = coeff; }  // сеттер коэфф.
    void SetPower(int pow) { power = pow; }                        // сеттер степени
    void SetVariableChar(char varChar) { var = varChar; }          // сеттер переменной
    void SetNext(PolynomialTerm* nxt) { next = nxt; }              // сеттер указателя на след. член
};

// класс для работы с целом многочленом
class Polynomial {
private:
    PolynomialTerm* head;

public:
    // конструктор
    Polynomial() { head = nullptr; }

    // деструктор
    ~Polynomial() { Clear(); }

    // методы для работы с многочленом
    void Clear();                                                   // очистка
    void AddTerm(long long coefficient, int power, char var);       // добавление слагаемого
    void Print();                                                   // вывод многочлена
    void CombineLikeTerms();                                        // приведение подобных
    void Sort();                                                    // упорядочивание

    // чтение и запись многочлена в файл
    bool ReadFromFile(string& filename);
    void WriteToFile(string& filename);
};

// функции для безопасного ввода
int GetIntInput(string& prompt);            // для целых чисел
double GetDoubleInput(string& prompt);      // для дробных
string GetStringInput(string& prompt);      // строковое
