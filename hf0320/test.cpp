#include <matrix.h>
#include <vector>
#include <iostream>

//összehasonlítófüggvény (majd a tesztekhez), hogy ne kelljen sokszor kiírni
template<typename T>
bool similar(MatrixNxN<T> const& m1, MatrixNxN<T> const& m2)
{
    bool cmp = true;
    if (!sizecheck(m1,m2)) {sizerr();}
    const int N = m1.size();
    for(int i=0; i<N; i++)
    {
        if(std::abs(m1[0]-m2[0]) > 1e-15)
        {
            cmp = false;
            break;
        }
    }
    return cmp;
}

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
		
		//test for 1-sized matrix
		data = {5.7};
		a = MatrixNxN<double>(1,[=](int i){return data[i];});
		b = transpose(std::move(a));
		if(b.size() != 1)                                           { err("move transpose test [size]");     }
		if(b[0] != 5.7)                                             { err("move transpose test [elements]"); }
		if(a.size() != 0)                                           { err("move transpose test [src size]");             }
		if(a.begin() != a.end())                                    { err("move transposer test [src begin == src end]"); }
	}	

	//Test operator- (l-value, l-value)
	{
		std::vector<double> dataA = {3.1, 5.2, 9.3, 1.9};
		std::vector<double> dataB = {7.1, 15.2, 11.3, 0.9};
		std::vector<double> dataRef = {4.0, 10.0, 2.0, -1.0};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = b - a;
		if(a.size() != 2)                                             { err("operator- test (l-value, l-value) [src size]");     }
		if(a[0] != 3.1 || a[1] != 5.2 || a[2] != 9.3 || a[3] != 1.9)  { err("operator- test (l-value, l-value) [src elements]"); }
		if(b.size() != 2)                                             { err("operator- test (l-value, l-value) [src size]");     }
		if(b[0] != 7.1 || b[1] != 15.2 || b[2] != 11.3 || b[3] != 0.9){ err("operator- test (l-value, l-value) [src elements]"); }
		if(res.size() != 2)                                           { err("operator- test (l-value, l-value) [size]");         }
		if(!similar(ref, res))                                        { err("operator- test (l-value, l-value) [elements]"); }
	}
	
	//Test operator- (r-value, l-value)
	{
		std::vector<double> dataA = {3.1, 5.2, 9.3, 1.9};
		std::vector<double> dataB = {7.1, 15.2, 11.3, 0.9};
		std::vector<double> dataRef = {4.0, 10.0, 2.0, -1.0};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = std::move(b) - a;
		if(a.size() != 2)                                             { err("operator- test (r-value, l-value) [src size]");     }
		if(b.size() != 0)                                             { err("operator- test (r-value, l-value) [src size]");     }
		if(res.size() != 2)                                           { err("operator- test (r-value, l-value) [size]");         }
		if(a[0] != 3.1 || a[1] !=  5.2 || a[2] !=  9.3 || a[3] != 1.9){ err("operator- test (r-value, l-value) [src elements]"); }
		if(!similar(ref, res))                                        { err("operator- test (r-value, l-value) [value]"); }
	}
	
	//Test operator- (l-value, r-value)
	{
		std::vector<double> dataA = {3.1, 5.2, 9.3, 1.9};
		std::vector<double> dataB = {7.1, 15.2, 11.3, 0.9};
		std::vector<double> dataRef = {4.0, 10.0, 2.0, -1.0};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = b - std::move(a);
		if(a.size() != 0)                                             { err("operator- test (l-value, r-value) [src size]");     }
		if(b.size() != 2)                                             { err("operator- test (l-value, r-value) [src size]");     }
		if(res.size() != 2)                                           { err("operator- test (l-value, r-value) [size]");         }
		if(b[0] != 7.1 || b[1] != 15.2 || b[2] != 11.3 || b[3] != 0.9){ err("operator- test (l-value, r-value) [src elements]"); }
		if(!similar(ref, res))                                        { err("operator- test (l-value, r-value) [value]"); }
	}
	
	//Test operator- (r-value, r-value)
	{
		std::vector<double> dataA = {3.1, 5.2, 9.3, 1.9};
		std::vector<double> dataB = {7.1, 15.2, 11.3, 0.9};
		std::vector<double> dataRef = {4.0, 10.0, 2.0, -1.0};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = std::move(b) - std::move(a);
		if(a.size() != 2)            { err("operator- test (r-value, r-value) [src size]"); }//this argument was not reused!
		if(b.size() != 0)            { err("operator- test (r-value, r-value) [src size]"); }
		if(res.size() != 2)          { err("operator- test (r-value, r-value) [size]");     }
		if(!similar(ref, res))       { err("operator- test (r-value, r-value) [value]"); }
	}
	
	//Test operator+ (l-value, l-value)
	{
		std::vector<double> dataA = {3.1, 5.2, 9.3, 1.9};
		std::vector<double> dataB = {7.1, 15.2, 11.3, 0.9};
		std::vector<double> dataRef = {10.2, 20.4, 20.6, 2.8};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = a + b;
		if(a.size() != 2)                                             { err("operator+ test (l-value, l-value) [src size]");     }
		if(b.size() != 2)                                             { err("operator+ test (l-value, l-value) [src size]");     }
		if(res.size() != 2)                                           { err("operator+ test (l-value, l-value) [size]");         }
		if(a[0] != 3.1 || a[1] !=  5.2 || a[2] !=  9.3 || a[3] != 1.9){ err("operator+ test (l-value, l-value) [src elements]"); }
		if(b[0] != 7.1 || b[1] != 15.2 || b[2] != 11.3 || b[3] != 0.9){ err("operator+ test (l-value, l-value) [src elements]"); }
		if(!similar(ref, res))                                        { err("operator+ test (l-value, l-value) [value]"); }
	}
	
	//Test operator+ (r-value, l-value)
	{
		std::vector<double> dataA = {3.1, 5.2, 9.3, 1.9};
		std::vector<double> dataB = {7.1, 15.2, 11.3, 0.9};
		std::vector<double> dataRef = {10.2, 20.4, 20.6, 2.8};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = std::move(a) + b;
		if(a.size() != 0)                                             { err("operator+ test (r-value, l-value) [src size]");     }
		if(b.size() != 2)                                             { err("operator+ test (r-value, l-value) [src size]");     }
		if(res.size() != 2)                                           { err("operator+ test (r-value, l-value) [size]");         }
		if(b[0] != 7.1 || b[1] != 15.2 || b[2] != 11.3 || b[3] != 0.9){ err("operator+ test (r-value, l-value) [src elements]"); }
		if(!similar(ref, res))                                        { err("operator+ test (r-value, l-value) [value]"); }
	}
	
	//Test operator+ (l-value, r-value)
	{
	    std::vector<double> dataA = {3.1, 5.2, 9.3, 1.9};
		std::vector<double> dataB = {7.1, 15.2, 11.3, 0.9};
		std::vector<double> dataRef = {10.2, 20.4, 20.6, 2.8};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = a + std::move(b);
		if(a.size() != 2)                                             { err("operator+ test (l-value, r-value) [src size]");     }
		if(b.size() != 0)                                             { err("operator+ test (l-value, r-value) [src size]");     }
		if(res.size() != 2)                                           { err("operator+ test (l-value, r-value) [size]");         }
		if(a[0] != 3.1 || a[1] !=  5.2 || a[2] !=  9.3 || a[3] != 1.9){ err("operator+ test (l-value, r-value) [src elements]"); }
		if(!similar(ref, res))                                        { err("operator+ test (l-value, r-value) [value]"); }
	}
	
	//Test operator+ (r-value, r-value)
	{
	    std::vector<double> dataA = {3.1, 5.2, 9.3, 1.9};
		std::vector<double> dataB = {7.1, 15.2, 11.3, 0.9};
		std::vector<double> dataRef = {10.2, 20.4, 20.6, 2.8};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = std::move(a) + std::move(b);
		if(a.size() != 0)            { err("operator+ test (r-value, r-value) [src size]"); }
		if(b.size() != 2)            { err("operator+ test (r-value, r-value) [src size]"); }//this argument was not reused!
		if(res.size() != 2)          { err("operator+ test (r-value, r-value) [size]");     }
		if(!similar(ref, res))       { err("operator+ test (r-value, r-value) [value]"); }
	}
	
	//Test +=:
	{
		std::vector<double> dataA = {3.1, 5.2, 9.3, 1.9};
		std::vector<double> dataB = {7.1, 15.2, 11.3, 0.9};
		std::vector<double> dataRef = {10.2, 20.4, 20.6, 2.8};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
	    a += b;
		if(a.size() != 2)                                             { err("+= test [size]");         }
		if(b.size() != 2)                                             { err("+= test [src size]");     }
		if(b[0] != 7.1 || b[1] != 15.2 || b[2] != 11.3 || b[3] != 0.9){ err("+= test [src elements]"); }
		if(!similar(ref, a))                                          { err("+= test [value]"); }
		
	}
	
	//Test -=:
	{
		std::vector<double> dataA = {3.1, 5.2, 9.3, 1.9};
		std::vector<double> dataB = {7.1, 15.2, 11.3, 0.9};
		std::vector<double> dataRef = {4.0, 10.0, 2.0, -1.0};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		b -= a;
		if(a.size() != 2)                                           { err("-= test [size]");         }
		if(b.size() != 2)                                           { err("-= test [src size]");     }
		if(a[0] != 3.1 || a[1] != 5.2 || a[2] != 9.3 || a[3] != 1.9){ err("-= test [src elements]"); }
		if(!similar(ref, b))                                        { err("-= test [value]"); }
	}
	
	//Test *=:
	{
		std::vector<double> dataA = {3.1, -5.2, 9.3, 1.9};
		std::vector<double> dataRef = {6.2, -10.4, 18.6, 3.8};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		a *= 2.0;
		if(a.size() != 2)                    { err("*= test [size]");  }
		if(!similar(ref, a))                 { err("*= test [value]"); }
	}
	
	//Test /=:
	{
		std::vector<double> dataA = {3.4, -5.2, 9.6, 1.8};
		std::vector<double> dataRef = {1.7, -2.6, 4.8, 0.9};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		a /= 2.0;
		if(a.size() != 2)                    { err("/= test [size]");  }
		if(!similar(ref, a))                 { err("/= test [value]"); }
	}

    //Test operator* (l-value, scalar)
	{
		std::vector<double> dataA = {3.1, -5.2, 9.3, 1.9};
		std::vector<double> dataRef = {6.2, -10.4, 18.6, 3.8};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = a * 2.0;
		if(a.size()   != 2)                                          { err("operator* test (l-value, scalar) [src size]");     }
		if(res.size() != 2)                                          { err("operator* test (l-value, scalar) [size]");         }
		if(a[0] != 3.1 || a[1] != -5.2 || a[2] != 9.3 || a[3] != 1.9){ err("operator* test (l-value, scalar) [src elements]"); }
		if(!similar(ref, res))                                       { err("operator* test (l-value, scalar) [value]");        }
	}
	
	//Test operator* (r-value, scalar)
	{
		std::vector<double> dataA = {3.1, -5.2, 9.3, 1.9};
		std::vector<double> dataRef = {6.2, -10.4, 18.6, 3.8};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = std::move(a) * 2.0;
		if(a.size()   != 0)                     { err("operator* test (r-value, scalar) [src size]");     }
		if(res.size() != 2)                     { err("operator* test (r-value, scalar) [size]");         }
		if(!similar(ref, res))                  { err("operator* test (r-value, scalar) [value]");        }
	}
	
	//Test operator* (scalar, l-value)
	{
		std::vector<double> dataA = {3.1, -5.2, 9.3, 1.9};
		std::vector<double> dataRef = {6.2, -10.4, 18.6, 3.8};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = 2.0 * a;
		if(a.size()   != 2)                                          { err("operator* test (scalar, l-value) [src size]");     }
		if(res.size() != 2)                                          { err("operator* test (scalar, l-value) [size]");         }
		if(a[0] != 3.1 || a[1] != -5.2 || a[2] != 9.3 || a[3] != 1.9){ err("operator* test (scalar, l-value) [src elements]"); }
		if(!similar(ref, res))                                       { err("operator* test (scalar, l-value) [value]");        }
	}
	
	//Test operator* (scalar, r-value)
	{
		std::vector<double> dataA = {3.1, -5.2, 9.3, 1.9};
		std::vector<double> dataRef = {6.2, -10.4, 18.6, 3.8};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = 2.0 * std::move(a);
		if(a.size()   != 0)                     { err("operator* test (scalar, r-value) [src size]");     }
		if(res.size() != 2)                     { err("operator* test (scalar, r-value) [size]");         }
		if(!similar(ref, res))                  { err("operator* test (scalar, r-value) [value]");        }
	}
	
	//Test operator/ (l-value, scalar)
	{
		std::vector<double> dataA = {3.4, -5.2, 9.6, 1.8};
		std::vector<double> dataRef = {1.7, -2.6, 4.8, 0.9};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = a / 2.0;
		if(a.size()   != 2)                                          { err("operator/ test (l-value, scalar) [src size]");     }
		if(res.size() != 2)                                          { err("operator/ test (l-value, scalar) [size]");         }
		if(a[0] != 3.4 || a[1] != -5.2 || a[2] != 9.6 || a[3] != 1.8){ err("operator/ test (l-value, scalar) [src elements]"); }
		if(!similar(ref, res))                                       { err("operator/ test (l-value, scalar) [value]");        }
	}
	
	//Test operator/ (r-value, scalar)
	{
		std::vector<double> dataA = {3.4, -5.2, 9.6, 1.8};
		std::vector<double> dataRef = {1.7, -2.6, 4.8, 0.9};
		MatrixNxN<double> a (2, [=](int i){return dataA[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = std::move(a) / 2.0;
		if(a.size()   != 0)                     { err("operator/ test (r-value, scalar) [src size]"); }
		if(res.size() != 2)                     { err("operator/ test (r-value, scalar) [size]");     }
		if(!similar(ref, res))                  { err("operator/ test (r-value, scalar) [value]");    }
	}
	
	//Test matrix indexing (sorfolytonos feltöltés)
	{
	    std::vector<double> dataA = {3.4, -5.2, 9.6, 1.8};
	    MatrixNxN<double> a (2, [=](int i){return dataA[i];});
	    if(a[0] != a(0,0) ||
	       a[1] != a(0,1) ||
	       a[2] != a(1,0) ||
	       a[3] != a(1,1)   ) { err("indexing test [value]");    }
	}
	
	//Test matrix multiplication (l-value, l-value)
	{
	    std::vector<double> dataA = {3.4, -5.2, 9.6, 1.8};
	    std::vector<double> dataB = {1.2,  4.2, 1.6, 5.8};
	    //std::vector<double> dataRef = {3.4*1.2 + (-5.2)*1.6, 3.4*4.2 + (-5.2)*5.8, 9.6*1.2 + 1.8*1.6, 9.6*4.2 + 1.8*5.8};
	    std::vector<double> dataRef = {-4.24, -15.88, 14.4, 50.76};
	    MatrixNxN<double> a (2, [=](int i){return dataA[i];});
	    MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = a*b;
		if(a.size() != 2)                       { err("matrix multiplication test (l-value, l-value) [src size]");   }
		if(b.size() != 2)                       { err("matrix multiplication test (l-value, l-value) [src size]");   }
		if(res.size() != 2)                     { err("matrix multiplication test (l-value, l-value) [size]");       }
		if(!similar(ref, res))                  { err("matrix multiplication test (l-value, l-value) [value]");    }
	}
	
	//Test matrix multiplication (r-value, l-value)
	{
	    std::vector<double> dataA = {3.4, -5.2, 9.6, 1.8};
	    std::vector<double> dataB = {1.2,  4.2, 1.6, 5.8};
	    std::vector<double> dataRef = {-4.24, -15.88, 14.4, 50.76};
	    MatrixNxN<double> a (2, [=](int i){return dataA[i];});
	    MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = std::move(a)*b;
		if(a.size() != 0)                       { err("matrix multiplication test (r-value, l-value) [src size]");   }
		if(b.size() != 2)                       { err("matrix multiplication test (r-value, l-value) [src size]");   }
		if(res.size() != 2)                     { err("matrix multiplication test (r-value, l-value) [size]");       }
		if(!similar(ref, res))                  { err("matrix multiplication test (r-value, l-value) [value]");    }
	}
	
	//Test matrix multiplication (l-value, r-value)
	{
	    std::vector<double> dataA = {3.4, -5.2, 9.6, 1.8};
	    std::vector<double> dataB = {1.2,  4.2, 1.6, 5.8};
	    std::vector<double> dataRef = {-4.24, -15.88, 14.4, 50.76};
	    MatrixNxN<double> a (2, [=](int i){return dataA[i];});
	    MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = a*std::move(b);
		if(a.size() != 2)                       { err("matrix multiplication test (l-value, r-value) [src size]");   }
		if(b.size() != 0)                       { err("matrix multiplication test (l-value, r-value) [src size]");   }
		if(res.size() != 2)                     { err("matrix multiplication test (l-value, r-value) [size]");       }
		if(!similar(ref, res))                  { err("matrix multiplication test (l-value, r-value) [value]");    }
	}
	
	//Test matrix multiplication (r-value, r-value)
	{
	    std::vector<double> dataA = {3.4, -5.2, 9.6, 1.8};
	    std::vector<double> dataB = {1.2,  4.2, 1.6, 5.8};
	    std::vector<double> dataRef = {-4.24, -15.88, 14.4, 50.76};
	    MatrixNxN<double> a (2, [=](int i){return dataA[i];});
	    MatrixNxN<double> b (2, [=](int i){return dataB[i];});
		MatrixNxN<double> ref (2, [=](int i){return dataRef[i];});
		MatrixNxN<double> res = std::move(a)*std::move(b);
		if(a.size() != 0)                       { err("matrix multiplication test (r-value, r-value) [src size]");   }
		if(b.size() != 2)                       { err("matrix multiplication test (r-value, r-value) [src size]");   }//this argument was not reused!
		if(res.size() != 2)                     { err("matrix multiplication test (r-value, r-value) [size]");       }
		if(!similar(ref, res))                  { err("matrix multiplication test (r-value, r-value) [value]");    }
	}
	
	//Test sizecheck
	{
	    std::vector<double> dataA = {3.4, -5.2, 9.6, 1.8};
	    std::vector<double> dataB = {1.2,  4.2, 1.6, 5.8, -6.7, 8.8, -2.1, 0.1, 3.3};
	    MatrixNxN<double> a (2, [=](int i){return dataA[i];});
	    MatrixNxN<double> b (3, [=](int i){return dataB[i];});
	    bool ref = false;
	    if (sizecheck(a,b) != ref)              {err("sizecheck function test [value]");}
	    dataB = {1.2,  4.2, 1.6, 5.8};
	    b =  MatrixNxN<double>(2, [=](int i){return dataB[i];});
	    ref = true;
	    if (sizecheck(a,b) != ref)              {err("sizecheck function test [value]");}
	}
	
	std::cout << "All tests are successful!\n";
    return 0;
}
