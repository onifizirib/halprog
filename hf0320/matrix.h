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
    T&          operator()(int x, int y)        {return data[(x)*sizeN + y];}
    T const&    operator()(int x, int y) const  {return data[(x)*sizeN + y];}
    T&          operator[](int i)               {return data[i];}
    T const&    operator[](int i) const         {return data[i];}

    //konstruktorok
    MatrixNxN()
    {
        /* 
        Q: csak simán vector<T> data; ? tudni fogja, hogy az a data az a matrixNxN data-jára vonatkozik?
        A: vector<T> data -> neki van default const-ja -> nem kell itt írni
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

    //konstruktor megadott elemekből
    template<typename F>
    MatrixNxN(int n, F f)
    {
        sizeN = n;
        n *= n;

        data.resize(n);        
        for(int i=0; i<n; i++) {data[i] = f(i);}
        /*
        Q: f(i) nem működne std::vector-ra, ezért írtam f[i]-t.
        A: eredetileg függvény lenne f, pl egy lambda -> gömbölyűvel indexelés
        így visszatértem az f(i) formátumra
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
    /*
    Q: destruktort kell írnom? az világos, hogy minden kostruktort meg kellett ha akár egyet is írtam. ha igen, hogyan?
    A: nem szükséges destruktort írni.
    */

    //műveletekhez lehessen használni vektorszerűként (csak olvasó esetekre cbegin cend)
    /*
    Q: ez így oké? nem kontárkodok így bele abba, ahogy eltárolja magát egy ilyen matrix struct egy példánya? pl nem tűnik-e/mutat-e még valami el a sizeN tag(ra)?
    A: nem lesz gond, nem akad össze semmivel
    */
    
    auto begin()   		{return data.begin();}
    auto cbegin() const {return data.cbegin();}
    auto end()      	{return data.end();}
    auto cend()   const	{return data.cend();}

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
    /*
    szóval ha jól értem, a  MatrixNxN<T> result; sor ügyes átfogalmazásával és megfelelő konstruktorok írásával a return-ig a sorok megspórolhatóak
    mikor szükséges "2 indexes konstruktort" használni? amit még nehezen tudok elképzelni, hogy hogyan kéne kinézzen
    */
    return result;
}

template<typename T>
MatrixNxN<T>&& operator+ (MatrixNxN<T>&& m1, MatrixNxN<T> const& m2)
{
    sizecheck(m1, m2);
    std::transform(m1.begin(),m1.end(),m2.cbegin(),m1.begin(),add);
    return std::move(m1);
}

template<typename T>
MatrixNxN<T>&& operator+ (MatrixNxN<T> const& m1, MatrixNxN<T>&& m2)
{
    sizecheck(m1, m2);
    std::transform(m1.cbegin(),m1.cend(),m2.begin(),m2.begin(),add);
    return std::move(m2);
}

template<typename T>
MatrixNxN<T>&& operator+ (MatrixNxN<T>&& m1, MatrixNxN<T>&& m2)
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
MatrixNxN<T>&& operator- (MatrixNxN<T>&& m1, MatrixNxN<T> const& m2)
{
    sizecheck(m1, m2);
    std::transform(m1.begin(),m1.end(),m2.cbegin(),m1.begin(),sub);
    return std::move(m1);
}

template<typename T>
MatrixNxN<T>&& operator- (MatrixNxN<T> const& m1, MatrixNxN<T>&& m2)
{
    sizecheck(m1, m2);
    std::transform(m1.cbegin(),m1.cend(),m2.begin(),m2.begin(),sub);
    return std::move(m2);
}

template<typename T>
MatrixNxN<T>&& operator- (MatrixNxN<T>&& m1, MatrixNxN<T>&& m2)
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
MatrixNxN<T>&& operator* (MatrixNxN<T>&& m, T const& scal)
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
    MatrixNxN<T> result;
    result.data.resize(m.data.size());
    result.sizeN = m.sizeN;
    std::transform(m.cbegin(),m.cend(),result.begin(),[=](T const& x){return x/scal;});
    return result;
}

template<typename T>
MatrixNxN<T>&& operator/ (MatrixNxN<T>&& m, T const& scal)
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
    T tempRes = (T)0;
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            tempRes = (T)0;
            for(int n=0; n<N; n++)
            {
                tempRes += m1(i,n)*m2(n,j);
            }
            result(i,j) = tempRes;
        }
    }
    return result;
}

//transzponálás (2 verzió)
template<typename T>
MatrixNxN<T> transpose(MatrixNxN<T> const& m)
{
    MatrixNxN<T> result;
    int N = m.sizeN;
    result.data.resize(m.data.size());
    result.sizeN = N;
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
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
MatrixNxN<T>&& transpose(MatrixNxN<T>&& m)
{
    //felső háromszögre meghívva -> ha csak 1x1-es a mátrix, nem fog működni -> ezt külön lekezelem
    int N = m.sizeN;
    
    if(N == 1){return std::move(m);}
    
    for(int i=1; i<N-1; i++)
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
