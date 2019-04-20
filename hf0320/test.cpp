#include <matrix.h>
#include <vector>
#include <iostream>

int main ()
{
    auto err = [](auto str){ std::cout << "MatrixNxN.h error in: " << str << "\n"; std::exit(-1); };
	
	//Test deafault const:
	{
		MatrixNxN<double> m;
		if(m.size() != 0)         { err("default contructor test [size]");           }
		if(m.begin() != m.end())  { err("default contructor test [begin == end]");   }
		if(m.cbegin() != m.cend()){ err("default contructor test [cbegin == cend]"); }
	}
	
	//Test move constructor:
	{
	    std::vector<double> data = {3.1, 5.2, 9.3, 1.9};
		MatrixNxN<double> a (2, [=](int i){return data[i];});
		MatrixNxN<double> b{ std::move(a) };
		if(a.size() != 0)                                                { err("move constructor test [src size]");             }
		if(a.begin() != a.end())                                         { err("move constructor test [src begin == src end]"); }
		if(b.size() != 2)                                                { err("move constructor test [size]");                 }
		if(b[0] != 3.1 || b[1] != 5.2 || b[2] != 9.3 || b[3] != 1.9)     { err("move constructor test [elements]");             }
		if( ++ ++ ++ ++(b.begin()) != b.end() )                          { err("move constructor test [begin + 4 == end]");     }
		if( ++ ++ ++ ++(b.cbegin()) != b.cend() )                        { err("move constructor test [cbegin + 4 == cend]");   }
	}
    
    //Test copy constructor:
	{
		std::vector<double> data = {3.1, 5.2, 9.3, 1.9};
		MatrixNxN<double> a (2, [=](int i){return data[i];});
		MatrixNxN<double> b{a};
		if(b.size() != 2)                                           { err("copy constructor test [size]");               }
		if(b[0] != 3.1 || b[1] != 5.2 || b[2] != 9.3 || b[3] != 1.9){ err("copy constructor test [elements]");           }
		if( ++ ++ ++ ++(b.begin()) != b.end() )                     { err("copy constructor test [begin + 3 == end]");   }
		if( ++ ++ ++ ++(b.cbegin()) != b.cend() )                   { err("copy constructor test [cbegin + 3 == cend]"); }
		if(a.size() != 2)                                           { err("copy constructor test [src size]");           }
		if(a[0] != 3.1 || a[1] != 5.2 || a[2] != 9.3 || a[3] != 1.9){ err("copy constructor test [src elements]");       }
	}
	
	//Test indexible function constructor:
	{
		MatrixNxN<int> a{4, [](int i){ return 100*i*i+20*i+5; }};
		if(a.size() != 4)                                         { err("constructor from indexible test [size]");     }
		if(a[0] != 5 || a[1] != 125 || a[2] != 445 || a[3] != 965){ err("constructor from indexible test [elements]"); }
	}
	
	//Test assignment:
	{
		std::vector<double> data = {3.1, 5.2, 9.3, 1.9};
		MatrixNxN<double> a (2, [=](int i){return data[i];});
		MatrixNxN<double> b;
		b = a;
		if(b.size() != 2)                                           { err("assignment test [size]");         }
		if(b[0] != 3.1 || b[1] != 5.2 || b[2] != 9.3 || b[3] != 1.9){ err("assignment test [elements]");     }
		if(a.size() != 2)                                           { err("assignment test [src size]");     }
		if(a[0] != 3.1 || a[1] != 5.2 || a[2] != 9.3 || a[3] != 1.9){ err("assignment test [src elements]"); }
	}
    
    //Test self assignment:
	{
		std::vector<double> data = {3.1, 5.2, 9.3, 1.9};
		MatrixNxN<double> c (2, [=](int i){return data[i];});
		c = c;
		if(c.size() != 2)                                           { err("self assignment test [size]");     }
		if(c[0] != 3.1 || c[1] != 5.2 || c[2] != 9.3 || c[3] != 1.9){ err("self assignment test [elements]"); }
	}
	
	//Test move assignment:
	{
		std::vector<double> data = {3.1, 5.2, 9.3, 1.9};
		MatrixNxN<double> a (2, [=](int i){return data[i];});
		MatrixNxN<double> b;
		b = std::move(a);
		if(a.begin() != a.end())                                    { err("assignment test [src begin == src end]"); }
		if(a.size() != 0)                                           { err("assignment test [src size]");             }
		if(b.size() != 2)                                           { err("assignment test [size]");                 }
		if(b[0] != 3.1 || b[1] != 5.2 || b[2] != 9.3 || b[3] != 1.9){ err("assignment test [elements]");             }
	}
	
	//Test self move assignment:
	{
		std::vector<double> data = {3.1, 5.2, 9.3, 1.9};
		MatrixNxN<double> c (2, [=](int i){return data[i];});
		c = std::move(c);
		if(c.size() != 2)                                           { err("self assignment test [size]");     }
		if(c[0] != 3.1 || c[1] != 5.2 || c[2] != 9.3 || c[3] != 1.9){ err("self assignment test [elements]"); }
	}
	
	//Test cpy transpose:
	{
		std::vector<double> data = {3.1, 5.2, 9.3, 1.9};
		MatrixNxN<double> a (2, [=](int i){return data[i];});
		MatrixNxN<double> b;
		b = transpose(a);
		if(b.size() != 2)                                           { err("cpy transpose test [size]");     }
		if(b[0] != 3.1 || b[2] != 5.2 || b[1] != 9.3 || b[3] != 1.9){ err("cpy transpose test [elements]"); }
	}
	
	//Test move transpose:
	{
		std::vector<double> data = {3.1, 5.2, 9.3, 1.9};
		MatrixNxN<double> a (2, [=](int i){return data[i];});
		MatrixNxN<double> b;
		b = transpose(std::move(a));
		if(b.size() != 2)                                           { err("move transpose test [size]");     }
		if(b[0] != 3.1 || b[2] != 5.2 || b[1] != 9.3 || b[3] != 1.9){ err("move transpose test [elements]"); }
		if(a.size() != 0)                                           { err("move transpose test [src size]");             }
		if(a.begin() != a.end())                                    { err("move transposer test [src begin == src end]"); }
	}
}


