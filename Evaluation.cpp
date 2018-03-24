#include "Evaluation.h"
#include "Position.h"

Evaluation::Evaluation() {
  this->queenvalue = 9.0;
  this->rookvalue = 5.0;
  this->bishopvalue = 3.0;
  this->knightvalue = 3.0;
  this->pawnvalue = 1.0;
}

Evaluation::~Evaluation() {
}