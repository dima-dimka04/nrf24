#pragma once
#include <iostream>
#include <vector> 

using namespace std;

struct CRSMatrix
{
    int m; // кол-во строк
    int n; // кол-во столбцов
    int k; // кол-во ненулевых элементов
    vector<double> AN; // массив значений
    vector<int> JA; // массив индексов столбцов
    vector<int> IA; // массив индексации строк
};
CRSMatrix sparse_transpose(const CRSMatrix& input);