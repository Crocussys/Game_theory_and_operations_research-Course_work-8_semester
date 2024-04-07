#include <fstream>
#include <iostream>
#include "mat.h"

Mat::Mat(char file_path[]){
    int inp;
    unsigned int size_n, size_m;
    fstream file(file_path, ios::in);
    if (!file){
        throw "FileError";
    }

    if (!(file >> inp)) throw "FileError";
    if (inp == 0){
        income = false;
    }else{
        income = true;
    }
    if (!(file >> size_n)) throw "FileError";
    if (!(file >> size_m)) throw "FileError";
    this->matrix.resize(size_n);
    for (int i = 0; i < size_n; i++){
        this->matrix[i].resize(size_m);
        for (int j = 0; j < size_m; j++){
            if (!(file >> this->matrix[i][j])) throw "FileError";
        }
    }
    if (!(file >> inp)) throw "FileError";
    if (inp == 1){
        probabilities_flag = true;
    }else{
        probabilities_flag = false;
    }
    if (probabilities_flag){
        this->probabilities.resize(size_m);
        for (int i = 0; i < size_m; i++){
            if (!(file >> this->probabilities[i])) throw "FileError";
        }
    }
    if (!(file >> optimism)){
        optimism = 0.5;
    }
    file.close();
}

template<typename T>
Maxmin<T> Mat::max(vector<T> arr){
    if (arr.size() < 1) throw "Array is empty";
    Maxmin<T> ans;
    ans.value = arr[0];
    ans.indexes.push_back(0);
    for (int i = 1; i < arr.size(); i++){
        if (arr[i] > ans.value){
            ans.value = arr[i];
            ans.indexes.clear();
            ans.indexes.push_back(i);
        }else if (arr[i] == ans.value){
            ans.indexes.push_back(i);
        }
    }
    return ans;
}

template<typename T>
Maxmin<T> Mat::min(vector<T> arr){
    if (arr.size() < 1) throw "Array is empty";
    Maxmin<T> ans;
    ans.value = arr[0];
    ans.indexes.push_back(0);
    for (int i = 1; i < arr.size(); i++){
        if (arr[i] < ans.value){
            ans.value = arr[i];
            ans.indexes.clear();
            ans.indexes.push_back(i);
        }else if (arr[i] == ans.value){
            ans.indexes.push_back(i);
        }
    }
    return ans;
}

vector<int> Mat::solution(bool print){
    vector<int> strategies(matrix.size());
    vector<vector<int>> criterions(4);
    criterions[0] = bayes_laplace(print);
    criterions[1] = wald(print);
    criterions[2] = savage(print);
    criterions[3] = hurwitz(print);
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < criterions[i].size(); j++){
            strategies[criterions[i][j]]++;
        }
    }
    Maxmin<int> ans = max(strategies);
    return ans.indexes;
}

void print_ans(Maxmin<double> ans){
    cout << "Оптимальные стратегии: ";
    for (int i = 0; i < ans.indexes.size(); i++){
        cout << ans.indexes[i] + 1;
        if (i != ans.indexes.size() - 1) cout << ", ";
    }
    cout << endl << endl;
}

vector<int> Mat::bayes_laplace(bool print){
    vector<double> temp(matrix.size());
    if (probabilities_flag){
        if (print) cout << "Критерий Байеса" << endl;
        for (int i = 0; i < matrix.size(); i++){
            for (int j = 0; j < matrix[0].size(); j++){
                temp[i] += matrix[i][j] * probabilities[j];
            }
            if (print) cout << "a" << i + 1 << " = " << temp[i] << endl;
        }
    }else{
        if (print) cout << "Критерий Лапласа" << endl;
        for (int i = 0; i < matrix.size(); i++){
            for (int j = 0; j < matrix[0].size(); j++){
                temp[i] += matrix[i][j];
            }
            temp[i] /= matrix[0].size();
            if (print) cout << "a" << i + 1 << " = " << temp[i] << endl;
        }
    }
    Maxmin<double> ans;
    if (income){
        ans = max(temp);
    }else{
        ans = min(temp);
    }
    if (print) print_ans(ans);
    return ans.indexes;
}

vector<int> Mat::wald(bool print){
    vector<double> temp(matrix.size());
    Maxmin<double> ans;
    if (print) cout << "Критерий Вальда" << endl;
    if (income){
        for (int i = 0; i < matrix.size(); i++){
            temp[i] = min(matrix[i]).value;
        }
        ans = max(temp);
    }else{
        for (int i = 0; i < matrix.size(); i++){
            temp[i] = max(matrix[i]).value;
        }
        ans = min(temp);
    }
    if (print) print_ans(ans);
    return ans.indexes;
}

vector<int> Mat::savage(bool print){
    vector<vector<double>> risks(matrix.size());
    if (print) cout << "Критерий Сэвиджа" << endl << "Матрица рисков:" << endl;
    if (income){
        vector<double> temp1(matrix[0].size());
        for (int i = 0; i < matrix[0].size(); i++){
            vector<double> temp2(matrix.size());
            for (int j = 0; j < matrix.size(); j++){
                temp2[j] = matrix[j][i];
            }
            temp1[i] = max(temp2).value;
        }
        for (int i = 0; i < matrix.size(); i++){
            risks[i].resize(matrix[i].size());
            for (int j = 0; j < matrix[i].size(); j++){
                double risk = temp1[j] - matrix[i][j];
                risks[i][j] = risk;
                if (print) cout << risk << " ";
            }
            if (print) cout << endl;
        }
    }else{
        vector<double> temp1(matrix[0].size());
        for (int i = 0; i < matrix[0].size(); i++){
            vector<double> temp2(matrix.size());
            for (int j = 0; j < matrix.size(); j++){
                temp2[j] = matrix[j][i];
            }
            temp1[i] = min(temp2).value;
        }
        for (int i = 0; i < matrix.size(); i++){
            risks[i].resize(matrix[i].size());
            for (int j = 0; j < matrix[i].size(); j++){
                double risk = matrix[i][j] - temp1[j];
                risks[i][j] = risk;
                if (print) cout << risk << " ";
            }
            if (print) cout << endl;
        }
    }
    vector<double> temp(risks.size());
    for (int i = 0; i < risks.size(); i++){
        temp[i] = max(risks[i]).value;
    }
    Maxmin<double> ans = min(temp);
    if (print) print_ans(ans);
    return ans.indexes;
}

vector<int> Mat::hurwitz(bool print){
    vector<double> temp_max(matrix.size());
    vector<double> temp_min(matrix.size());
    Maxmin<double> ans;
    if (print) cout << "Критерий Гурвица" << endl;
    for (int i = 0; i < matrix.size(); i++){
        temp_max[i] = max(matrix[i]).value;
        temp_min[i] = min(matrix[i]).value;
    }
    if (income){
        vector<double> temp(matrix.size());
        for (int i = 0; i < matrix.size(); i++){
            temp[i] = optimism * temp_min[i] + (1 - optimism) * temp_max[i];
            if (print) cout << "a" << i + 1 << " = " << temp[i] << endl;
        }
        ans = max(temp);
    }else{
        vector<double> temp(matrix.size());
        for (int i = 0; i < matrix.size(); i++){
            temp[i] = optimism * temp_max[i] + (1 - optimism) * temp_min[i];
            if (print) cout << "a" << i + 1 << " = " << temp[i] << endl;
        }
        ans = min(temp);
    }
    if (print) print_ans(ans);
    return ans.indexes;
}
