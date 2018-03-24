#include <iostream>
using namespace std;

#include "Move.h"
#include "Piece.h"

Move::Move(short startfile, short startrank, short targetfile, short targetrank, short idcapturedpiece, short promotion) {
  this->startfile = startfile;
  this->startrank = startrank;
  this->targetfile = targetfile;
  this->targetrank = targetrank;
  this->idcapturedpiece = idcapturedpiece;
  if (promotion > king && promotion < pawn) {
    this->promotion = promotion;
  }
  else {
    if (!(promotion == 0)) cout << "Error: Wrong promotion type. Set to no promotion" << endl;
    this->promotion = 0;
  }
}

Move::~Move() {
}

void Move::SetPromotion(short flag) {
  if (flag > king && flag < pawn) {
    promotion = flag;
  }
  else {
    promotion = 0;
  }
}
