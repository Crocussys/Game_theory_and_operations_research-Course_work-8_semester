#ifndef MAT_H
#define MAT_H
#include <vector>

using namespace std;

template<typename T>
struct Maxmin{
    T value;
    vector<int> indexes;
};

class Mat
{
private:
    bool income;
    vector<vector<double>> matrix;
    bool probabilities_flag;
    vector<double> probabilities;
    double optimism;

    template<typename T>
    Maxmin<T> max(vector<T> arr);
    template<typename T>
    Maxmin<T> min(vector<T> arr);

public:
    Mat(char file_path[]);

    vector<int> solution(bool print = false);
    vector<int> bayes_laplace(bool print = false);
    vector<int> wald(bool print = false);
    vector<int> savage(bool print = false);
    vector<int> hurwitz(bool print = false);
};

#endif // MAT_H
