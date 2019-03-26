// legtöbb hibakezelés hiányos

#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>
#include <ostream>

//gyakori lambda-fv-ek kiemelése (valamint pici függvények, tehát előhívni több idő lenne mint végrehajtani -> inline)
inline auto add = [](auto const& x, auto const& y){return x+y;};
inline auto sub = [](auto const& x, auto const& y){return x-y;};

template<typename T>
struct MatrixNxN
{
    std::vector<T> data;
    int sizeN;

    //indexelés
    T&          operator()(int x, int y)        {return data[(x-1)*sizeN + y-1];}
    T const&    operator()(int x, int y) const  {return data[(x-1)*sizeN + y-1];}

    //konstruktorok
    MatrixNxN()
    {
        std::vector<T> v;
        data = v;
        // ez így elég gyanús, hogy lehet szebben is. hogy lehet valami ilyesmit csinálni: data = vector<T>; ?
        //vagy csak simán vector<T> data; ? tudni fogja, hogy az a data az a matrixNxN data-jára vonatkozik?
        sizeN = 0;
    }
    MatrixNxN(MatrixNxN<T> const& cpy)
    {
        data = cpy.data;
        sizeN = cpy.sizeN;
    }
    MatrixNxN(MatrixNxN<T> && mv)
    {
        std::swap(data, mv.data);
        std::swap(sizeN, mv.sizeN);
    }
    //konstruktor megadott elemekből
    template<typename F>
    MatrixNxN(F f, int n)
    {
        sizeN = n;
        n *= n;
        if (f.size() != n)
        {
            std::cout << "number of given elements do not correnspont with the given size N" << std::endl;
        }
        data.resize(n);        
        for(int i=0; i<n; i++) {data[i] = f[i];}
        // f(i) nem működne std::vector-ra, ezért írtam f[i]-t.
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
        return *this;
    }
    //destruktort kell írnom? az világos, hogy minden kostruktort meg kellett ha akár egyet is írtam. ha igen, hogyan?

    //műveletekhez lehessen használni vektorszerűként (csak olvasó esetekre cbegin cend)
    //ez így oké? nem kontárkodok így bele abba, ahogy eltárolja magát egy ilyen matrix struct egy példánya? pl nem tűnik-e/mutat-e még valami el a sizeN tag(ra)?
    auto begin()    {return data.begin();}
    auto cbegin()   {return data.cbegin();}
    auto end()      {return data.end();}
    auto cend()     {return data.cend();}

    //+= operátor
    MatrixNxN<T>& operator+= (MatrixNxN<T> const& cpy)
    {
        std::transform(*this.begin(),*this.end(),cpy.cbegin(),*this.begin(), add);
        return *this;
    }

    //-= oerátor
    MatrixNxN<T>& operator-= (MatrixNxN<T> const& cpy)
    {
        std::transform(*this.begin(),*this.end(),cpy.cbegin(),*this.begin(), sub);
        return *this;
    }

    //*= operátor (csak skalárral szorzásra)
    MatrixNxN<T>& operator*= (T const& scal)
    {
        std::transform(*this.begin(),*this.end(),*this.begin(), [=](T const& x){return x*scal;});
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
        std::transform(*this.begin(),*this.end(),*this.begin(), [=](T const& x){return x/scal;});
        return *this;
    }

    //NxN mátrix mérete (N)
    int size()
    {
        return sizeN;
    }
};
//-------------------------------------------------------------------------

//méret-egyezést ellenőrző függvény:
//lehetne ez bool visszatérési értékű, és ott, ahol meg van hívva, az eredménytől függően továbbhaladni. egyelőre csak kiírás történik. 
template<typename T1, typename T2>
void sizecheck (MatrixNxN<T1> const& m1, MatrixNxN<T2> const& m2)
{
    if (m1.size() != m2.size())
    {
        std::cout << "the two matrices do not have the same size \n";
    }
    return;
}

//összeadás mátrixokra (4 verzió)
template<typename T>
MatrixNxN<T> operator+ (MatrixNxN<T> const& m1, MatrixNxN<T> const& m2)
{
    sizecheck(m1, m2);
    MatrixNxN<T> result;
    result.data.resize(m1.data.size());
    result.sizeN = m1.sizeN;
    std::transform(m1.cbegin(),m1.cend(),m2.cbegin(),result.begin(),add);
    return result;
}

template<typename T>
MatrixNxN<T> operator+ (MatrixNxN<T>&& m1, MatrixNxN<T> const& m2)
{
    sizecheck(m1, m2);
    std::transform(m1.begin(),m1.end(),m2.cbegin(),m1.begin(),add);
    return std::move(m1);
}

template<typename T>
MatrixNxN<T> operator+ (MatrixNxN<T> const& m1, MatrixNxN<T>&& m2)
{
    sizecheck(m1, m2);
    std::transform(m1.cbegin(),m1.cend(),m2.begin(),m2.begin(),add);
    return std::move(m2);
}

template<typename T>
MatrixNxN<T> operator+ (MatrixNxN<T>&& m1, MatrixNxN<T>&& m2)
{
    sizecheck(m1, m2);
    std::transform(m1.begin(),m1.end(),m2.cbegin(),m1.begin(),add);
    return std::move(m1);
}

//kivonás mátrixokra (4 verzió)
template<typename T>
MatrixNxN<T> operator- (MatrixNxN<T> const& m1, MatrixNxN<T> const& m2)
{
    sizecheck(m1, m2);
    MatrixNxN<T> result;
    result.data.resize(m1.data.size());
    result.sizeN = m1.sizeN;
    std::transform(m1.cbegin(),m1.cend(),m2.cbegin(),result.begin(),sub);
    return result;
}

template<typename T>
MatrixNxN<T> operator- (MatrixNxN<T>&& m1, MatrixNxN<T> const& m2)
{
    sizecheck(m1, m2);
    std::transform(m1.begin(),m1.end(),m2.cbegin(),m1.begin(),sub);
    return std::move(m1);
}

template<typename T>
MatrixNxN<T> operator- (MatrixNxN<T> const& m1, MatrixNxN<T>&& m2)
{
    sizecheck(m1, m2);
    std::transform(m1.cbegin(),m1.cend(),m2.begin(),m2.begin(),sub);
    return std::move(m2);
}

template<typename T>
MatrixNxN<T> operator- (MatrixNxN<T>&& m1, MatrixNxN<T>&& m2)
{
    sizecheck(m1, m2);
    std::transform(m1.begin(),m1.end(),m2.cbegin(),m1.begin(),sub);
    return std::move(m1);
}

//szorzás skalárral, mindkét oldalról (2x2 verzió)
template<typename T>
MatrixNxN<T> operator* (MatrixNxN<T> const& m, T const& scal)
{
    MatrixNxN<T> result;
    result.data.resize(m.data.size());
    result.sizeN = m.sizeN;
    std::transform(m.cbegin(),m.cend(),result.begin(),[=](T const& x){return x*scal;});
    return result;
}

template<typename T>
MatrixNxN<T> operator* (MatrixNxN<T>&& m, T const& scal)
{
    std::transform(m.begin(),m.end(),m.begin(),[=](T const& x){return x*scal;});
    return std::move(m);
}

template<typename T>
MatrixNxN<T> operator* (T const& scal, MatrixNxN<T> const& m)
{
    MatrixNxN<T> result;
    result.data.resize(m.data.size());
    result.sizeN = m.sizeN;
    std::transform(m.cbegin(),m.cend(),result.begin(),[=](T const& x){return scal*x;});
    return result;
}

template<typename T>
MatrixNxN<T> operator* (T const& scal, MatrixNxN<T>&& m)
{
    std::transform(m.begin(),m.end(),m.begin(),[=](T const& x){return scal*x;});
    return std::move(m);
}

//osztás skalárral, csak jobbról van értelme (2 verzió)
template<typename T>
MatrixNxN<T> operator/ (MatrixNxN<T> const& m, T const& scal)
{
    if ((T)0 == scal) {std::cout << "dividing with zero \n";}
    MatrixNxN<T> result;
    result.data.resize(m.data.size());
    result.sizeN = m.sizeN;
    std::transform(m.cbegin(),m.cend(),result.begin(),[=](T const& x){return x/scal;});
    return result;
}

template<typename T>
MatrixNxN<T> operator/ (MatrixNxN<T>&& m, T const& scal)
{
    if ((T)0 == scal) {std::cout << "dividing with zero \n";}
    std::transform(m.begin(),m.end(),m.begin(),[=](T const& x){return x/scal;});
    return std::move(m);
}

//mátrixok szorzása (4? verzió)
/*
    mivel az egyik mátrix esetén nem sorfolytonos az elemek kiolvasása, ezért ciklusokkal fogom implementálni a szorzást
    UPDATE: ha módosítok elemet abban a mátrixban, amelyik részt vesz a szorzásban, az elront néhány másik szorzást is!
    -> jobbérték referenciával nem éri meg a jelenelgi megoldás. leehtne pl létrehozni ideiglenes sort / oszlopot,
    attól függően, hogy a bemeneti mátrixok közül melyik jobbérték. kérdés, megéri-e, elvégre egy halom pici kis másolás történne
    ide-oda az ideiglenes egysoros tárolóból. sőt, kell akkor egyaltalán a sok egysor másolás után a move visszatéréskor??
*/
template<typename T>
MatrixNxN<T> operator* (MatrixNxN<T> const& m1, MatrixNxN<T> const& m2)
{
    sizecheck(m1, m2);
    int N = m1.size(N);
    MatrixNxN<T> result;
    result.data.resize(m1.data.size());
    result.sizeN = N;
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            result(i,j) = (T)0;
            // bár ha jól gondolom ez eleve erre ((T)0) lett inicializálva
            for(int n=1; n<=N; n++)
            {
                result(i,j) += m1(i,n)*m2(n,j);
            }
        }
    }
    return result;
}

//transzponálás (2 verzió)
template<typename T>
MatrixNxN<T> transpone(MatrixNxN<T> const& m)
{
    MatrixNxN<T> result;
    int N = m.sizeN;
    result.data.resize(m.data.size());
    result.sizeN = N;
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if (i==j)
            {
                result(i,j) = m(i,j);
            }
            else
            {
                result(i,j) = m(j,i);
            }            
        }
    }
    // próbálkoztam for_each()-csel is, de onnét nem férek hozzá az indexekhez -> nem tudom, melyik m.data elemet kéne másoljam
    return result;
}

template<typename T>
MatrixNxN<T> transpone(MatrixNxN<T>&& m)
{
    //felső háromszögre meghívva -> ha csak 1x1-es a mátrix, nem fog működni
    int N = m.sizeN;
    for(int i=2; i<N; i++)
    {
        for(int j=1; j<i; j++)
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
    int n = m.data.size();
    int N = m.sizeN;
    o << "size: " << N << "x" << N << std::endl;    
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