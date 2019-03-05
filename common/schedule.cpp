#include "common/schedule.h"

bool operator== (const Schedule &s1, const Schedule &s2)
{
    return (s1.id == s2.id)
            && Schedule(s1).getInList() == Schedule(s2).getInList();
}

bool operator!= (const Schedule &s1, const Schedule &s2)
{
    return !(s1 == s2);
}

