#pragma once

#include <QString>

class Relation
{
public:
    Relation(){}
    Relation(int trainer_id, int child_id, QString sportType)
    {
        this->trainer_id = trainer_id;
        this->child_id = child_id;
        this->sportType = sportType;
    }

    bool isFull()
    {
        return     trainer_id > 0
                && child_id > 0
                && !sportType.isEmpty();
    }

    int id = 0;
    int trainer_id = 0;
    int child_id = 0;
    QString sportType;
};



