#include "common/schedule.h"

bool operator== (const Schedule &s_1, const Schedule &s_2)
{
    return (s_1.id == s_2.id)
            && Schedule(s_1).getInList() == Schedule(s_2).getInList();
}

bool operator!= (const Schedule &s_1, const Schedule &s_2)
{
    return !(s_1 == s_2);
}

