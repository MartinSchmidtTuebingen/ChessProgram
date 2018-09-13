#ifndef EVALUATION_H
#define EVALUATION_H

class Position;
class Test;

class Evaluation {
public:
  Evaluation();
  ~Evaluation();
  virtual float EvaluatePosition(Position *pos);
  float GetQueenValue() const { return queenvalue; };
  float GetRookValue() const { return rookvalue; };
  float GetBishopValue() const { return bishopvalue; };
  float GetKnightValue() const { return knightvalue; };
  float GetPawnValue() const { return pawnvalue; };
  int GetCalcDepth() const { return calcdepth; };
  void SetCalcDepth(int depth) {calcdepth = depth; };
private:
  friend class Test;
  float queenvalue;
  float rookvalue;
  float bishopvalue;
  float knightvalue;
  float pawnvalue;
  int calcdepth;
};

#endif