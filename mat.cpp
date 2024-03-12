#include <fstream>
#include <iostream>
#include "mat.h"

Mat::Mat() {
    income = true;
    std::vector<std::vector<double>> matrix;
    probabilities_flag = false;
    std::vector<double> probabilities;
    optimism = 0.5;
}

Mat::Mat(char file_path[]){
    int inp;
    unsigned int size_n, size_m;
    std::fstream file(file_path, std::ios::in);
    if (!file){
        throw "FileError";
    }

    file >> inp;
    if (inp == 0){
        income = false;
    }else{
        income = true;
    }
    file >> size_n;
    file >> size_m;
    this->matrix.resize(size_n);
    for (int i = 0; i < size_n; i++){
        this->matrix[i].resize(size_m);
        for (int j = 0; j < size_m; j++){
            file >> this->matrix[i][j];
        }
    }
    file >> inp;
    if (inp == 1){
        probabilities_flag = true;
    }else{
        probabilities_flag = false;
    }
    if (probabilities_flag){
        this->probabilities.resize(size_m);
        for (int i = 0; i < size_m; i++){
            file >> this->probabilities[i];
        }
    }
    if (!(file >> optimism)){
        optimism = 0.5;
    }
    file.close();
}

Mat::~Mat(){}

template<typename T>
Maxmin<T> Mat::max(std::vector<T> arr){
    T max_num = arr[0];
    for (int i = 1; i < arr.size(); i++){
        if (arr[i] > max_num){
            max_num = arr[i];
        }
    }
    Maxmin<T> ans(max_num);
    for (int i = 0; i < arr.size(); i++){
        if (arr[i] >= max_num){
            ans.get_vector()->push_back(i);
        }
    }
    return ans;
}

template<typename T>
Maxmin<T> Mat::min(std::vector<T> arr){
    T min_num = arr[0];
    for (int i = 1; i < arr.size(); i++){
        if (arr[i] < min_num){
            min_num = arr[i];
        }
    }
    Maxmin<T> ans(min_num);
    for (int i = 0; i < arr.size(); i++){
        if (arr[i] <= min_num){
            ans.get_vector()->push_back(i);
        }
    }
    return ans;
}

std::vector<int> Mat::solution(){
    std::vector<int> strategies(matrix.size());
    std::vector<std::vector<int>> criterions(4);
    criterions[0] = bayes_laplace();
    criterions[1] = wald();
    criterions[2] = savage();
    criterions[3] = hurwitz();
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < criterions[i].size(); j++){
            strategies[criterions[i][j]]++;
        }
    }
    Maxmin<int> ans = max(strategies);
    return *ans.get_vector();
}

std::vector<int> Mat::bayes_laplace(){
    std::vector<double> temp(matrix.size());
    if (probabilities_flag){
        for (int i = 0; i < matrix.size(); i++){
            for (int j = 0; j < matrix[0].size(); j++){
                temp[i] += matrix[i][j] * probabilities[j];
            }
        }
    }else{
        for (int i = 0; i < matrix.size(); i++){
            for (int j = 0; j < matrix[0].size(); j++){
                temp[i] += matrix[i][j];
            }
            temp[i] /= matrix[0].size();
        }
    }
    Maxmin<double> ans;
    if (income){
        ans = max(temp);
    }else{
        ans = min(temp);
    }
    return *ans.get_vector();
}

std::vector<int> Mat::wald(){
    std::vector<double> temp(matrix.size());
    if (income){
        for (int i = 0; i < matrix.size(); i++){
            temp[i] = min(matrix[i]).get_value();
        }
        return *max(temp).get_vector();
    }else{
        for (int i = 0; i < matrix.size(); i++){
            temp[i] = max(matrix[i]).get_value();
        }
        return *min(temp).get_vector();
    }
}

std::vector<int> Mat::savage(){
    std::vector<std::vector<double>> risks(matrix.size());
    if (income){
        std::vector<double> temp1(matrix[0].size());
        for (int i = 0; i < matrix[0].size(); i++){
            std::vector<double> temp2(matrix.size());
            for (int j = 0; j < matrix.size(); j++){
                temp2[j] = matrix[j][i];
            }
            temp1[i] = max(temp2).get_value();
        }
        for (int i = 0; i < matrix.size(); i++){
            risks[i].resize(matrix[i].size());
            for (int j = 0; j < matrix[i].size(); j++){
                risks[i][j] = temp1[j] - matrix[i][j];
            }
        }
    }else{
        std::vector<double> temp1(matrix[0].size());
        for (int i = 0; i < matrix[0].size(); i++){
            std::vector<double> temp2(matrix.size());
            for (int j = 0; j < matrix.size(); j++){
                temp2[j] = matrix[j][i];
            }
            temp1[i] = min(temp2).get_value();
        }
        for (int i = 0; i < matrix.size(); i++){
            risks[i].resize(matrix[i].size());
            for (int j = 0; j < matrix[i].size(); j++){
                risks[i][j] = matrix[i][j] - temp1[j];
            }
        }
    }
    std::vector<double> temp(risks.size());
    for (int i = 0; i < risks.size(); i++){
        temp[i] = max(risks[i]).get_value();
    }
    return *min(temp).get_vector();
}

std::vector<int> Mat::hurwitz(){
    std::vector<double> temp_max(matrix.size());
    std::vector<double> temp_min(matrix.size());
    for (int i = 0; i < matrix.size(); i++){
        temp_max[i] = max(matrix[i]).get_value();
        temp_min[i] = min(matrix[i]).get_value();
    }
    if (income){
        std::vector<double> temp(matrix.size());
        for (int i = 0; i < matrix.size(); i++){
            temp[i] = optimism * temp_min[i] + (1 - optimism) * temp_max[i];
        }
        return *max(temp).get_vector();
    }else{
        std::vector<double> temp(matrix.size());
        for (int i = 0; i < matrix.size(); i++){
            temp[i] = optimism * temp_max[i] + (1 - optimism) * temp_min[i];
        }
        return *min(temp).get_vector();
    }
}
