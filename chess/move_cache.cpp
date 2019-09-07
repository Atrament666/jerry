#include "move_cache.h"

namespace chess {

MoveCache::MoveCache()
{
    for(int i=0;i<500;i++) {
        this->cache.append(Move());
    }
    this->useCount = 0;
}

void MoveCache::reset() {
    this->useCount = 0;
}

int MoveCache::count() {
    return this->useCount;
}

void MoveCache::addMove(uint8_t from, uint8_t to) {

    // create ascii (latin1) code numbers from
    // uint8_t board pos numbers
    QChar col_from = QChar((from % 10) + 96);
    QChar row_from = QChar((from / 10) + 47);

    QChar col_to = QChar((to % 10) + 96);
    QChar row_to = QChar((to / 10) + 47);

    this->cache[this->useCount].from = from;
    this->cache[this->useCount].to = to;
    this->cache[this->useCount].promotion_piece = 0;
    this->cache[this->useCount].uci_string = QString(col_from) + row_from + col_to + row_to;
    this->cache[this->useCount].is_null = false;

    this->useCount++;
}

void MoveCache::addMove(uint8_t from, uint8_t to, uint8_t promotion_piece) {
    QChar col_from = QChar((from % 10) + 96);
    QChar row_from = QChar((from / 10) + 47);

    QChar col_to = QChar((to % 10) + 96);
    QChar row_to = QChar((to / 10) + 47);

    QChar prom_piece;
    if(promotion_piece == KNIGHT) {
        prom_piece = QChar('N');
    } else if (promotion_piece == BISHOP) {
        prom_piece = QChar('B');
    } else if (promotion_piece == ROOK) {
        prom_piece = QChar('R');
    } else if (promotion_piece == QUEEN) {
        prom_piece = QChar('Q');
    } else {
        prom_piece = QChar(' ');
    }
    this->cache[this->useCount].from = from;
    this->cache[this->useCount].to = to;
    this->cache[this->useCount].promotion_piece = promotion_piece;
    if(prom_piece == QChar(' ')) {
        this->cache[this->useCount].uci_string = QString(col_from) + row_from + col_to + row_to;
    } else {
        this->cache[this->useCount].uci_string = QString(col_from) + row_from + col_to + row_to + prom_piece;
    }
    this->cache[this->useCount].is_null = false;

    this->useCount++;
}






}
