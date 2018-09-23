#ifndef EVALMOVE_H
#define EVALMOVE_H

class Move;

class EvalMove {
public:
  enum GameEnding { kNoEnd = 0, kStaleMate = 1, kMate = 2 };
  
  EvalMove(Move* move = 0x0, float evaluation = 0.0, int MovesToFinish = 0, GameEnding end = kNoEnd, bool owner = true);
  ~EvalMove();
  Move* GetMove() const { return m;};
  void SetMove(Move* mo);
  float GetEvaluation() const { return evaluation;};
  void SetEvaluation(float eval) {evaluation = eval;};
  int GetMovesToFinish () const { return MovesToFinish;};
  void SetMovesToFinish(int moves) {MovesToFinish = moves;};
  bool GetStaleMate() const {return gameend == kStaleMate;};
  bool GetMate() const { return gameend == kMate;};
  void SetGameEnd(GameEnding flag) {gameend = flag;};
  GameEnding GetGameEnd() const {return gameend;};
  bool GetOwner() const {return owner;};
  void SetOwner(bool flag) {owner = flag;};
  bool GameOver() const {if (m) {return false;} else {return true;}};
  bool IsBetterOrEqual(EvalMove* em, bool isWhiteMove);
  void TransferEvaluation(EvalMove* em);
  void WriteOutMove() const;
  bool Moveequal(Move* meq) const;
  bool WeSetMate() const {return GetMate() && (GetMovesToFinish() % 2 == 1);};
  bool OpponentMates() const {return GetMate() && (GetMovesToFinish() % 2 == 0);};
private:
  friend class Test;
  Move* m;
  float evaluation;
  int MovesToFinish;
  GameEnding gameend;
  bool owner;
};

#endif