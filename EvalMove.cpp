#include "EvalMove.h"
#include "Move.h"

EvalMove::EvalMove(Move* move, float evaluation, int MovesToFinish, bool stalemate, bool owner) {
  if (move)
    m = move;
  else
    m = new Move();
  this->evaluation = evaluation;
  this->MovesToFinish = MovesToFinish;
  this->stalemate = stalemate;
  this->owner = owner;
}

EvalMove::~EvalMove() {
  if (owner) {
    delete m;
  }
  m = 0x0;
}
