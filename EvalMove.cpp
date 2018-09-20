#include <iostream>
using namespace std;

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

void EvalMove::TransferEvaluation(EvalMove* em) {
  if (!em)
    return;
  
  SetEvaluation(-1 * em->GetEvaluation());
  SetStaleMate(em->GetStaleMate());
  if (!em->GetMove())
    SetMovesToFinish(1);
  if (em->GetMovesToFinish() != 0)
    SetMovesToFinish(em->GetMovesToFinish() + 1);
  
  return;
}

bool EvalMove::IsBetterOrEqual(EvalMove* em, bool isWhiteMove) {
  if (WeSetMate() && (!em->WeSetMate() || GetMovesToFinish() <= em->GetMovesToFinish()))
      return true;
  
  if (em->OpponentMates() && (!OpponentMates() || em->GetMovesToFinish() <= GetMovesToFinish()))
    return true;
    
  float evalthis = GetStaleMate() ? 0.0 : GetEvaluation();
  float evalthem = em->GetStaleMate() ? 0.0 : em->GetEvaluation();
  
  return isWhiteMove ? evalthis >= evalthem : evalthis <= evalthem;
}

void EvalMove::WriteOutMove() const {
  if (m) {
    m->WriteOutMove();
    cout << ": ";
    if (WeSetMate())
      cout << "Color to move sets mate in " << GetMovesToFinish() << " moves.";
    else if (OpponentMates())
      cout << "Opponent sets mate in " << GetMovesToFinish() << " moves.";    
    else if (GetStaleMate())
      cout << "Stalemate in " << GetMovesToFinish() << " moves.";
    else 
      cout << "Evaluation of the position after the move is: " << GetEvaluation();
  }
  return;
}

bool EvalMove::Moveequal(Move* meq) const {
  if (m && meq)
    return m->Moveequal(meq);
  else
    return false;
}