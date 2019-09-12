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


#ifndef PIECE_IMAGES_H
#define PIECE_IMAGES_H

#include <QtSvg/QSvgRenderer>
#include "scid/common.h"

class PieceImages
{
public:
    PieceImages(QString resourcePath);
    QImage* getPieceImage(pieceT piece_type, colorT color, int size, qreal dpr, int type=0);

private:
    QString resourcePath;
    QHash<std::pair<pieceT,colorT>, QSvgRenderer*> *svg_images_merida;
    QHash<std::pair<pieceT,colorT>, QSvgRenderer*> *svg_images_old;
    QHash<std::pair<pieceT,colorT>, QSvgRenderer*> *svg_images_uscf;
    QHash<QString, QImage*> *rendered_svg_images_merida;
    QHash<QString, QImage*> *rendered_svg_images_old;
    QHash<QString, QImage*> *rendered_svg_images_uscf;
    void initSvgs(QHash<std::pair<pieceT,colorT>, QSvgRenderer*> *svg_images, QString &pieceStyle);

};

#endif // PIECE_IMAGES_H
