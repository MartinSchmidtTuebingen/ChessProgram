#include <iostream>
using namespace std;

#include "ChessGame.h"
#include "EvalMove.h"
#include "Evaluation.h"
#include "Chess.h"
#include "EvalMoveList.h"
#include "ReverseMove.h"
#include "Position.h"
#include "Move.h"
#include "SupportFunctions.h"

ChessGame::ChessGame(Evaluation* we, Evaluation* be, Position* p) {
  if (!we)
    we = new Evaluation();
  if (!be)
    be = new Evaluation();
  if (!p) {
    p = new Position();
    p->SetUpStartPosition();
  }
   
  whiteEvaluation = we;
  blackEvaluation = be;
  pos = p;
}

ChessGame::~ChessGame() {
  delete whiteEvaluation;
  delete blackEvaluation;
  delete pos;
}

EvalMoveList* ChessGame::GetMoveList() {
  if (pos)
    return pos->MakeMoveList();
  else
    return 0x0;
}

EvalMoveList* ChessGame::GetEvalMoveList(int depth) {
  EvalMoveList* movelist = GetMoveList();
  if (!movelist)
    return 0x0;
  
  EvalMoveList* looper = movelist;
  do {
    Move* m = looper->GetMove();
    if (m) {
      
      ReverseMove* rm = new ReverseMove();
      pos->ExecuteMove(m,rm);
      int mem_old = getCurrentMemoryUbuntu();
      EvalMove* em = GiveBestMoveWithEvaluation(depth - 1);
      int mem = getCurrentMemoryUbuntu();
      looper->TransferEvaluation(em);
      
      delete em;
      pos->RetractMove(rm);
      delete rm;
    }
    looper = looper->GetNext();
  }
  while (looper);
  
  return movelist;
}

EvalMove* ChessGame::GiveBestMoveWithEvaluation(int depth) {
  EvalMove* bestmove = new EvalMove();
  if (depth <= 0) {
    bestmove->SetGameEnd(pos->GameEnd());
    if (bestmove->GetGameEnd() == EvalMove::kNoEnd) {
      Evaluation* eval = pos->GetColorToMove()==whiteNumber ? whiteEvaluation : blackEvaluation;
      bestmove->SetEvaluation(eval->EvaluatePosition(pos));
    }
  }
  else {
    EvalMoveList* movelist = GetEvalMoveList(depth);
  
    if (!movelist)
      return 0x0;
 
    movelist->OrderMoveList(pos->GetColorToMove());
    bestmove = movelist->GetEvalMove();
    movelist->SetOwner(false);
    delete movelist;
  }
  return bestmove;
}

void ChessGame::DoMove(Move* m) {
  if (m) {
    if (pos->IsMoveLegal(m))
      pos->ExecuteMove(m);
    else
      cout << "Move is not legal" << endl;
  }
  else {
    Evaluation* ev = pos->GetColorToMove() == whiteNumber ? whiteEvaluation : blackEvaluation;
    EvalMove* bestmove = GiveBestMoveWithEvaluation(ev->GetCalcDepth());
    m = bestmove->GetMove();
    if (m) 
      pos->ExecuteMove(m);
    else 
      cout << (pos->GetColorToMove() == whiteNumber ? "White" : "Black") << " is " << (bestmove->GetStaleMate() ? "stalemate." : "mate.") << endl;
  }
  delete m;
}

void ChessGame::WriteOutPosition() {
  if (!pos) {
    cout << "No Position given" << endl;
    return;
  }
  pos->WriteOutPosition();
  EvalMove::GameEnding end = pos->GameEnd();
  if (end != EvalMove::kNoEnd) 
    cout << (pos->GetColorToMove() == whiteNumber ? "White" : "Black") << " is " << (end == EvalMove::kStaleMate) ? "stalemate." : "mate.") << endl;
  else {
    cout << "Evaluation according to white Evaluation is: " << whiteEvaluation->EvaluatePosition(pos) << endl;
    cout << "Evaluation according to black Evaluation is: " << blackEvaluation->EvaluatePosition(pos) << endl;
  }
  return;
}