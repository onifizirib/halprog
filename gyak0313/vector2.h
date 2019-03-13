#include <iostream>
#include <cmath>

//szükségem van egy négyzetre emelő függvényre
template<typename T>
T sq(T const&a)
{
    return a*a;
}


template<typename T>
struct Vector2
{
    T x, y;
     
     //skalárral szorzás... valamelyik oldalról..
    Vector2<T> operator*(T const& p)
    {
        return Vector2{x*p, y*p};
        //nem kell this->x, elég x
    }
    //TODO: másik oldalról szorzás

     //skalárral osztás
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

     //elforgatás

     //+= operátor
     

};

//összeadás
template<typename T>
Vector2<T> operator+(Vector2<T> const& a, Vector2<T> const& b)
{
    return Vector2<T>{a.x + b.x, a.y + b.y};
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

//kiiratás << operátorral
template<typename T>
std::ostream& operator <<(std::ostream& o, Vector2<T> const& v)
{
    o << v.x << ", " << v.y;
    return o;
}