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

    this->svg_images_merida = new QHash<std::pair<pieceT,colorT>, QSvgRenderer*>();

    this->svg_images_old = new QHash<std::pair<pieceT,colorT>, QSvgRenderer*>();
    this->svg_images_uscf = new QHash<std::pair<pieceT,colorT>, QSvgRenderer*>();

    this->initSvgs(svg_images_merida, merida);
    this->initSvgs(svg_images_old, old);
    this->initSvgs(svg_images_uscf, uscf);

    this->rendered_svg_images_merida = new QHash<QString, QImage*>();
    this->rendered_svg_images_old = new QHash<QString, QImage*>();
    this->rendered_svg_images_uscf = new QHash<QString, QImage*>();

}

void PieceImages::initSvgs(QHash<std::pair<pieceT,colorT>, QSvgRenderer*> *svg_images, QString &pieceStyle) {

    QString filename = QString(":/res/pieces/");
    filename.append(pieceStyle);
    filename.append("/");

    QString fn_ba = filename.append("ba.svg");
    QString fn_wa = filename.append("wa.svg");

    QString fn_bp = filename.append("bp.svg");
    QString fn_wp = filename.append("wp.svg");

    QString fn_br = filename.append("br.svg");
    QString fn_wr = filename.append("wr.svg");

    QString fn_bn = filename.append("bn.svg");
    QString fn_wn = filename.append("wn.svg");

    QString fn_bb = filename.append("bb.svg");
    QString fn_wb = filename.append("wb.svg");

    QString fn_bq = filename.append("bq.svg");
    QString fn_wq = filename.append("wq.svg");

    QString fn_bk = filename.append("bk.svg");
    QString fn_wk = filename.append("wk.svg");

    QSvgRenderer *ren_ba = new QSvgRenderer();
    QSvgRenderer *ren_wa = new QSvgRenderer();

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

    ren_ba->load(fn_ba);
    ren_wa->load(fn_wa);

    ren_ba->load(fn_bp);
    ren_wa->load(fn_wp);

    ren_ba->load(fn_br);
    ren_wa->load(fn_wr);

    ren_ba->load(fn_bn);
    ren_wa->load(fn_wn);

    ren_ba->load(fn_bb);
    ren_wa->load(fn_wb);

    ren_ba->load(fn_bq);
    ren_wa->load(fn_wq);

    ren_ba->load(fn_bk);
    ren_wa->load(fn_wk);

    svg_images->insert(std::pair(INVALID_PIECE, BLACK), ren_ba);
    svg_images->insert(std::pair(INVALID_PIECE, WHITE), ren_wa);

    svg_images->insert(std::pair(PAWN, BLACK), ren_bp);
    svg_images->insert(std::pair(PAWN, WHITE), ren_wp);

    svg_images->insert(std::pair(ROOK, BLACK), ren_br);
    svg_images->insert(std::pair(ROOK, WHITE), ren_wr);

    svg_images->insert(std::pair(KNIGHT, BLACK), ren_bn);
    svg_images->insert(std::pair(KNIGHT, WHITE), ren_wn);

    svg_images->insert(std::pair(BISHOP, BLACK), ren_bb);
    svg_images->insert(std::pair(BISHOP, WHITE), ren_wb);

    svg_images->insert(std::pair(QUEEN, BLACK), ren_bq);
    svg_images->insert(std::pair(QUEEN, WHITE), ren_wq);

    svg_images->insert(std::pair(KING, BLACK), ren_bk);
    svg_images->insert(std::pair(KING, WHITE), ren_wk);
}

QImage* PieceImages::getPieceImage(pieceT piece_type, colorT color, int size, qreal dpr, int type) {


    QHash<std::pair<pieceT, colorT>, QSvgRenderer*> *svg_images;
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
        QSvgRenderer *ren = svg_images->value(std::pair(piece_type, color));
        assert(ren != 0);
        ren->render(painter);
        painter->end();
        img->setDevicePixelRatio(dpr);
        delete painter;
        rendered_svg_images->insert(idx,img);
        return img;
    }

}
