/* Jerry - A Chess Graphical User Interface
 * Copyright (C) 2014-2016 Dominik Klein
 * Copyright (C) 2015-2016 Karl Josef Klein
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include <QPainter>
#include <QGuiApplication>
#include "boardviewcontroller.h"
#include <QDebug>
#include <QMouseEvent>
#include <iostream>
#include <QGraphicsLineItem>
#include <assert.h>
#include "dialogs/dialog_promotion.h"
#include "various/resource_finder.h"
#include <math.h>

BoardViewController::BoardViewController(GameModel *gameModel, QWidget *parent) :
    Chessboard(parent)
{
    QSizePolicy *policy = new QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->setSizePolicy(*policy);

    this->colorClickSrc = new QPoint(-1,-1);

    this->transpRed = QColor(200,0,0,100);
    this->transpGreen = QColor(0,200,0,64);
    this->transpYellow = QColor(200,200,0,64);
    this->yellow = QColor(200,200,0);
    this->green = QColor(50,88,0);
    this->red = QColor(200,0,0);

    this->selArrowColor = this->green;
    this->selFieldColor = this->transpGreen;
    this->colorClick = false;

    this->gameModel = gameModel;

    this->show();
}


void BoardViewController::paintEvent(QPaintEvent *event) {
    QPainter *painter = new QPainter();
    painter->begin(this);
    this->drawBoard(event, painter);
    painter->end();
}

/*
void BoardViewController::resizeEvent(QResizeEvent* ) {
    this->setMinimumWidth(this->height());
}*/

void BoardViewController::flipBoard() {
    this->gameModel->flipBoard = !this->gameModel->flipBoard;
    this->setFlipBoard(this->gameModel->flipBoard);
    this->update();
}

void BoardViewController::resetMove() {
    this->moveSrc = scid::NULL_SQUARE;
    this->grabbedPiece->piece_type = scid::EMPTY;
    this->drawGrabbedPiece = false;
}

void BoardViewController::touchPiece(int x, int y, int mouse_x, int mouse_y) {
    this->moveSrc = this->xyToBoardIdx(x, y);
    scid::pieceT piece = this->currentPos.GetBoard()[this->moveSrc];
    scid::pieceT piece_type = scid::piece_Type(piece);
    qDebug()  << "touch piece, type: " << +(piece_type);
    scid::pieceT piece_color = scid::piece_Color(piece);
    this->grabbedPiece->piece_type = piece_type;
    qDebug()  << "touch piece, grabbed piece type: " << +(this->grabbedPiece->piece_type);
    this->grabbedPiece->color = piece_color;
    this->grabbedPiece->x = mouse_x;
    this->grabbedPiece->y = mouse_y;
    this->drawGrabbedPiece = true;
}

QPoint BoardViewController::getBoardPosition(int x, int y) {
    QPoint q(-1,-1);
    q.setX(-1);
    q.setY(-1);
    int boardSize = 0;
    int squareSize = 0;
    Chessboard::calculateBoardSize(&boardSize, &squareSize);
    int addLeft = (this->width() - boardSize) / 2;
    if(addLeft < 0) {
        addLeft = 0;
    }
    if(x > (this->borderWidth + addLeft) && y > this->borderWidth
            && x < ((boardSize +addLeft) - this->borderWidth) && y < (boardSize - this->borderWidth)) {
        x = x - (this->borderWidth + addLeft);
        y = y - (this->borderWidth);
        x = x / squareSize;
        y = 7 - (y / squareSize);
        //if(this->gameModel->flipBoard) {
        //    q.setX(7-x);
        //    q.setY(7-x);
        //} else {
            q.setX(x);
            q.setY(y);
        //}
    }
    return q;
}

scid::squareT BoardViewController::xyToBoardIdx(int x, int y) {
    // scid.A1 =

    scid::squareT board_idx = ((y)*8)+x;
    //board_idx = scid::E1;
    std::cout << "x, y, squareT:" << +(x) << " " << +(y) << " " << +(board_idx) << std::endl;

    if(this->gameModel->flipBoard) {
        board_idx = ((7-y)*8)+x;
    }
    return board_idx;
}

void BoardViewController::handleColoringOnKeyPress(QPoint &pos) {
        // user clicked and is going to draw arrow or mark a field
        if(pos.x() != -1 && pos.y() != -1 && !this->colorClick) {
            if(this->gameModel->flipBoard) {
                pos.setX(7-pos.x());
                pos.setY(7-pos.y());
            }
            this->colorClickSrc->setX(pos.x());
            this->colorClickSrc->setY(pos.y());
            this->colorClick = true;

            //this->setGrabbedArrowFrom(pos.x(), pos.y());
            //this->drawGrabbedArrow = true;
        }
}


void BoardViewController::applyMove(scid::simpleMoveT move) {
    this->gameModel->getGame()->AddMove(&move, nullptr);
    this->currentPos = scid::Position(*this->gameModel->getGame()->GetCurrentPos());
    this->gameModel->triggerStateChange();
}

scid::errorT BoardViewController::tryMove(scid::squareT from, scid::squareT to, scid::simpleMoveT *move) {

    if(this->currentPos.IsPromoMove(from, to)) {
        // first create all possible promo moves, incl. all underpromotions
        // if one of them is legal, we give the user a selection choice
        // this move is then executed.
        QString uciPromoRook = this->FromToToUci(from, to, "R");
        QString uciPromoKnight = this->FromToToUci(from, to, "N");
        QString uciPromoBishop = this->FromToToUci(from, to, "B");
        QString uciPromoQueen = this->FromToToUci(from, to, "Q");
        const char *uci_pr_as_c_str = uciPromoRook.toStdString().c_str();
        const char *uci_pn_as_c_str = uciPromoKnight.toStdString().c_str();
        const char *uci_pb_as_c_str = uciPromoBishop.toStdString().c_str();
        const char *uci_pq_as_c_str = uciPromoQueen.toStdString().c_str();
        scid::simpleMoveT moveR;
        scid::errorT errR = this->currentPos.ReadCoordMove(&moveR, uci_pr_as_c_str, false);
        scid::simpleMoveT moveN;
        scid::errorT errN = this->currentPos.ReadCoordMove(&moveN, uci_pn_as_c_str, false);
        scid::simpleMoveT moveB;
        scid::errorT errB = this->currentPos.ReadCoordMove(&moveB, uci_pb_as_c_str, false);
        scid::simpleMoveT moveQ;
        scid::errorT errQ = this->currentPos.ReadCoordMove(&moveQ, uci_pq_as_c_str, false);
        if(errR == scid::OK || errN == scid::OK || errB == scid::OK || errQ == scid::OK) {
            // one of these is ok, so give user a choice. Chance remains that
            // he selects an impossible move
            DialogPromotion d(this->currentPos.GetToMove(),this->parentWidget());
            d.exec();
            QString uci = this->FromToToUci(from, to, d.promotesTo);
            const char *uci_as_c_str = uci.toStdString().c_str();
            scid::errorT err = this->currentPos.ReadCoordMove(move, uci_as_c_str, false);
            return err;
            //if(err == scid::OK) {
            //    this->applyMove(move);
            //    this->resetMove();
            //}
        } else {
            return scid::ERROR_General;
        }
    } else {
        // definitely not promoting, so check if legal move
        QString uci = this->FromToToUci(from, to);
        const char *uci_as_c_str = uci.toStdString().c_str();
        //scid::simpleMoveT move;
        scid::errorT err = this->currentPos.ReadCoordMove(move, uci_as_c_str, false);
        return err;
        //if(err == scid::OK) {
        //    this->applyMove(move);
        //    this->resetMove();
        //}
    }
}

void BoardViewController::mousePressEvent(QMouseEvent *me) {

    qDebug() << "MOUSE Press Event:";
    QPoint pos = this->getBoardPosition(me->x(), me->y());
    if(me->button() == Qt::RightButton)
    {
        this->handleColoringOnKeyPress(pos);
    } else if(me->button() == Qt::LeftButton) {        
        qDebug() << "mousePressEvent, before pos check, grabbed piece type: " << +(this->grabbedPiece->piece_type);
        //chess::Board b = this->gameModel->getGame()->getCurrentNode()->getBoard();
        scid::squareT to_square = this->xyToBoardIdx(pos.x(), pos.y());
        if(pos.x() != -1 && pos.y() != -1) {
            qDebug() << "mousePressEvent, grabbed piece type: " << +(this->grabbedPiece->piece_type);
            if(this->grabbedPiece->piece_type != scid::EMPTY) {
                scid::simpleMoveT move;
                scid::errorT err = this->tryMove(this->moveSrc, to_square, &move);
                if(err == scid::OK) {
                    this->applyMove(move);
                    this->resetMove();
                } else {
                    this->resetMove();
                    if(this->currentPos.GetBoard()[to_square] != scid::EMPTY) {
                        this->touchPiece(pos.x(),pos.y(),me->x(),me->y());
                   }
                }
            } else {
                //this->resetMove();
                if(this->currentPos.GetBoard()[to_square] != scid::EMPTY) {
                    this->touchPiece(pos.x(),pos.y(),me->x(),me->y());
               }
            }
        }
    }

}

void BoardViewController::mouseMoveEvent(QMouseEvent *m) {


    Qt::MouseButton button = m->button();
    if(button == Qt::NoButton &&
            //this->grabbedPiece->piece_type != scid::EMPTY
            this->drawGrabbedPiece && this->grabbedPiece->piece_type != scid::EMPTY && !this->colorClick) {
        this->grabbedPiece->x = m->x();
        this->grabbedPiece->y = m->y();
        this->drawGrabbedPiece = true;
        this->update();
    }
    if(button == Qt::NoButton && this->drawGrabbedArrow) {
        QPoint xy = this->getBoardPosition(m->x(),m->y());
        if(this->gameModel->flipBoard) {
            xy.setX(7-xy.x());
            xy.setY(7-xy.y());
        }
        //this->setGrabbedArrowTo(xy.x(), xy.y());
        this->update();
    }

}

void BoardViewController::handleColoringonKeyRelease(QPoint &pos) {
/*
        // user clicked and is going to draw arrow
        if(pos->x() != -1 && pos->y() != -1 && this->colorClick) {
            if(this->gameModel->flipBoard) {
                pos->setX(7-pos->x());
                pos->setY(7-pos->y());
            }
            // arrow case
            if(pos->x() != this->colorClickSrc->x() || pos->y() != this->colorClickSrc->y()) {
                chess::Arrow a; // = new chess::Arrow();
                a.from = QPoint(this->colorClickSrc->x(), this->colorClickSrc->y());
                a.to = QPoint(pos->x(), pos->y());
                //a->color = this->red;
                a.color = this->green;
                this->gameModel->getGame()->getCurrentNode()->addOrDelArrow(a);
            } else {
                chess::ColoredField c; // = new chess::ColoredField();
                c.field = QPoint(pos->x(), pos->y());
                c.color = this->transpRed;
                this->gameModel->getGame()->getCurrentNode()->addOrDelColoredField(c);
            }
        }
        this->drawGrabbedArrow = false;
*/
}

QString BoardViewController::FromToToUci(scid::squareT from, scid::squareT to, QString promo) {
    QString uci = "";
    uci.append(scid::square_FyleChar(from));
    uci.append(scid::square_RankChar(from));
    uci.append(scid::square_FyleChar(to));
    uci.append(scid::square_RankChar(to));
    if(!promo.isEmpty()) {
        uci.append(promo);
    }
    return uci;
}


void BoardViewController::mouseReleaseEvent(QMouseEvent *m) {

    this->drawGrabbedPiece = false;
    QPoint pos = this->getBoardPosition(m->x(), m->y());
    if(m->button() == Qt::RightButton)
    {
        this->handleColoringonKeyRelease(pos);

    } else if(m->button() == Qt::LeftButton){

        qDebug() << "mouse release event";
        //chess::Board b = this->gameModel->getGame()->getCurrentNode()->getBoard();
        if(pos.x() != -1 && pos.y() != -1 && this->grabbedPiece->piece_type != scid::EMPTY) {
            qDebug() << "piece type not empty";
            //this->resetMove();
            scid::squareT release_square = this->xyToBoardIdx(pos.x(), pos.y());
            if(!(release_square == this->moveSrc)) {
                scid::simpleMoveT move;
                scid::errorT err = this->tryMove(this->moveSrc, release_square, &move);
                if(err == scid::OK) {
                    this->applyMove(move);
                } else {
                    this->resetMove();
                }
            }
        }
    }
    this->colorClick = false;
    this->update();

}


void BoardViewController::onStateChange() {
    /*
    this->lastMove = this->gameModel->getGame()->getCurrentNode()->getMove();
    this->style = this->gameModel->colorStyle;
    this->setFlipBoard(this->gameModel->flipBoard);
    this->update();
    */
}

void BoardViewController::drawBoard(QPaintEvent *event, QPainter *painter) {

    /*
    //Chessboard::board = this->gameModel->getGame()->getCurrentNode()->getBoard();
    //Chessboard::currentArrows = this->gameModel->getGame()->getCurrentNode()->getArrows();
    //Chessboard::currentColoredFields = this->gameModel->getGame()->getCurrentNode()->getColoredFields();
    chess::GameNode *node = this->gameModel->getGame()->getCurrentNode();
    this->setBoard(node->getBoard());
    this->setArrows(node->getArrows());
    this->setColoredFields(node->getColoredFields());
    */
    Chessboard::drawBoard(event, painter);

}
