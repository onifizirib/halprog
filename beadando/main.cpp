#include <iostream>
#include "graph.h"

int main(int, char**) {
    std::cout << "Hello, world!\n";
    graph<int> proba;
    int A = 9;
    int B = 8;
    int C = 4;
    proba.addNode(A);
    proba.addNode(B);
    proba.addNode(B);
    proba.addNode(C);
    proba.addLink(A, B);
    std::vector<std::vector<int>> v = proba.getAdjList();
    std::cout<< "adjList hossza: " << v.size() << ", NodeNum: " << proba.getNodeNum() <<"\n";
    std::cout<< "adjList elemeinek hossza: ";
    for(int i=0; i<v.size(); i++)
    {
        std::cout << v[i].size() << ", ";
    }
    std::cout << "linkNum: " << proba.getLinkNum() <<"\n";
    //std::cout<<v[0][0]<<", "<<v[0][1]<<"\n";
    //std::cout<<v[1][0]<<", "<<v[1][1]<<"\n";
    std::cout << proba;

    graph<int> proba2(10);
    std::cout << proba2;
    std::vector<std::vector<int>> wat = proba2.getAdjList();
    std::cout<< "adjList hossza: " << wat.size() <<", nodenum=" <<proba2.getNodeNum() <<"\n";
    std::cout<< "adjList elemeinek hossza: ";
    for(int i=0; i<wat.size(); i++)
    {
        std::cout << wat[i].size() << ", ";
    }
    std::cout << "linkNum: " << proba2.getLinkNum() <<"\n";
}