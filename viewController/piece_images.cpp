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


#include <QtSvg/QSvgRenderer>
#include <QtGui>
#include "piece_images.h"
//#include "chess/board.h"
//#include "chess/move.h"
#include "colorstyle.h"
#include <QDebug>
#include <assert.h>
#include "various/messagebox.h"
#include "model/game_model.h"

PieceImages::PieceImages(QString resourcePath) {
    this->resourcePath = QString(resourcePath);

    QString merida = QString("merida");
    QString uscf = QString("uscf");
    QString old = QString("old");

    this->svg_images_merida = new QHash<std::pair<scid::pieceT,scid::colorT>, QSvgRenderer*>();

    this->svg_images_old = new QHash<std::pair<scid::pieceT,scid::colorT>, QSvgRenderer*>();
    this->svg_images_uscf = new QHash<std::pair<scid::pieceT,scid::colorT>, QSvgRenderer*>();

    this->initSvgs(svg_images_merida, merida);
    this->initSvgs(svg_images_old, old);
    this->initSvgs(svg_images_uscf, uscf);

    this->rendered_svg_images_merida = new QHash<QString, QImage*>();
    this->rendered_svg_images_old = new QHash<QString, QImage*>();
    this->rendered_svg_images_uscf = new QHash<QString, QImage*>();

}

void PieceImages::initSvgs(QHash<std::pair<scid::pieceT,scid::colorT>, QSvgRenderer*> *svg_images, QString &pieceStyle) {

    QString filename = QString(":/res/pieces/");
    filename.append(pieceStyle);
    filename.append("/");

    //QString fn_ba = QString(filename).append("ba.svg");
    //QString fn_wa = filename.append("wa.svg");

    QString fn_bp = QString(filename).append("bp.svg");
    QString fn_wp = QString(filename).append("wp.svg");

    QString fn_br = QString(filename).append("br.svg");
    QString fn_wr = QString(filename).append("wr.svg");

    QString fn_bn = QString(filename).append("bn.svg");
    QString fn_wn = QString(filename).append("wn.svg");

    QString fn_bb = QString(filename).append("bb.svg");
    QString fn_wb = QString(filename).append("wb.svg");

    QString fn_bq = QString(filename).append("bq.svg");
    QString fn_wq = QString(filename).append("wq.svg");

    QString fn_bk = QString(filename).append("bk.svg");
    QString fn_wk = QString(filename).append("wk.svg");

    //QSvgRenderer *ren_ba = new QSvgRenderer();
    //QSvgRenderer *ren_wa = new QSvgRenderer();

    QSvgRenderer *ren_bp = new QSvgRenderer();
    QSvgRenderer *ren_wp = new QSvgRenderer();

    QSvgRenderer *ren_br = new QSvgRenderer();
    QSvgRenderer *ren_wr = new QSvgRenderer();

    QSvgRenderer *ren_bn = new QSvgRenderer();
    QSvgRenderer *ren_wn = new QSvgRenderer();

    QSvgRenderer *ren_bb = new QSvgRenderer();
    QSvgRenderer *ren_wb = new QSvgRenderer();

    QSvgRenderer *ren_bq = new QSvgRenderer();
    QSvgRenderer *ren_wq = new QSvgRenderer();

    QSvgRenderer *ren_bk = new QSvgRenderer();
    QSvgRenderer *ren_wk = new QSvgRenderer();

    //ren_ba->load(fn_ba);
    //ren_wa->load(fn_wa);

    ren_bp->load(fn_bp);
    ren_wp->load(fn_wp);

    ren_br->load(fn_br);
    ren_wr->load(fn_wr);

    ren_bn->load(fn_bn);
    ren_wn->load(fn_wn);

    ren_bb->load(fn_bb);
    ren_wb->load(fn_wb);

    ren_bq->load(fn_bq);
    ren_wq->load(fn_wq);

    ren_bk->load(fn_bk);
    ren_wk->load(fn_wk);

    //svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::ANY_PIECE, scid::BLACK), ren_ba);
    //svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::ANY_PIECE, scid::WHITE), ren_wa);

    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::PAWN, scid::BLACK), ren_bp);
    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::PAWN, scid::WHITE), ren_wp);

    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::ROOK, scid::BLACK), ren_br);
    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::ROOK, scid::WHITE), ren_wr);

    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::KNIGHT, scid::BLACK), ren_bn);
    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::KNIGHT, scid::WHITE), ren_wn);

    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::BISHOP, scid::BLACK), ren_bb);
    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::BISHOP, scid::WHITE), ren_wb);

    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::QUEEN, scid::BLACK), ren_bq);
    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::QUEEN, scid::WHITE), ren_wq);

    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::KING, scid::BLACK), ren_bk);
    svg_images->insert(std::pair<scid::pieceT, scid::colorT>(scid::KING, scid::WHITE), ren_wk);
}

QImage* PieceImages::getPieceImage(scid::pieceT piece_type, scid::colorT color,
                                   int size, qreal dpr, int type) {


    QHash<std::pair<scid::pieceT, scid::colorT>, QSvgRenderer*> *svg_images;
    QHash<QString, QImage*> *rendered_svg_images;
    if(type == PIECE_STYLE_OLD) {
        svg_images = this->svg_images_old;
        rendered_svg_images = this->rendered_svg_images_old;
    } else if(type == PIECE_STYLE_USCF) {
        svg_images = this->svg_images_uscf;
        rendered_svg_images = this->rendered_svg_images_uscf;
    } else {
        svg_images = this->svg_images_merida;
        rendered_svg_images = this->rendered_svg_images_merida;
    }
    QString idx = QString::number(piece_type).append("_").append(QString::number(color));
    idx.append("_").append(QString::number(size));

    if(rendered_svg_images->contains(idx)) {
        return rendered_svg_images->value(idx);
    } else {
        QImage* img = new QImage(size*dpr,size*dpr,QImage::Format_ARGB32);
        QColor fill = QColor(1,1,1,1);
        img->fill(fill);
        QPainter *painter = new QPainter();
        assert(!img->isNull());
        painter->begin(img);
        QSvgRenderer *ren = svg_images->value(std::pair<scid::pieceT, scid::colorT>(piece_type, color));
        assert(ren != 0);
        ren->render(painter);
        painter->end();
        img->setDevicePixelRatio(dpr);
        delete painter;
        rendered_svg_images->insert(idx,img);
        return img;
    }

}
