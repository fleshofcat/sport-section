#pragma once

#include <QString>

class Relation
{
public:
    Relation(){}
    Relation(int trainer_id, int child_id)
    {
        this->trainer_id = trainer_id;
        this->child_id = child_id;
    }

    bool isFull()
    {
        return     trainer_id > 0
                && child_id > 0;
    }

    int id = 0;
    int trainer_id = 0;
    int child_id = 0;
};



