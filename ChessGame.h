#ifndef CHESSGAME_H
#define CHESSGAME_H

class Position;
class EvalMove;
class Evaluation;

class ChessGame {
public:
  ChessGame(Evaluation *whiteEvaluation = 0x0, Evaluation *blackEvaluation = 0x0);
  ~ChessGame();
  EvalMove* GiveBestMoveWithEvaluation(Position* pos, int depth = -1);
  Evaluation* GetWhiteEvaluation() const {return whiteEvaluation;};
  void SetWhiteEvaluation(Evaluation* eval) {whiteEvaluation = eval;};
  Evaluation* GetBlackEvaluation() const {return blackEvaluation;};
  void SetBlackEvaluation(Evaluation* eval) {blackEvaluation = eval;};
  
private:
  Evaluation* whiteEvaluation;
  Evaluation* blackEvaluation;
  
};

#endif