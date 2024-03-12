#ifndef MAT_H
#define MAT_H
#include <vector>
#include "maxmin.h"

class Mat
{
private:
    bool income;
    std::vector<std::vector<double>> matrix;
    bool probabilities_flag;
    std::vector<double> probabilities;
    float optimism;

    template<typename T>
    Maxmin<T> max(std::vector<T> arr);
    template<typename T>
    Maxmin<T> min(std::vector<T> arr);

public:
    Mat();
    Mat(char file_path[]);
    ~Mat();

    std::vector<int> solution();
    std::vector<int> bayes_laplace();
    std::vector<int> wald();
    std::vector<int> savage();
    std::vector<int> hurwitz();
};

#endif // MAT_H
