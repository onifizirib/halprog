#include <iostream>
#include <cmath>

//szükségem van egy négyzetre emelő függvényre
template<typename T>
T sq(T const&a)
{
    return a*a;
}
//------------------------------------------

template<typename T>
struct Vector2
{
    T x, y;  


     //skalárral osztás (mivel azt úgyis csak egyik oldalról lehet)
     Vector2<T> operator/(T const& p)
     {
         if ((T)0 == p)
         {
             /*
             oké, de mit adjunk vissza? egyelőre csak kiírok hibaüzenetet, de a return független az if két ágától. 
             kérdés kell-e include<iostream> ide, vagy elég a main.cpp-ben. vagy egyaltalán header-ben lehet-e include-olni :D
             */
             std::cout << "dividing with zero!" << std::endl;
         }
         return Vector2{x/p, y/p};
         //nem kell this->x, elég x
     }

    //hossznégyzet
    /*
    Q: végülis ehhez nem kell semmi más bemenet, miért ne lehetne ide írni? csak máshogy kell meghívni majd,
       v.len() vagy len(v)-ként.
    A: bár belegondolva az lehet az értelmesebb csoportosítás, hogy mi az ami módosítja az objektumot -> az kerüljön ide be.
    */

     //normalizálás
     Vector2<T> norm()
     {
        /*
        Q: ejj, ide csak kéne egy hosszt meghívni..
        A: szerencsére templatekkel nincs ilyen gond: csak akkor lesz belőle kód, ha behelyettesítésre kerül bele valami
           addig viszont vígan "beolvasódik" a headerem. tehát használhatom a később megírt len() fv-t.
        */
        if (len(this) == (T)0)
        {
            std::cout << "vector with zero length is being normalised!" << std::endl;
        }
        return Vector2{x/len(this), y/len(this)};
     }

     //elforgatás egy radiánban adott szöggel
     Vector2<T> rotate(T const& angle)
     {
         /*
         T típusú a szög, az egyszerűség kedvéért, hogy ne kelljen a vektor típusán is változtatni
         persze ha kívülre raknám, akkor valószínűleg elég lenne egy másféle template-s vektorral visszatérni
         */
         return Vector2{x*cos(angle)-y*sin(angle), x*sin(angle)+y*cos(angle)};
     }

     //+= operátor
     Vector2<T>& operator+=(Vector2<T> const& v)
    {
        x += v.x; 
        y += v.y;
        return *this;
    }
    
    //-= operátor
    Vector2<T>& operator-=(Vector2<T> const& v)
    {
        x -= v.x; 
        y -= v.y;
        return *this;
    }
    
    //*= operátor csak skalárral való szorzásra lesz értelmezve, megkülönböztetve a dot-tól
    Vector2<T>& operator*=(T const& p)
    {
        x *= p; 
        y *= p;
        return *this;
    }
    
    // /= operátor csak skalárral való jobbról osztásra lesz értelmezve
    Vector2<T>& operator/=(T const& p)
    {
        x /= p; 
        y /= p;
        return *this;
    }    

};

//----------------------------------------------

//skalárral szorzás bal oldalról
template<typename T>
Vector2<T> operator*(T const& p, Vector2<T>& v)
{
    return Vector2<T>{v.x*p, v.y*p};
}

//skalárral szorzás jobb oldalról
template<typename T>
Vector2<T> operator*(Vector2<T>& v, T const& p)
{
    return Vector2<T>{v.x*p, v.y*p};
}

//összeadás
template<typename T>
Vector2<T> operator+(Vector2<T> const& a, Vector2<T> const& b)
{
    return Vector2<T>{a.x + b.x, a.y + b.y};
}

//kivonás
template<typename T>
Vector2<T> operator-(Vector2<T> const& a, Vector2<T> const& b)
{
    return Vector2<T>{a.x - b.x, a.y - b.y};
}

//skalárszorzat
template<typename T>
T dot(Vector2<T> const& a, Vector2<T> const& b)
{
    return a.x*b.x + a.y*b.y;
}

//hossz
template<typename T>
T len(Vector2<T> const& v)
{
    return sqrt(sq(v.x) + sq(v.y));
    //ehhez kell cmath
}

//hossznégyzet
template<typename T>
T sqlen(Vector2<T> const& v)
{
    return sq(v.x)+sq(v.y);
    //v.x^2 bitenként xor-ozza a kettővel. írjunk külön függvényt
}

template<typename T>
Vector2<T> norm(Vector2<T> const& v)
{
    if (len(v) == (T)0)
    {
        std::cout << "vector with zero length is being normalised!" << std::endl;
    }
    return Vector2<T>{v.x/len(v), v.y/len(v)};
}

//kiiratás << operátorral
template<typename T>
std::ostream& operator <<(std::ostream& o, Vector2<T> const& v)
{
    o << v.x << ", " << v.y;
    return o;
}
