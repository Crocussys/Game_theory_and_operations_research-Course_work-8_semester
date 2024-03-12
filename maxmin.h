#ifndef MAXMIN_H
#define MAXMIN_H
#include <vector>

template<typename T>
class Maxmin
{
private:
    T value;
    std::vector<int> indexes;

public:
    Maxmin(){};
    Maxmin(T new_value){
        value = new_value;
    }
    ~Maxmin(){}

    void set_value(T new_value){
        value = new_value;
    }

    T get_value(){
        return value;
    }
    std::vector<int>* get_vector(){
        return &indexes;
    }
};

#endif // MAXMIN_H
