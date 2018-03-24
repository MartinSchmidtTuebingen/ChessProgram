#ifndef EVALMOVE_H
#define EVALMOVE_H

//Description: If MovesToMate is not 0, the given move leads to mate in the given number of moves. If it is positive, White will mate. If it is negative, Black will mate. If stalemate is true, the colortomove will set stalemate with the given move. If both are not given, evaluation returns the evaluation of the position after the move. 

class Move;

class EvalMove {
public:
  EvalMove(Move* m = 0x0, float evaluation = 0.0, int MovesToMate = 0, bool stalemate = false, bool owner = true);
  ~EvalMove();
  Move* GetMove() const { return m;};
  void SetMove(Move* mo) {m = mo;};
  float GetEvaluation() const { return evaluation;};
  void SetEvaluation(float eval) {evaluation = eval;};
  int GetMovesToMate () const { return MovesToMate;};
  void SetMovesToMate(int moves) {MovesToMate = moves;};
  bool GetStaleMate() const {return stalemate;};
  void SetStaleMate(bool flag) {stalemate = flag;}
  bool GetOwner() const {return owner;};
  void SetOwner(bool flag) {owner = flag;};
  bool GameOver() const {if (m) {return false;} else {return true;}};
  void WriteOut() const {;};
  
private:
  friend class Test;
  Move* m;
  float evaluation;
  int MovesToMate;
  bool stalemate;
  bool owner;
};

#endif