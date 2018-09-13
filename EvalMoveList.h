#ifndef EVALMOVELIST_H
#define EVALMOVELIST_H

#include "EvalMove.h"

class Move;

class EvalMoveList {
public:
  EvalMoveList(EvalMove* evalmove = 0x0, EvalMoveList* nextevalmove = 0x0, bool ownerflag = true);
  ~EvalMoveList();
  Move* GetMove() const {return em->GetMove();};
  void SetMove(Move* m) {em->SetMove(m);};
  float GetEvaluation() const {return em->GetEvaluation();};
  void SetEvaluation(float e) {em->SetEvaluation(e);};
  void SetOwner(bool flag) {owner=flag;};
  bool GetOwner() const {return owner;};
  void AddEvalMove(EvalMove* evalmove);
  void AddMove(Move* m);
  int GetMovesToFinish() const {return em->GetMovesToFinish();};
  void SetMovesToFinish(int moves) {em->SetMovesToFinish(moves);};
  bool GetStaleMate() const {em->GetStaleMate();};
  void SetStaleMate(bool isStaleMate) {em->SetStaleMate(isStaleMate);};
  EvalMoveList* GetNext() const {return next;};
  void SetNext(EvalMoveList* n) {next = n;};
  void Append(EvalMoveList* n) {if (!next) {next = n;} else {next->Append(n);}};
  bool IsEmpty() {return GetNMoves() == 0;};
  float Compare(EvalMove* emv) const {return em->Compare(emv);;};
  bool Moveequal(Move* m) const;
  bool ClearFromMove();
  EvalMove* GetEvalMove() const {return em;};
  int GetNMoves(int previous = 0);
  void WriteOutMoves();
private:
  EvalMove* em;
  EvalMoveList* next;
  bool owner;
};

#endif