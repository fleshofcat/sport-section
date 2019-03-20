#include "common/group.h"

bool operator== (const Group &g1, const Group &g2)
{
    return (g1.id == g2.id)
            && Group(g1).getFullProperty() == Group(g2).getFullProperty()
            && Group(g1).trainers == Group(g2).trainers
            && Group(g1).sportsmen == Group(g2).sportsmen;
}

bool operator!= (const Group &g1, const Group &g2)
{
    return !(g1 == g2);
}






