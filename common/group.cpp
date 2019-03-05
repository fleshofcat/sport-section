#include "common/group.h"

bool operator== (const Group &g1, const Group &g2)
{
    return (g1.id == g2.id)
            && Group(g1).getInList() == Group(g2).getInList();
}

bool operator!= (const Group &g1, const Group &g2)
{
    return !(g1 == g2);
}






