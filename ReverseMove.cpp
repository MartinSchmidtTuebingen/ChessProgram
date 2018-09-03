#include "ReverseMove.h"

ReverseMove::ReverseMove(short startf, short startr, short targetf, short targetr, short id, short type, bool prom, bool castlong, bool castshort, short ep) {
  startfile = startf;
  startrank = startr;
  targetfile = targetf;
  targetrank = targetr;
  idcapturedpiece = id;
  typecapturedpiece = type;
  promotion = prom;
  castlinglong = castlong;
  castlingshort = castshort;
  enpassantfile = 0;
  if (ep >= 1 && ep <= 8)
    enpassantfile = ep;
}

ReverseMove::~ReverseMove() {
}

void ReverseMove::SetFields(short startf, short startr, short targetf, short targetr) {
  startfile = startf;
  startrank = startr;
  targetfile = targetf;
  targetrank = targetr;
}

