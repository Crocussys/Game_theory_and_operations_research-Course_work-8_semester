#include <iostream>
#include <vector>
#include "mat.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2){
        cout << "Недостаточно аргуметов!" << endl;
        return -1;
    }

    try{
        Mat test(argv[1]);
        std::vector<int> result = test.solution(true);
        cout << "Рекомендуемые стратегии на основе всех критериев: ";
        for (int i = 0; i < result.size(); i++){
            if (i != 0){
                cout << ", ";
            }
            cout << "№" << result[i] + 1;
        }
        cout << endl;
    }catch (const char* msg){
        cout << "Error: " << msg << endl;
        return -2;
    }

    return 0;
}
