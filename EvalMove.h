#ifndef EVALMOVE_H
#define EVALMOVE_H

//Description: If MovesToFinish is not 0, the given move leads to a finish in the given number of moves. If stalemate is set, the finish is a certain stalemate. Otherwise it is a mate. This means that the color executing this move will give stalemate or checkmate in the given number if the given number is not even. If it is even, this color will be stalemated or checkmated in the given number of moves. If no move is given, the color to move is stale- or checkmated.

class Move;

class EvalMove {
public:
  EvalMove(Move* move = 0x0, float evaluation = 0.0, int MovesToFinish = 0, bool stalemate = false, bool owner = true);
  ~EvalMove();
  Move* GetMove() const { return m;};
  void SetMove(Move* mo);
  float GetEvaluation() const { return evaluation;};
  void SetEvaluation(float eval) {evaluation = eval;};
  int GetMovesToFinish () const { return MovesToFinish;};
  void SetMovesToFinish(int moves) {MovesToFinish = moves;};
  bool GetStaleMate() const {return stalemate;};
  void SetStaleMate(bool flag) {stalemate = flag; SetMove(0x0);};
  bool GetOwner() const {return owner;};
  void SetOwner(bool flag) {owner = flag;};
  bool GameOver() const {if (m) {return false;} else {return true;}};
  bool IsBetterOrEqual(EvalMove* em, bool isWhiteMove);
  void TransferEvaluation(EvalMove* em);
  void WriteOutMove() const;
  bool Moveequal(Move* meq) const;
  bool WeSetMate() const {return !GetStaleMate() && (GetMovesToFinish() % 2 == 1);};
  bool OpponentMates() const {return !GetStaleMate() && GetMovesToFinish() > 0 && (GetMovesToFinish() % 2 == 1);};
private:
  friend class Test;
  Move* m;
  float evaluation;
  int MovesToFinish;
  bool stalemate;
  bool owner;
};

#endif