#include <iostream>

#include "EvalMoveList.h"

EvalMoveList::EvalMoveList(EvalMove* evalmove, EvalMoveList* nextevalmove, bool ownerflag) {
  if (evalmove)
    em = evalmove;
  else
    em = 0x0;
  next = nextevalmove;
  owner = ownerflag;
}

EvalMoveList::~EvalMoveList() {
  if (next) {
    delete next;
    next = 0x0;
  }
  if (owner) 
    delete em;
  
  em = 0x0;
}

void EvalMoveList::AddEvalMove(EvalMove* evalmove) {
  if (em) {
    if (next)
      next->AddEvalMove(evalmove);
    else 
      next = new EvalMoveList(evalmove);
  }
  else
    em = evalmove;
}

void EvalMoveList::AddMove(Move* m) {
  EvalMove* evalmove = new EvalMove(m);
  AddEvalMove(evalmove);
}

bool EvalMoveList::ClearFromMove() {
  if (owner) 
    delete em;

  em = 0x0;
}

int EvalMoveList::GetNMoves(int previous) {
  if (next)
    return next->GetNMoves(previous + em ? 1 :0);
  else 
    return previous + em ? 1 : 0;
}

bool EvalMoveList::Moveequal(Move* m) const {
  bool equalmove = false;
  if (em)
    equalmove = em->Moveequal(m);
  if (equalmove)
    return true;
  else {
    if (next)
      return next->Moveequal(m);
  }
}

void EvalMoveList::WriteOutMoves() {
 if (em) {
   em->WriteOutMove();
   std::cout << std::endl;
 }
 
 if (next) 
   next->WriteOutMoves();
 return;
}