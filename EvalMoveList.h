#ifndef EVALMOVELIST_H
#define EVALMOVELIST_H

#include "EvalMove.h"

class EvalMoveList {
public:
  EvalMoveList(EvalMove* em = 0x0, EvalMoveList* next = 0x0, bool owner = true);
  ~EvalMoveList();
  Move* GetMove() const {return em->GetMove();};
  void SetMove(Move* m) {em->SetMove(m);};
  float GetEvaluation() const {return em->GetEvaluation();};
  void SetEvaluation(float e) {em->SetEvaluation(e);};
  int GetMovesToMate() const {return em->GetMovesToMate();};
  void SetMovesToMate(int moves) {em->SetMovesToMate(moves);};
  bool GetStaleMate() const {em->GetStaleMate();};
  void SetStaleMate(bool isStaleMate) {em->SetStaleMate(isStaleMate);};
  EvalMoveList* GetNext() const {return next;};
  void SetNext(EvalMoveList* n) {next = n;};
  void Append(EvalMoveList* n) {if (!next) {next = n;} else {next->Append(n);}};
  bool IsEmpty() {return true;};
  bool BetterOrEqual(EvalMove* em) const {return false;};
  bool ClearFromMove();
  EvalMove* GetEvalMove() const {return em;};
private:
  EvalMove* em;
  EvalMoveList* next;
  bool owner;
};

#endif