#include "EvalMoveList.h"

EvalMoveList::EvalMoveList(EvalMove* em, EvalMoveList* next, bool owner) {
  this->em = em;
  this->next = next;
  this->owner = owner;
}

EvalMoveList::~EvalMoveList() {
  if (next) {
    delete next;
    next = 0x0;
  }
  if (owner) {
    delete em;
  }
  em = 0x0;
}

bool EvalMoveList::ClearFromMove() {
  if (owner) {
    delete em;
  }
  em = 0x0;
}

int EvalMoveList::int GetNMoves(int previous) {
  if (next)
    return next->GetNMoves(previous + em ? 1 :0);
  else 
    return previous + em ? 1 : 0;
}