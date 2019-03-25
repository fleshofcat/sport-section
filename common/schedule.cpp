#include "common/schedule.h"

bool operator== (const Schedule &s1, const Schedule &s2)
{
    return (s1.id == s2.id)
            && Schedule(s1).getSavebleProperty() == Schedule(s2).getSavebleProperty()
            && Schedule(s1).getGroupsIds() == Schedule(s2).getGroupsIds();
}

bool operator!= (const Schedule &s1, const Schedule &s2)
{
    return !(s1 == s2);
}

