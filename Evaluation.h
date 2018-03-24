#ifndef EVALUATION_H
#define EVALUATION_H

class Position;
class Test;

class Evaluation {
public:
  Evaluation();
  ~Evaluation();
  float EvaluatePosition(const Position *p);
  float GetQueenValue() const { return queenvalue; };
  float GetRookValue() const { return rookvalue; };
  float GetBishopValue() const { return bishopvalue; };
  float GetKnightValue() const { return knightvalue; };
  float GetPawnValue() const { return pawnvalue; };
private:
  friend class Test;
  float queenvalue;
  float rookvalue;
  float bishopvalue;
  float knightvalue;
  float pawnvalue;
};

#endif