#include "header.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");
    
    cout << "\t\n=== ��������� ��� ������ � ������������, ��������������� � ���� ������ ��������� ===\n";
    cout << "                ��������� ����� ���: P(x) = a_n * x^n + a_n-1 * x^n-1 + ... + a_1 * x a_0\n\n";

    cout << "�������� ��������� ���� � �����������, ���� �� ��� �������, �� \n";

    string input = "������� ��� ����� -> ";
    string filename = GetStringInput(input);
    
    // ������� ������ ����������
    Polynomial polynomial;
    
    // ������ ��������� �� �����
    if (!polynomial.ReadFromFile(filename)) {
        return 1;
    }
    
    cout << "�������� ��������� -> ";
    polynomial.Print();
    cout << endl;
    
    // �������� �������� �����
    polynomial.CombineLikeTerms();
    
    cout << "��������� ����� ���������� �������� ������ -> ";
    polynomial.Print();
    
    // ���������� ��������� � ���� (���� ����)
    polynomial.WriteToFile(filename);

    return 0;
}