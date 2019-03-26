#include <matrix.h>
#include <vector>
#include <iostream>

int main(int, char**) {
    std::cout << "Hello, world!\n";
    std::cout<< "----------------------------------------------------\n";
    MatrixNxN<int> m;
    std::cout << "initializing empty matrix: " <<m;
    
    std::vector<int> v{1,11,111,2,22,222,3,33,333};
    MatrixNxN<int> m1(v,3);
    std::cout <<"initializing matrix with a vector as its data: " <<m1;
    
    /*
    MatrixNxN<int> m2(v,3);
    m2 += m1;
    //úgy néz ki, a matrix.begin() nem megfelelő. a pointereknek is különböző típusa van? mi a feloldás akkor, 
    //ha nem tudom "kiköpetni" egy függvénnyel a matrix.data.begin()-t?
    std::cout << m1+m2;
    */
    
    return 0;
}
