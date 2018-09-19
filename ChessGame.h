#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "Evaluation.h"
#include "Position.h"

class EvalMove;
class Move;

class ChessGame {
public:
  ChessGame(Evaluation *we = 0x0, Evaluation *be = 0x0, Position* p = 0x0);
  ~ChessGame();
  Evaluation* GetWhiteEvaluation() const {return whiteEvaluation;};
  void SetWhiteEvaluation(Evaluation* eval) {delete whiteEvaluation; whiteEvaluation = eval;};
  Evaluation* GetBlackEvaluation() const {return blackEvaluation;};
  void SetBlackEvaluation(Evaluation* eval) {delete blackEvaluation; blackEvaluation = eval;};
  Position* GetPosition() const {return pos;};
  void SetPosition(Position* p) {delete pos;pos = p;};  
  EvalMoveList* GetMoveList();
  EvalMoveList* GetEvalMoveList(int depth);
  EvalMove* GiveBestMoveWithEvaluation(int dept);
  void DoMove(Move* m = 0x0);
  void WriteOutPosition();
private:
  Evaluation* whiteEvaluation;
  Evaluation* blackEvaluation;
  Position* pos;
};

#endif