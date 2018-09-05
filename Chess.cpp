#include <iostream>
#include <stdlib.h>
using namespace std;

#include "Chess.h"
#include "GameManager.h"
#include "Test.h"

int main() {
  Test *t = new Test();
  cout << "Start testing" << endl;
//   t->TestPieceListFunctions();
  t->TestPositionFunctions();
  delete t;
  t = 0x0;
  cout << "Test finished" << endl;
  GameManager *game = new GameManager();
  
  game->StartGame();	
  delete game;
  game = 0x0;
  
  return 1;
}