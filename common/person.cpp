#include "common/person.h"

bool operator== (const Person &p1, const Person &p2)
{
    return (p1.id == p2.id)
            && Person(p1).getInList() == Person(p2).getInList();
}

bool operator!= (const Person &p1, const Person &p2)
{
    return !(p1 == p2);
}






