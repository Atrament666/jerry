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

    scid::Game g;
    scid::simpleMoveT move_e2e4;
    move_e2e4.from = scid::E2;
    move_e2e4.to = scid::E4;
    char* snp = (char *)"e4";  // extremely fugly
    g.AddMove(&move_e2e4, snp);
    char san[CHAR_MAX];
    g.GetSAN(san);
    std::cout << "game.getSan()" << san << std::endl;
    auto ret = g.WriteToPGN();
    std::cout << "game.WriteToPGN(), first :" << ret.first << std::endl;
    std::cout << "game.WriteToPGN(), second:" << ret.second << std::endl;

    scid::simpleMoveT move_e2e3;

    // proper way of reading moves:

    scid::Position * pos = g.GetCurrentPos();
    scid::errorT err = pos->ReadCoordMove(&move_e2e3, "e2e3", 4);

    std::cout << "game.addMove illegal:" << err << std::endl;
    if(err == scid::ERROR_InvalidMove) {
        std::cout << "bad move!" << std::endl;
    }

    // check if move w/o specific promotion marker is checked to be legal
    //  bool        IsPromoMove (squareT from, squareT to); useful?
    // resp. generate all four possible moves and check for errors

}
