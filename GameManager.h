#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Position.h"
class EvalMove;
class Move;

class GameManager {
public:
  enum Status {idle, leave, setupposition, positionsetted, analyse, startgame, playgame};
  //Constructors and destructor
  GameManager();
  ~GameManager();
  //Getters and Setters
  Status GetStatus() const {return stat;};
  void SetStatus(Status newstat) {stat = newstat;};
  int GetDepth() const {return depth;};
  void SetDepth(int d) {depth = d;};
  EvalMove* GiveBestMoveWithEvaluation(Position* p, int depth) {return 0x0;};
  void StartGame();
  void RequestInput();
  void ChooseAction();
  void SetUpPosition();
  void SetUpPieces(short color);
  void DeletePieces();
  void ChangeAttributes();
  void PerformAnalysis();
  Move* RequestMove();
  void ChangePosition();
  void ManageGame(int humancolor);
private:
  friend class Test;
  Status stat;
  Position *pos;
  int depth;
};


#endif