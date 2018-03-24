#include "EvalMove.h"
#include "Move.h"

EvalMove::EvalMove(Move* m, float evaluation, int MovesToMate, bool stalemate, bool owner) {
  this->m = m;
  this->evaluation = evaluation;
  this->MovesToMate = MovesToMate;
  this->stalemate = stalemate;
  this->owner = owner;
}

EvalMove::~EvalMove() {
  if (owner) {
    delete m;
  }
  m = 0x0;
}
