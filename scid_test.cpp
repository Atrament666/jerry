#include "scid_test.h"
#include "scid/game.h"
#include "scid/error.h"
#include <iostream>

ScidTest::ScidTest()
{

}

void ScidTest::run() {

    // basic game creation
    // wrong: use g.GetCurrentPos and ReadCoordMove first!

    Game g;
    simpleMoveT move_e2e4;
    move_e2e4.from = E2;
    move_e2e4.to = E4;
    g.AddMove(&move_e2e4);
    char san[CHAR_MAX];
    g.GetSAN(san);
    std::cout << "game.getSan()" << san << std::endl;
    auto ret = g.WriteToPGN();
    std::cout << "game.WriteToPGN(), first :" << ret.first << std::endl;
    std::cout << "game.WriteToPGN(), second:" << ret.second << std::endl;

    simpleMoveT move_e2e3;

    // proper way of reading moves:

    Position * pos = g.GetCurrentPos();
    errorT err = pos->ReadCoordMove(&move_e2e3, "e2e3", 4, false);

    std::cout << "game.addMove illegal:" << err << std::endl;
    if(err == ERROR_InvalidMove) {
        std::cout << "bad move!" << std::endl;
    }

    // check if move w/o specific promotion marker is checked to be legal


}
