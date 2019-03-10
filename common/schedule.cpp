#include "common/schedule.h"

bool operator== (const Schedule &s1, const Schedule &s2)
{
    return (s1.id == s2.id)
            && Schedule(s1).getFullList() == Schedule(s2).getFullList()
            && Schedule(s1).groups == Schedule(s2).groups;
}

bool operator!= (const Schedule &s1, const Schedule &s2)
{
    return !(s1 == s2);
}

