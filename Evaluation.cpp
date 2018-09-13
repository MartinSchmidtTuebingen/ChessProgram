#include "Evaluation.h"
#include "Position.h"
#include "PieceList.h"

Evaluation::Evaluation() {
  queenvalue = 9.0;
  rookvalue = 5.0;
  bishopvalue = 3.0;
  knightvalue = 3.0;
  pawnvalue = 1.0;
  calcdepth = 4;
}

Evaluation::~Evaluation() {
}

float Evaluation::EvaluatePosition(Position* pos) {
  if (!pos)
    return 0.0;
  
  PieceList* white = pos->GetWhitePieces();
  float whitevalue = white ? white->GetValuePieces() : 0.0;
  
  PieceList* black = pos->GetBlackPieces();
  float blackvalue = black ? black->GetValuePieces() : 0.0;  
  return whitevalue - blackvalue;
}