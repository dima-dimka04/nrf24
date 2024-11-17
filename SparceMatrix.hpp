#pragma once
#include <iostream>
#include <vector> 

using namespace std;

struct CRSMatrix
{
    int m; // ���-�� �����
    int n; // ���-�� ��������
    int k; // ���-�� ��������� ���������
    vector<double> AN; // ������ ��������
    vector<int> JA; // ������ �������� ��������
    vector<int> IA; // ������ ���������� �����
};
CRSMatrix sparse_transpose(const CRSMatrix& input);