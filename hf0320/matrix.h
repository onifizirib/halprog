// van kezdetleges hibadobálás a méretkülönbségekre

#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>
#include <ostream>

//gyakori lambda-fv-ek kiemelése (valamint pici függvények, tehát előhívni több idő lenne mint végrehajtani -> inline)
inline auto add = [](auto const& x, auto const& y){return x+y;};
inline auto sub = [](auto const& x, auto const& y){return x-y;};

//segéd struct-ok, hogy elkülöníthető legyenek az ezek nélkül azonos szintaxisú konstruktorok
struct lambda1{};
struct lambda2{};
struct specimen1{};
/*
a specimen itt arra utal, hogy pl egy vektor egy példányát lehet átadni -> matrix data-ját azon vektor elemei fogják alkotni
le kell ellenőrizni, hogy a vektor példány mellé megadott sizeN négyzete megegyezik-e a vektor hosszával!
*/

//hibahajigáláshoz
auto err = [](auto str){ std::cout << "MatrixNxN.h error: " << str << "\n"; std::exit(-1); };
auto sizerr() 
{
    err("matrices do not have the same size!");
    /*
    Q: kell-e itt visszatérni bármivel is? eredetileg void függvénnyel próbálkoztam és az err(..) után állt egy return;
    de kaptam egy warningot, miszerint: control reaches end of non-void function [-Wreturn-type]
    így átírtam auto visszatérési értékűre. de ugyanazt a warningot kapom.
    */
}

//------------------------------------------------------------------------------------------------------------------------
template<typename T>
struct MatrixNxN
{
    std::vector<T> data;
    int sizeN;

    //indexelés
    T&          operator()(int x, int y)        {return data[(x)*sizeN + y];}
    T const&    operator()(int x, int y) const  {return data[(x)*sizeN + y];}
    T&          operator[](int i)               {return data[i];}
    T const&    operator[](int i) const         {return data[i];}

    //konstruktorok
    MatrixNxN()
    {
        /* 
        vector<T> data -> neki van default const-ja -> nem kell itt írni
        */
        sizeN = 0;
    }
    MatrixNxN(MatrixNxN<T> const&) = default;
    MatrixNxN(MatrixNxN<T> && mv)
	{
		std::swap(data, mv.data);
		sizeN = 0;
		std::swap(sizeN, mv.sizeN);		
	}
    
    template<typename F>
    MatrixNxN(lambda1, int n, F f)
    {
        sizeN = n;
        n *= n;
        data.resize(n);
        for(int i=0; i<n; i++) {data[i] = f(i);}
    }
    
    template<typename F>
    MatrixNxN(lambda2, int n, F f)
    {
        sizeN = n;
        data.resize(n*n); 
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                data[i*n+j] = f(i,j);
            }
        }
    }
    
    template<typename F>
    MatrixNxN(specimen1, int n, F f)
    {
        sizeN = n;
        n *= n;
        data.resize(n);
        for(int i=0; i<n; i++) {data[i] = f[i];}
    }    
    
    //konstruktor függvény szerint megadott elemekből
    template<typename F>
    MatrixNxN(int n, F f)
    {
        sizeN = n;
        n *= n;

        data.resize(n);        
        for(int i=0; i<n; i++) {data[i] = f(i);}
        /*
        Q: ha a felhasználó iyen formában ad meg az F helyén egy lambdával vagy egy vektorral, azt hogyan kéne megkülönböztetni?
        feljebb megírtam az egy és két indexes lambdákhoz a konstruktort, meg a vektorra jó jonstruktort, de hogyan döntöm el itt,
        hogy melyiket kell meghívni?
        */
    }

    //copy és move operátorok
    MatrixNxN<T>& operator=(MatrixNxN<T> const& cpy)
    {
        if(&cpy == this) {return *this;}
        data = cpy.data;
        sizeN = cpy.sizeN;
        return *this;
    }
    MatrixNxN<T>& operator=(MatrixNxN<T> && mv)
    {
        if(&mv == this) {return *this;}
        data = std::move(mv.data);
        sizeN = std::move(mv.sizeN);
		/*
		skalár nem move-olható, másolódni fog -> kinullázni érdemes
		*/
		mv.sizeN = (T)0;
        return *this;
    }

    
    auto begin()   		{return data.begin();}
    auto cbegin() const {return data.cbegin();}
    auto end()      	{return data.end();}
    auto cend()   const	{return data.cend();}
    
    //NxN mátrix mérete (N)
    int size() const {return sizeN;} 
    /*
    valamiért más szóköz/tabulátor mennyiséggel nem akart lefordulni.. úgyhogy egy sorban van.
    */

    //+= operátor
    MatrixNxN<T>& operator+= (MatrixNxN<T> const& cpy)
    {
        if (this->size() != cpy.size()) {sizerr();}
        std::transform(this->begin(), this->end(), cpy.cbegin(), this->begin(), add);
        return *this;
        /*
        this: pointer to the current object
        *this: dereferenced
        Tehát a következő is működik (zárójelek a precedencia miatt kellenek):
           std::transform((*this).begin(), (*this).end(), cpy.cbegin(), (*this).begin(), add);
        */
    }

    //-= oerátor
    MatrixNxN<T>& operator-= (MatrixNxN<T> const& cpy)
    {
        if (this->size() != cpy.size()) {sizerr();}
        std::transform(this->begin(), this->end(), cpy.cbegin(), this->begin(), sub);
        return *this;
    }

    //*= operátor (csak skalárral szorzásra)
    MatrixNxN<T>& operator*= (T const& scal)
    {
        std::transform(this->begin(), this->end(), this->begin(), [=](T const& x){return x*scal;});
        return *this;
    }

    // /= operátor (csak skalárral osztásra)
    MatrixNxN<T> operator/= (T const& scal)
    {
        if ((T)0 == scal)
        {
            std::cout << "dividing with zero" << std::endl;
            return *this;
        }
        std::transform(this->begin(), this->end(), this->begin(), [=](T const& x){return x/scal;});
        return *this;
    }
};
//-------------------------------------------------------------------------

//méret-egyezést ellenőrző függvény:
template<typename T1, typename T2>
bool sizecheck (MatrixNxN<T1> const& m1, MatrixNxN<T2> const& m2)
{
    if (m1.size() != m2.size())
    {
        return false;
    }
    else
    {
        return true;
    }
}

//összeadás mátrixokra (4 verzió)
template<typename T>
MatrixNxN<T> operator+ (MatrixNxN<T> const& m1, MatrixNxN<T> const& m2)
{
    if (!sizecheck(m1, m2)) {sizerr();}
    return MatrixNxN<T>(lambda1{}, m1.size(), [&](int i){return m1[i]+m2[i];});
}

template<typename T>
MatrixNxN<T>&& operator+ (MatrixNxN<T>&& m1, MatrixNxN<T> const& m2)
{
    if (!sizecheck(m1,m2)) {sizerr();}
    std::transform(m1.begin(),m1.end(),m2.cbegin(),m1.begin(),add);
    return std::move(m1);
}

template<typename T>
MatrixNxN<T>&& operator+ (MatrixNxN<T> const& m1, MatrixNxN<T>&& m2)
{
    if (!sizecheck(m1,m2)) {sizerr();}
    std::transform(m1.cbegin(),m1.cend(),m2.begin(),m2.begin(),add);
    return std::move(m2);
}

template<typename T>
MatrixNxN<T>&& operator+ (MatrixNxN<T>&& m1, MatrixNxN<T>&& m2)
{
    if (!sizecheck(m1,m2)) {sizerr();}
    std::transform(m1.begin(),m1.end(),m2.cbegin(),m1.begin(),add);
    return std::move(m1);
}

//kivonás mátrixokra (4 verzió)
template<typename T>
MatrixNxN<T> operator- (MatrixNxN<T> const& m1, MatrixNxN<T> const& m2)
{
    if (!sizecheck(m1,m2)) {sizerr();}
    return MatrixNxN<T>(lambda1{}, m1.size(), [&](int i){return m1[i]-m2[i];});
}

template<typename T>
MatrixNxN<T>&& operator- (MatrixNxN<T>&& m1, MatrixNxN<T> const& m2)
{
    if (!sizecheck(m1,m2)) {sizerr();}
    std::transform(m1.begin(),m1.end(),m2.cbegin(),m1.begin(),sub);
    return std::move(m1);
}

template<typename T>
MatrixNxN<T>&& operator- (MatrixNxN<T> const& m1, MatrixNxN<T>&& m2)
{
    if (!sizecheck(m1,m2)) {sizerr();}
    std::transform(m1.cbegin(),m1.cend(),m2.begin(),m2.begin(),sub);
    return std::move(m2);
}

template<typename T>
MatrixNxN<T>&& operator- (MatrixNxN<T>&& m1, MatrixNxN<T>&& m2)
{
    if (!sizecheck(m1,m2)) {sizerr();}
    std::transform(m1.begin(),m1.end(),m2.cbegin(),m1.begin(),sub);
    return std::move(m1);
}

//szorzás skalárral, mindkét oldalról (2x2 verzió)
template<typename T>
MatrixNxN<T> operator* (MatrixNxN<T> const& m, T const& scal)
{
    return MatrixNxN<T>(lambda1{}, m.size(), [&](int i){return m[i]*scal;});
}

template<typename T>
MatrixNxN<T>&& operator* (MatrixNxN<T>&& m, T const& scal)
{
    std::transform(m.begin(),m.end(),m.begin(),[=](T const& x){return x*scal;});
    return std::move(m);
}

template<typename T>
MatrixNxN<T> operator* (T const& scal, MatrixNxN<T> const& m)
{
    return MatrixNxN<T>(lambda1{}, m.size(), [&](int i){return scal*m[i];});
}

template<typename T>
MatrixNxN<T>&& operator* (T const& scal, MatrixNxN<T>&& m)
{
    std::transform(m.begin(),m.end(),m.begin(),[=](T const& x){return scal*x;});
    return std::move(m);
}

//osztás skalárral, csak jobbról van értelme (2 verzió)
template<typename T>
MatrixNxN<T> operator/ (MatrixNxN<T> const& m, T const& scal)
{
    if ((T)0 == scal) {std::cout << "dividing with zero \n";}
    return MatrixNxN<T>(lambda1{}, m.size(), [&](int i){return m[i]/scal;});
}

template<typename T>
MatrixNxN<T>&& operator/ (MatrixNxN<T>&& m, T const& scal)
{
    if ((T)0 == scal) {std::cout << "dividing with zero \n";}
    std::transform(m.begin(),m.end(),m.begin(),[=](T const& x){return x/scal;});
    return std::move(m);
}

//mátrixok szorzása (4 verzió)
/*
    mivel az egyik mátrix esetén nem sorfolytonos az elemek kiolvasása, ezért ciklusokkal fogom implementálni a szorzást
    jobbértékes verzióhoz létrehozni ideiglenes sort / oszlopot, attól függően, hogy a bemeneti mátrixok közül melyik jobbérték.
	kérdés, megéri-e, elvégre egy halom pici kis másolás történne ide-oda az ideiglenes egysoros tárolóból.
*/
template<typename T>
//Q: érdemes-e külön template típust bevezetni a mátrixoknak? pl ha int beépített módon helyesen szorzódik egy double-lel akkor lehet értelme
MatrixNxN<T> operator* (MatrixNxN<T> const& m1, MatrixNxN<T> const& m2)
{
    if (!sizecheck(m1,m2)) {sizerr();}
    const int N = m1.size();
    return MatrixNxN<T>(lambda2{}, N, [&](int i, int j)
                                    {
                                        T tempRes = (T)0;
                                        for(int n=0; n<N; n++)
                                        {
                                            tempRes += m1(i,n)*m2(n,j);
                                        }
                                        return tempRes;
                                    });
}

template<typename T>
MatrixNxN<T>&& operator* (MatrixNxN<T>&& mv, MatrixNxN<T> const& m)
{
    if (!sizecheck(m,mv)) {sizerr();}
    const int N = m.size();
    std::vector<T> tmp;
    //ideiglenes sor tmp
    for(int i=0; i<N; i++)
    {
        tmp = std::vector<double>(N,(T)0);
        for(int j=0; j<N; j++)
        {
            T tempRes = (T)0;
            for(int n=0; n<N; n++)
            {
                tempRes += mv(i,n)*m(n,j);
            }
            tmp[j] = tempRes;
        }
        //sor beszúrása mv i.sora helyére
        for(int k=0; k<N; k++)
        {
            mv(i,k) = tmp[k];
        }
    }
    return std::move(mv);
}

template<typename T>
MatrixNxN<T>&& operator* (MatrixNxN<T> const& m, MatrixNxN<T>&& mv)
{
    if (!sizecheck(m,mv)) {sizerr();}
    const int N = m.size();
    std::vector<T> tmp;
    //ideiglenes oszlop tmp, valamint eddigiektől eltérő iteráció, mert a jobboldali mátrix memóriája használható fel
    for(int i=0; i<N; i++)
    {
        tmp = std::vector<double>(N,(T)0);
        for(int j=0; j<N; j++)
        {
            T tempRes = (T)0;
            for(int n=0; n<N; n++)
            {
                tempRes += m(j,n)*mv(n,i);
            }
            tmp[j] = tempRes;
        }
        //sor beszúrása mv i.oszlopa helyére
        for(int k=0; k<N; k++)
        {
            mv(k,i) = tmp[k];
        }
    }
    return std::move(mv);
}

template<typename T>
MatrixNxN<T>&& operator* (MatrixNxN<T>&& mv, MatrixNxN<T>&& m)
{
    return std::move(std::move(mv)*m);
}

//transzponálás (2 verzió)
template<typename T>
MatrixNxN<T> transpose(MatrixNxN<T> const& m)
{    
    const int N = m.sizeN;
    return MatrixNxN<T>(lambda2{}, N, [&](int i, int j){return m(j,i);});
}

template<typename T>
MatrixNxN<T>&& transpose(MatrixNxN<T>&& m)
{
    //felső háromszögre meghívva -> ha csak 1x1-es a mátrix, nem fog működni -> ezt külön lekezelem
    const int N = m.sizeN;
    
    if(N == 1){return std::move(m);}
    
    for(int i=1; i<N; i++)
    {
        for(int j=0; j<i; j++)
        {
            std::swap(m(i,j),m(j,i));
        }
    }
    return std::move(m);
}

//kiírás
template<typename T>
std::ostream& operator<< (std::ostream& o, MatrixNxN<T> const& m)
{
    const int n = m.data.size();
    const int N = m.sizeN;
    o <<  N << "x" << N << std::endl;    
    if (n>0)
    {
        for (int i=1; i<n; i++)
        {
            o << m.data[i-1];
            if (i%N == 0)
            {
                o << "; " ;
            }
            else
            {
                o << ", " ;
            }            
        }
        o << m.data[n-1] << std::endl;
    }
    return o;
}
