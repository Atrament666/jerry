#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include "viewController/piece_images.h"
#include "viewController/colorstyle.h"
//#include "chess/board.h"
//#include "chess/game_node.h"
#include "scid/position.h"

struct GrabbedPiece {
    scid::pieceT piece_type;
    scid::colorT color;
    int x;
    int y;
};

class Chessboard : public QWidget
{
    Q_OBJECT

public:
    explicit Chessboard(QWidget *parent = 0);
    void setColorStyle(ColorStyle *style);
    ColorStyle* getColorStyle();
    void setFlipBoard(bool onOff);
    void setGrabbedPiece(int piece, int color);
    void setBoard(scid::Position p);
    //void setArrows(QVector<chess::Arrow> arrows);
    //void setColoredFields(QVector<chess::ColoredField> fields);

    void setGrabbedArrowFrom(int x, int y);
    void setGrabbedArrowTo(int x, int y);

private:

    //QVector<chess::Arrow> currentArrows;
    //QVector<chess::ColoredField> currentColoredFields;

protected:

    //chess::Move lastMove;
    scid::squareT moveSrc;
    bool drawGrabbedPiece;
    bool drawGrabbedArrow;
    struct GrabbedPiece *grabbedPiece;
    //struct chess::Arrow *grabbedArrow;
    bool flipBoard;

    QColor *arrowGrabColor;
    QColor *lastMoveColor;

    ColorStyle *style;
    int borderWidth;
    PieceImages *pieceImages;
    //chess::Board board;
    scid::Position currentPos;

    // device pixel ratio
    qreal dpr;

    void drawBoard(QPaintEvent *event, QPainter *painter);
    void calculateBoardSize(int *boardSize, int *squareSize);
    //void drawArrow(const chess::Arrow &ai, int boardOffsetX, int boardOffsetY, int squareSize, QPainter *painter);

    void paintEvent(QPaintEvent *e);
    //void resizeEvent(QResizeEvent *e);
    QPoint scidSqureToPos(int boardOffsetX, int boardOffsetY,
                          int squareSize, scid::squareT square);

signals:

public slots:

};

#endif // CHESSBOARD_H
