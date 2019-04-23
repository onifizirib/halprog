#include <matrix.h>
#include <vector>
#include <iostream>

int main(int, char**) {
    MatrixNxN<int> m;
    std::cout << "initializing empty matrix:\n" <<m;
    
    std::vector<int> v{1,11,111,2,22,222,3,33,333};
    MatrixNxN<int> m1(3, [=](int i){return v[i];});
    std::cout <<"initializing non-empty matrix:\n" <<m1;  
    std::cout <<"-----------------------------------\n";
    
    return 0;
}
