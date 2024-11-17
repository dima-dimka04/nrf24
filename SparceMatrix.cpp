#include "SparceMatrix.hpp"
using namespace std;

CRSMatrix sparse_transpose(const CRSMatrix& input) {
    int y = 0;
    int g = 0, l = 0, h = 0;

    // результирующие массивы
    CRSMatrix res{
        input.m,
        input.n,
        input.k,
        vector<double>(input.k, 0.0),
        vector<int>(input.k, 0),
        vector<int>(input.m + 1, 0)
    };

    double Arr[10][10]; // массивы, созданyые для демонстрации 
    double ArrT[10][10]; // работы примера

    // Вывод изначальных данных
    cout << "CSR matrix:\nIA: ";
    for (int i = 0; i < input.m + 1; i++) {
        cout << input.IA[i] << " ";
    }
    cout << endl << "JA: ";
    for (int i = 0; i < input.k; i++) {
        cout << input.JA[i] << " ";
    }
    cout << endl << "AN: ";
    for (int i = 0; i < input.k; i++) {
        cout << input.AN[i] << " ";
    }
    cout << endl << endl;

    // Только для примера на небольших матрицах
    cout << "Matrix:" << endl;
    for (int i = 0; i < input.m; i++) {
        for (int j = 0; j < input.m; j++) {
            l = input.JA[g];
            h = input.IA[i + 1] - input.IA[i];
            if ((j == l) && (h > 0)) {
                Arr[i][l] = input.AN[g];
                g++;
                h--;
            }
            else {
                Arr[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < input.m; i++) {
        for (int j = 0; j < input.m; j++) {
            cout << Arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Создание N векторов 
    vector<vector<int> > IntV(input.JA.size(), vector<int>());
    vector<vector<double> > DoubleV(input.AN.size(), vector<double>());

    // Заполнение N векторов
    for (int i = 0; i <= input.m - 1; i++) {
        for (int j = input.IA[i]; j < input.IA[i + 1]; j++) {
            int c = input.JA[j];
            IntV[c].resize(IntV[c].size() + 1);
            DoubleV[c].resize(DoubleV[c].size() + 1);
            IntV[c][IntV[c].size() - 1] = i;
            DoubleV[c][DoubleV[c].size() - 1] = input.AN[j];
        }
    }

    // Копирование в результирующие массивы
    for (int i = 0; i <= input.m - 1; i++) {
        for (int j = 0; j < IntV[i].size(); j++) {
            res.JA[y] = IntV[i][j];
            res.AN[y] = DoubleV[i][j];
            res.IA[i + 1] = res.IA[i] + IntV[i].size();
            y++;
        }
    }

    // Вывод конечных данных
    cout << "-------------------------------------------------\n\nResult:" << endl;
    cout << "IntV:" << endl;
    for (int i = 0; i < input.m; i++) {
        for (int j = 0; j < IntV[i].size(); j++) {
            cout << IntV[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl << "DoubleV:" << endl;
    for (int i = 0; i < input.m; i++) {
        for (int j = 0; j < DoubleV[i].size(); j++) {
            cout << DoubleV[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl << "CSR matrix(t):\nIAt: ";
    for (int i = 0; i < input.m + 1; i++) {
        cout << res.IA[i] << " ";
    }
    cout << endl << "JAt: ";
    for (int i = 0; i < input.k; i++) {
        cout << res.JA[i] << " ";
    }
    cout << endl << "ANt: ";
    for (int i = 0; i < input.k; i++) {
        cout << res.AN[i] << " ";
    }
    cout << endl;

    g = 0, h = 0;
    cout << endl << "Matrix(t):" << endl;
    for (int i = 0; i < input.m; i++) {
        h = res.IA[i + 1] - res.IA[i];
        for (int j = 0; j < input.n; j++) {
            l = res.JA[g];
            if ((j == l) && (h > 0)) {
                ArrT[i][l] = res.AN[g];
                g++;
                h--;
            }
            else {
                ArrT[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < input.m; i++) {
        for (int j = 0; j < input.n; j++) {
            cout << ArrT[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    IntV.clear();
    DoubleV.clear();

    return res;
}

int main()
{
    // исходные данные
    CRSMatrix testCRS = {
        10,
        10,
        22,
        vector<double> {3,2,7,8,6,4,5,3,1,7,8,2,3,2,3,4,8,1,3,6,7,9},
        vector<int> {0,4,9,2,7,4,8,0,6,3,2,8,0,1,6,2,5,1,5,3,5,9},
        vector<int> {0,3,5,7,9,10,12,15,17,19,22}
    };
    CRSMatrix returner = sparse_transpose(testCRS);

    return 0;
}