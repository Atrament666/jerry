/*
* Copyright (C) 2014  Fulvio Benini

* This file is part of Scid (Shane's Chess Information Database).
*
* Scid is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation.
*
* Scid is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Scid.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FULLMOVE_H
#define FULLMOVE_H

#include "common.h"
#include <string>

namespace scid {

class FullMove {
	// ** Lower 16 bits are compatible with Stockfish's Move **
	// bits  0- 5: destination square (from 0 to 63)
	// bits  6-11: origin square (from 0 to 63)
	// bits 12-13: promotion piece type -2 (from QUEEN-2 to KNIGHT-2)
	// bits 14-15: special move flag: promotion (1), en passant (2), castling (3)

	// ** Info for undoing the move **
	// bits 16-17: castling flags - TODO
	// bits 18-20: enpassant file - TODO
	// bits 21-23: captured pieceT

	// ** Info for direct SAN conversion **
	// bits 24-26: moving pieceT
	// bit     27: black to move
	// bit     28: ambiguous move, insert from fyle
	// bit     29: ambiguous move, insert from rank
	// bit     30: check

	// ** TODO: Use this flag to embed tags, variations, etc.. in a move stream
	// bit     31: special flag
	uint32_t m_;

public:
	FullMove(uint32_t m = 0) : m_(m) {};
	bool operator!=(const FullMove& f) const { return m_ != f.m_; }
	bool    isNull()      const { return (m_ & 0xFFFF) == 0; }
	bool    isPromo()     const { return (m_ & (3 << 14)) == (1 << 14); }
	bool    isEnpassant() const { return (m_ & (3 << 14)) == (2 << 14); }
	bool    isCastle()    const { return (m_ & (3 << 14)) == (3 << 14); }
	squareT getTo()       const { return m_ & 0x3F; }
	squareT getFrom()     const { return (m_ >> 6) & 0x3F; }
	pieceT  getPiece()    const { return (m_ >> 24) & 0x07; }
	colorT  getColor()    const { return (m_ >> 27 & 1) ? BLACK : WHITE; }
	pieceT  getPromo()    const { return ((m_ >> 12) & 0x03) +2; }
	pieceT  getCaptured() const { return (m_ >> 21) & 0x07; }
	squareT getCaptSq()   const {
		squareT res = getTo();
		if (isEnpassant()) res += (getColor() == WHITE) ? -8 : +8;
		return res;
	}
	std::string getSAN(colorT* toMove = 0) const {
		std::string res;
		if (toMove) *toMove = getColor();
		squareT to = getTo();
		squareT from = getFrom();
		if (to == 0 && from == 0) return "--";
		if (isCastle()) return (to > from) ? "O-O" : "O-O-O";
		bool fromFyle = (m_ >> 28) & 1;
		bool fromRank = (m_ >> 29) & 1;
		bool check    = (m_ >> 30) & 1;
		bool capture  = (getCaptured() != 0);

		switch (getPiece()) {
		case BISHOP: res += "B"; break;
		case KNIGHT: res += "N"; break;
		case ROOK:   res += "R"; break;
		case QUEEN:  res += "Q"; break;
		case KING:   res += "K"; break;
		default: //PAWN
			if (capture) res += 'a' + (from % 8);
		}
		if (fromFyle) res += 'a' + (from % 8);
		if (fromRank) res += '1' + (from / 8);
		if (capture)  res += "x";
		res += 'a' + (to % 8);
		res += '1' + (to / 8);
		if (isPromo()) {
			switch (getPromo()) {
			case BISHOP: res += "=B"; break;
			case KNIGHT: res += "=N"; break;
			case ROOK:   res += "=R"; break;
			case QUEEN:  res += "=Q"; break;
			}
		}
		if (check) res += "+";
		return res;
	}

	void reset(colorT c, pieceT p, squareT from, squareT to, pieceT promo = 0) {
		m_ = to | (from << 6) | (p << 24) | (c << 27);
		if (promo > 2) m_ |= ((promo -2) << 12) | (1 << 14);
	}
	void resetCastle(colorT c, squareT kingSq, squareT rookSq) {
		//Encoding as king to rook allow undoing of chess360 moves
		m_ = rookSq | (kingSq << 6) | (3 << 14) | (KING << 24) | (c << 27);
	}
	void setCapture(pieceT piece, bool enPassant) {
		m_ |= ((piece & 0x07) << 21);
		if (enPassant) m_ |= (2 << 14);
	}
	void setAmbiguous(const FullMove& move2) {
		if ((m_ & 0x700003F) == (move2.m_ & 0x700003F)) {
			int from = getFrom();
			int from2 = move2.getFrom();
			if ((from % 8) != (from2 % 8)) m_ |= (1 << 28);
			else m_ |= (1 << 29);
		}
	}
	void setCheck() { m_ |= (1 << 30); }
};

}

#endif
