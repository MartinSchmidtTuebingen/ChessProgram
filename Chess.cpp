#include <iostream>
#include <stdlib.h>
using namespace std;

#include "Chess.h"
#include "GameManager.h"
#include "Test.h"

//TODO Write the classes and functions needed for the class ChessGame
/* Test function Position::IsMoveLegal()
 * Test ReverseMove including all difficulties (castling, enpassant, promotion)
 * Close functionality for Piece::MakeEvalMoveList
 * Compare to EvalMoves, function gives back 0.0 at the moment
 */

// (See papers)

//TODO Write class evaluation. This class is doing later the evaluation of the positions. As basis, it only evaluates the material. Later possibilities can be included using suggestions of blog readers. The function evaluation also evaluates possible mates for the side who moves and similar things. 

//NOTE Except being in the deepest branch, there is no need to call "IsChecked" Everytime. Just make a new move tree - if one of the moves includes capturing the king, the complete predecessor move can be removed. However, this is probably not a good idea from the point that the program should be kept simple.

//NOTE The different evaluations will be implemented as classes which inherit from the base class Evaluation. All have only one public element: The function "float EvaluatePosition(const Position *p)"

//NOTE The Evaluation of the position has to go first down the complete branch, then go up again. Otherwise, too much memory is needed.

//TODO Clean Up Program: Copy-Constructors, making appropriate functions private, clean up, comment


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