#include <iostream>
using namespace std;

#include "Move.h"
#include "Piece.h"

Move::Move(short startf, short startr, short targetf, short targetr, short id, short prom) {
  startfile = startf;
  startrank = startr;
  targetfile = targetf;
  targetrank = targetr;
  idcapturedpiece = id;
  if (prom > king && prom < pawn) {
    promotion = prom;
  }
  else {
    if (!(prom == 0)) cout << "Error: Wrong promotion type. Set to no promotion" << endl;
    promotion = 0;
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
