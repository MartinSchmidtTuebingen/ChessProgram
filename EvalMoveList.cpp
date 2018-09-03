#include "EvalMoveList.h"

EvalMoveList::EvalMoveList(EvalMove* evalmove, EvalMoveList* nextevalmove, bool ownerflag) {
  if (evalmove)
    em = evalmove;
  else
    em = new EvalMove();
  next = nextevalmove;
  owner = ownerflag;
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

int EvalMoveList::GetNMoves(int previous) {
  if (next)
    return next->GetNMoves(previous + em ? 1 :0);
  else 
    return previous + em ? 1 : 0;
}