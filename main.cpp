#include "header.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");
    
    cout << "\t\n=== Программа для работы с многочленами, представленными в виде списка слагаемых ===\n";
    cout << "                Многочлен имеет вид: P(x) = a_n * x^n + a_n-1 * x^n-1 + ... + a_1 * x a_0\n\n";

    cout << "Создайте текстовый файл с многочленом, если вы это сделали, то \n";

    string input = "Введите имя файла -> ";
    string filename = GetStringInput(input);
    
    // создаем объект многочлена
    Polynomial polynomial;
    
    // читаем многочлен из файла
    if (!polynomial.ReadFromFile(filename)) {
        return 1;
    }
    
    cout << "Исходный многочлен -> ";
    polynomial.Print();
    cout << endl;
    
    // приводим подобные члены
    polynomial.CombineLikeTerms();
    
    cout << "Многочлен после приведения подобных членов -> ";
    polynomial.Print();
    
    // записываем многочлен в файл (если надо)
    polynomial.WriteToFile(filename);

    return 0;
}