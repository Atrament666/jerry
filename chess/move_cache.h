#ifndef MOVE_CACHE_H
#define MOVE_CACHE_H

#include <QVector>
#include "chess/move.h"

namespace chess {

class MoveCache
{
public:
    MoveCache();
    void addMove(uint8_t from, uint8_t to);
    void addMove(uint8_t from, uint8_t to, uint8_t promotion_piece);
    void reset();
    int count();

    QVector<chess::Move> cache;

private:
    int useCount;

};

}

#endif // MOVE_CACHE_H
