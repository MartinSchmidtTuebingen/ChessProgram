#include <iostream>
using namespace std;

#include "ChessGame.h"
#include "EvalMove.h"
#include "Evaluation.h"
#include "Chess.h"
#include "EvalMoveList.h"
#include "Position.h"

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
  
  return 0x0;
}

EvalMove* ChessGame::GiveBestMoveWithEvaluation(int depth) {
  EvalMoveList* movelist = GetMoveList();
  if (!movelist)
    return 0x0;
  
  EvalMove* bestmove = new EvalMove();
  if (movelist->IsEmpty()) {
    bestmove->SetStaleMate(!pos->IsChecked(pos->GetColorToMove()));
    //Otherwise the evaluation assumes a mate
  }
//   else {
//     if (depth > 0) {
//       Move* m = 0x0;
//       ReverseMove* rm = 0x0;
//       for (int n=1;n<=movelist->GetNMoves();i++) {
//         m->movelist->GetMoveEntry(n);
//         pos->ExecuteMove(m);
//       }
//       do {
//         m = looper->GetMove();
//         rm = pos->ExecuteMove(m);
//         looper->SetEvaluation(GiveBestMoveWithEvaluation(pos, depth-1)->GetEvaluation());
//         looper->SetMove(m);
//         pos->ReverseMove(rm);
//         bool betterorequal = looper->BetterOrEqual(bestevalmove);
//         int numberOfBestMoves;
//         if (betterorequal > 0) {
//           EvalMove* bestevalmove = looper->GetEvalMove();
//           int numberOfBestMoves = 1;
//         }
//         else if (betterorequal >= 0) {
//           numberOfBestMoves++;
//         }
//         else {
//           looper->ClearFromMove();
//         }
//         looper = looper->GetNext();
//       }
//       while (looper);
//       if (numberOfBestMoves = 1) {
//         evalmove = bestevalmove;
//       }
//       else {
//         int number = GetRandomNumber(numberOfBestMoves);
//         int i = 1;
//         looper = movelist;
//         do {
//       if (!looper->GetEvalMove()) 
//         continue;
//         
//       if (i == number) {
//         evalmove = looper->GetEvalMove();
//         looper->SetOwner(false);
//         break;
//       }
//       i++;
//       looper = looper->GetNext();
//         }
//         while (looper);
//       }
//     }
//     else {
//       if (pos->GetColorToMove() == whiteNumber) {
//         evalmove->SetEvaluation(whiteEvaluation->EvaluatePosition(pos));
//       }
//       if (pos->GetColorToMove() == blackNumber) {
//         evalmove->SetEvaluation(blackEvaluation->EvaluatePosition(pos));
//       }
//     }
//   }
  delete movelist;
  movelist = 0x0;
  return bestmove;
}

void WriteOutEvaluation() {
  
  
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
  
  cout << "Evaluation according to white Evaluation is: " << whiteEvaluation->EvaluatePosition(pos) << endl;
  cout << "Evaluation according to black Evaluation is: " << blackEvaluation->EvaluatePosition(pos) << endl;
}