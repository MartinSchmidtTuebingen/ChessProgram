#include "ChessGame.h"
#include "EvalMove.h"
#include "Evaluation.h"
#include "Chess.h"
#include "EvalMoveList.h"
#include "Position.h"

ChessGame::ChessGame(Evaluation* whiteEvaluation, Evaluation* blackEvaluation) {
  this->whiteEvaluation = whiteEvaluation;
  this->blackEvaluation = blackEvaluation;
}

ChessGame::~ChessGame() {
  whiteEvaluation = 0x0;
  blackEvaluation = 0x0;
}

EvalMove* ChessGame::GiveBestMoveWithEvaluation(Position* pos, int depth) {
  EvalMoveList* movelist = pos->MakeMoveList();
  EvalMove* evalmove = new EvalMove();
  if (movelist->IsEmpty()) {
    if (!pos->IsChecked(pos->GetColorToMove()))
      evalmove->SetStaleMate(true);
    //Otherwise the evaluation assumes a mate
  }
  else {
    if (depth > 0) {
      Move* m = 0x0;
      ReverseMove* rm = 0x0;
      for (int n=1;n<=movelist->GetNMoves();i++) {
        m->movelist->GetMoveEntry(n);
        rm = pos->ExecuteMove(m);
      }
      do {
        m = looper->GetMove();
        rm = pos->ExecuteMove(m);
        looper->SetEvaluation(GiveBestMoveWithEvaluation(pos, depth-1)->GetEvaluation());
        looper->SetMove(m);
        pos->ReverseMove(rm);
        bool betterorequal = looper->BetterOrEqual(bestevalmove);
        int numberOfBestMoves;
        if (betterorequal > 0) {
          EvalMove* bestevalmove = looper->GetEvalMove();
          int numberOfBestMoves = 1;
        }
        else if (betterorequal >= 0) {
          numberOfBestMoves++;
        }
        else {
          looper->ClearFromMove();
        }
        looper = looper->GetNext();
      }
      while (looper);
      if (numberOfBestMoves = 1) {
        evalmove = bestevalmove;
      }
      else {
        int number = GetRandomNumber(numberOfBestMoves);
        int i = 1;
        looper = movelist;
        do {
      if (!looper->GetEvalMove()) 
        continue;
        
      if (i == number) {
        evalmove = looper->GetEvalMove();
        looper->SetOwner(false);
        break;
      }
      i++;
      looper = looper->GetNext();
        }
        while (looper);
      }
    }
    else {
      if (pos->GetColorToMove() == whiteNumber) {
        evalmove->SetEvaluation(whiteEvaluation->GetEvaluation(pos));
      }
      if (pos->GetColorToMove() == blackNumber) {
        evalmove->SetEvaluation(blackEvaluation->GetEvaluation(pos));
      }
    }
  }
  delete movelist;
  movelist = 0x0;
  return evalmove;
}