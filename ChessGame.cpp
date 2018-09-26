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
  if (!movelist || depth < 0)
    return movelist;
  
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
  EvalMoveList* movelist = GetEvalMoveList(depth);
  
  if (!movelist)
    return 0x0;
  
  if (movelist->IsEmpty()) {
    bestmove->SetGameEnd(!pos->IsChecked(pos->GetColorToMove()) ? EvalMove::kStaleMate : EvalMove::kMate);
    delete movelist;
    return bestmove;
  }
  else if (depth < 0) {
    Evaluation* eval = pos->GetColorToMove()==whiteNumber ? whiteEvaluation : blackEvaluation;
    bestmove->SetEvaluation(eval->EvaluatePosition(pos));
    delete movelist;
    return bestmove;
  }
  
  movelist->OrderMoveList(pos->GetColorToMove());
  bestmove = movelist->GetEvalMove();
  movelist->SetOwner(false);
  delete movelist;

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
  EvalMoveList* movelist = GetMoveList();
  if (movelist->IsEmpty()) 
    cout << (pos->GetColorToMove() == whiteNumber ? "White" : "Black") << " is " << (!pos->IsChecked(pos->GetColorToMove()) ? "stalemate." : "mate.") << endl;
  else
    delete movelist;
  
  cout << "Evaluation according to white Evaluation is: " << whiteEvaluation->EvaluatePosition(pos) << endl;
  cout << "Evaluation according to black Evaluation is: " << blackEvaluation->EvaluatePosition(pos) << endl;
  return;
}