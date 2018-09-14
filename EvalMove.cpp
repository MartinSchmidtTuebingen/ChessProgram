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

void EvalMove::SetMove(Move* mo) {
  if (GetOwner()) 
    delete m;
  
  m=mo; 
}

void EvalMove::TransferEvaluationofNextBestEvalMove(EvalMove* m) {
  
  
}

void EvalMove::WriteOutMove() const {
  if (m)
    m->WriteOutMove();
  return;
}

bool EvalMove::Moveequal(Move* meq) const {
  if (m && meq)
    return m->Moveequal(meq);
  else
    return false;
}