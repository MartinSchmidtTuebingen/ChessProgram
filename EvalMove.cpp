#include <iostream>
using namespace std;

#include "EvalMove.h"
#include "Move.h"

EvalMove::EvalMove(Move* move, float evaluation, int MovesToFinish, GameEnding end, bool owner) {
  m = move;
  this->evaluation = evaluation;
  this->MovesToFinish = MovesToFinish;
  gameend = end;
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
  
  SetEvaluation(em->GetEvaluation());
  SetGameEnd(em->GetGameEnd());
  
  if (em->GetGameEnd() != kNoEnd)
    SetMovesToFinish(em->GetMovesToFinish() + 1);
  
  return;
}

bool EvalMove::IsBetterOrEqual(EvalMove* em, bool isWhiteMove) {
  if (WeSetMate() && (!em->WeSetMate() || GetMovesToFinish() <= em->GetMovesToFinish()))
    return true;  

  if (em->OpponentMates() && (!OpponentMates() || em->GetMovesToFinish() <= GetMovesToFinish()))
    return true;
  
  if (em->WeSetMate() && (!WeSetMate() || em->GetMovesToFinish() <= GetMovesToFinish()))
    return false;  

  if (OpponentMates() && (!em->OpponentMates() || GetMovesToFinish() <= em->GetMovesToFinish()))
    return false;  
    
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