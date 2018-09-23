#ifndef EVALMOVELIST_H
#define EVALMOVELIST_H

#include "EvalMove.h"

class Move;

class EvalMoveList {
public:
  EvalMoveList(EvalMove* evalmove = 0x0, EvalMoveList* nextevalmove = 0x0, bool ownerflag = true);
  ~EvalMoveList();
  Move* GetMove() const {if (em) {return em->GetMove();}else {return 0x0;}};
  void SetMove(Move* m) {em->SetMove(m);};
  EvalMove* GetEvalMove() const {return em;};
  void SetEvalMove(EvalMove* nem) {em = nem;};
  float GetEvaluation() const {return em->GetEvaluation();};
  void SetEvaluation(float e) {em->SetEvaluation(e);};
  void SetOwner(bool flag) {owner=flag;};
  bool GetOwner() const {return owner;};
  void AddEvalMove(EvalMove* evalmove);
  void AddMove(Move* m);
  int GetMovesToFinish() const {return em->GetMovesToFinish();};
  void SetMovesToFinish(int moves) {em->SetMovesToFinish(moves);};
  bool GetStaleMate() const {em->GetStaleMate();};
  void SetStaleMate(bool isStaleMate) {if (em) em->SetGameEnd(EvalMove::kStaleMate);};
  EvalMoveList* GetNext() const {return next;};
  void SetNext(EvalMoveList* n) {next = n;};
  void Append(EvalMoveList* n) {if (!next) {next = n;} else {next->Append(n);}};
  bool IsEmpty() {return GetNMoves() == 0;};
  bool IsBetterOrEqual(EvalMove* emv, bool isWhiteMove) const {return em->IsBetterOrEqual(emv, isWhiteMove);};
  bool Moveequal(Move* m) const;
  bool ClearFromMove();
  void OrderMoveList(bool isWhiteMove);
  int GetNMoves(int previous = 0);
  void TransferEvaluation(EvalMove* emcomp) {if (em) em->TransferEvaluation(emcomp);};
  void WriteOutMoves();
private:
  EvalMove* em;
  EvalMoveList* next;
  bool owner;
};

#endif