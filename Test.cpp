#include <iostream>


#include "sys/types.h"
#include "sys/sysinfo.h"
using namespace std;

#include "Test.h"
#include "Evaluation.h"
#include "Move.h"
#include "EvalMoveList.h"
#include "Piece.h"
#include "PieceList.h"
#include "Position.h"
#include "Chess.h"
#include "ChessGame.h"
#include "ReverseMoveStack.h"
#include "ReverseMove.h"
#include "SupportFunctions.h"

Test::Test() {
}

Test::~Test() {
}

void Test::TestPieceFunctions() {
  cout << endl << "TEST: Test Piece Functions" << endl << endl;
  cout << "Not yet implemented. Try TestPieceListFunctions()" << endl;
}

void Test::TestPieceListFunctions() {
cout << endl << "TEST: Test PieceList Functions" << endl << endl;
  PieceList *WhitePieces = new PieceList();
  PieceList *BlackPieces = new PieceList();
  
  cout << "Test PieceList: Creating Pieces" << endl;
  Piece *whitepawn = new Piece(6,1,3,7);
  cout << "Writing out single Piece: ";
  whitepawn->WriteOutPiece();
  cout << " Correct: Pawn on c7." << endl;
  WhitePieces->CreatePiece(whitepawn);
  WhitePieces->CreatePiece(1,1,8,1);
  WhitePieces->CreatePiece(4,1,6,6);
  cout << "Test Writing Pieces out" << endl;
  WhitePieces->WriteOutPieces();
  cout << "Should be: (order not important)" << endl;
  cout << "Bishop on f6." << endl << "Pawn on c7." << endl << "King on h1." << endl;
  Piece *blackrook = new Piece(3,-1,4,8);
  BlackPieces->CreatePiece(blackrook);
  BlackPieces->CreatePiece(2,-1,7,8);
  BlackPieces->CreatePiece(6,-1,6,7);
  BlackPieces->CreatePiece(6,-1,8,7);
  BlackPieces->WriteOutPieces();
  
  cout << "Values of White Pieces: " << WhitePieces->GetValuePieces() << " Correct Value: 4" << endl;
  cout << "Values of Black Pieces: " << BlackPieces->GetValuePieces() << " Correct Value: 16" << endl;
  cout << "Type of white Piece on Field d8? " << WhitePieces->IsAnyPieceOnField(4,8) << " Correct Value: 0" << endl;
  cout << "Type of black Piece on Field d8? " << BlackPieces->IsAnyPieceOnField(4,8) << " Correct Value: 3" << endl;
  cout << "Get the position of the king. " << (WhitePieces->GetKing())->GetFile() << " " << (WhitePieces->GetKing())->GetRank() << " Correct values: 8 1" << endl;
  cout << "Get type and color of the black piece on field f7: " << (BlackPieces->GetPieceOnField(6,7))->GetType() << " " << (BlackPieces->GetPieceOnField(6,7))->GetColor() << " Correct Values: 6 -1" << endl;
  cout << "Get Pointer to black piece on field a1: " << BlackPieces->GetPieceOnField(1,1) << " Correct value: 0" << endl;
  
  cout << "Test PieceList: Moving Pieces" << endl;
  Move *m = new Move(3,7,4,8, blackrook->GetID(), 3);
  
  WhitePieces->MovePiece(m);
  BlackPieces->DeletePiece(m);
  
  cout << "Moving the white pawn on c7 to d8, capturing the black rook there and promoting to a rook." << endl;

  cout << "Writing out white pieces after white moved." << endl;
  WhitePieces->WriteOutPieces();
  cout << "Should be: (order not important)" << endl;
  cout << "White Bishop on f6." << endl << "White Rook on d8." << endl << "White King on h1." << endl; 
  cout << "Writing out black pieces after white moved." << endl;
  BlackPieces->WriteOutPieces();
  cout << "Should be: (order not important)" << endl;
  cout << "Black Queen on g8." << endl << "Black Pawn on f7" << endl << "Black Pawn on h7" << endl;
  
  cout << endl << "Clear the white piece list" << endl;
  WhitePieces->ClearList();
  WhitePieces->WriteOutPieces();
  cout << "Should be empty" << endl;
  delete WhitePieces;
  WhitePieces = 0x0;
  BlackPieces->SetOwner(true);
  BlackPieces->ClearList();
  delete BlackPieces;
  BlackPieces = 0x0;
  
  cout << "Should delete contained pieces too. Return pointer of the created rook: " << blackrook << endl;
  
  cout << endl << "TEST of PieceList Functions finished" << endl << endl;  
}

void Test::TestPositionFunctions() {
 cout << endl << "TEST: Test Position Functions" << endl << endl;
  cout << "Test Position: Constructing the position, checking the position, Creating new pieces, moving and capturing (deleting) pieces" << endl;
  Position *pos = new Position();
  pos->CreatePiece(1,1,1,2);
  pos->CreatePiece(2,1,4,1);
  pos->CreatePiece(1,-1,5,5);
  pos->CreatePiece(3,-1,3,3);
  pos->CreatePiece(4,-1,4,4);
  pos->WriteOutPosition();
  cout << endl << "Correct would be (without ordering):" << endl;
  cout << "It is whites turn." << endl;
  cout << "White Pieces:" << endl;
  cout << "White King on a2." << endl;
  cout << "White Queen on d1." << endl;
  cout << "Black Pieces:" << endl;
  cout << "Black King on e5." << endl;
  cout << "Black Rook on c3." << endl;
  cout << "Black Bishop on d4." << endl << endl;
  cout << "ANY Mentioning of castling or en passant is wrong!" << endl;
  
  cout << endl << "Moving the white queen from d1 to b3" << endl;
  Move *m = new Move(4,1,2,3);
  pos->ExecuteMove(m);
  pos->WriteOutPosition();
  delete m;
  m = 0x0;
  cout << endl << "Capturing the white queen on b3 with the black rook on c3" << endl;
  m = new Move(3,3,2,3);
  pos->ExecuteMove(m);
  pos->WriteOutPosition();
  delete m;
  m = 0x0;
  cout << endl << "Capturing the black rook on b3 with the white king on a2" << endl;
  m = new Move(1,2,2,3);
  pos->ExecuteMove(m);
  pos->WriteOutPosition();
  
  delete m;
  m = 0x0;
  delete pos;
  pos = 0x0;
  
  cout << endl << "Test Position: Test castling" << endl;
  pos = new Position(0x0,0x0,1,true,true,true,true);
  pos->CreatePiece(1,1,5,1);
  pos->CreatePiece(3,1,1,1);
  pos->CreatePiece(1,-1,5,8);
  pos->CreatePiece(3,-1,8,8);
  pos->WriteOutPosition();
  cout << endl << "White castles long and black castles short" << endl;
  Move *whitelongcastling = new Move(5,1,3,1);
  Move *blackshortcastling = new Move(5,8,7,8);
  pos->ExecuteMove(whitelongcastling);
  pos->WriteOutPosition();
  pos->ExecuteMove(blackshortcastling);
  pos->WriteOutPosition();
  delete pos;
  pos = 0x0;
  delete whitelongcastling;
  whitelongcastling = 0x0;
  delete blackshortcastling;
  blackshortcastling = 0x0;
  cout << endl << "Test Position: Test disallowing of castling if moving the rooks" << endl;
  pos = new Position(0x0,0x0,1,true,true,true,true);
  pos->CreatePiece(1,1,5,1);
  pos->CreatePiece(3,1,1,1);
  pos->CreatePiece(1,-1,5,8);
  pos->CreatePiece(3,-1,8,8);
  pos->CreatePiece(3,1,8,1);
  pos->CreatePiece(3,-1,1,8);
  pos->WriteOutPosition();
  cout << endl << "Ta1-b1" << endl;
  Move *rookmove = new Move(1,1,2,1);
  pos->ExecuteMove(rookmove);
  delete rookmove;
  rookmove = 0x0;
  pos->WriteOutPosition();
  cout << endl << "Ta8-b8" << endl;
  rookmove = new Move(1,8,2,8);
  pos->ExecuteMove(rookmove);
  delete rookmove;
  rookmove = 0x0;
  pos->WriteOutPosition();
  cout << endl << "Th1-h2" << endl;
  rookmove = new Move(8,1,8,2);
  pos->ExecuteMove(rookmove);
  delete rookmove;
  rookmove = 0x0;
  pos->WriteOutPosition();
  cout << endl << "Th8xh2" << endl;
  rookmove = new Move(8,8,8,2);
  pos->ExecuteMove(rookmove);
  delete rookmove;
  rookmove = 0x0;
  pos->WriteOutPosition();
  
  cout << endl << "Test Position: End Test castling" << endl;
  delete pos;
  pos = 0x0;
  cout << endl << "Test Position: Test en passant" << endl;  
  pos = new Position();
  pos->CreatePiece(6,1,2,2);
  pos->CreatePiece(6,-1,3,4);
  pos->CreatePiece(6,1,6,4);
  pos->CreatePiece(6,-1,7,7);
  pos->WriteOutPosition();
  cout << endl << "b2-b4" << endl;
  Move *pawnmove = new Move(2,2,2,4);
  pos->ExecuteMove(pawnmove);
  delete pawnmove;
  pawnmove = 0x0;
  pos->WriteOutPosition();
  
  cout << endl << "c4xb3 e.p." << endl;
  Move *enpassant = new Move(3,4,2,3);
  pos->ExecuteMove(enpassant);
  pos->WriteOutPosition();
  delete enpassant;
  enpassant = 0x0;  
  
  cout << endl << "f4-f5" << endl;
  pawnmove = new Move(6,4,6,5);
  pos->ExecuteMove(pawnmove);
  pos->WriteOutPosition();
  delete pawnmove;
  pawnmove = 0x0;
  
  cout << endl << "g7-g5" << endl;
  pawnmove = new Move(7,7,7,5);
  pos->ExecuteMove(pawnmove);
  pos->WriteOutPosition();
  delete pawnmove;
  pawnmove = 0x0;
  
  cout << endl << "f5xg6 e.p." << endl;
  enpassant = new Move(6,5,7,6);
  pos->ExecuteMove(enpassant);
  pos->WriteOutPosition();
  delete enpassant;
  enpassant = 0x0;
  delete pos;
  pos = 0x0;
  
  cout << "Test Position: End Test en passant" << endl << endl;
  
  cout << "Test Position: Begin Test Check" << endl << endl;
  
  cout << "Creating Position with white to move." << endl;
  
  pos = new Position(0x0,0x0,-1);
  
  cout << endl << "Setting up white king on d4 and black rook on d8" << endl;
  pos->CreatePiece(1,1,4,4);
  pos->CreatePiece(3,-1,4,8);
  cout << "Test for Check: " << pos->IsChecked() << " Correct answer: 1" << endl;
 
  cout << endl << "Setting a white rook on d5" << endl;
  pos->CreatePiece(3,1,4,5);
  cout << "Test for Check: " << pos->IsChecked() << " Correct answer: 0" << endl;
  
  cout << endl << "Setting black bishop on f6" << endl;
  pos->CreatePiece(4,-1,6,6);
  cout << "Test for Check: " << pos->IsChecked() << " Correct answer: 1" << endl;

  cout << endl << "Setting a black rook on e5" << endl;
  pos->CreatePiece(3,-1,5,5);
  cout << "Test for Check: " << pos->IsChecked() << " Correct answer: 0" << endl;

  cout << endl << "Setting a black knight on b3" << endl;
  pos->CreatePiece(5,-1,2,3);
  cout << "Test for Check: " << pos->IsChecked() << " Correct answer: 1" << endl;
  
  delete pos;
  pos = 0x0;
  
  cout << endl << "Creating Position with black king on e5 and white Queen on h5" << endl;
  pos = new Position();
  pos->CreatePiece(1,-1,5,5);
  pos->CreatePiece(2,1,8,5);
  cout << "Test for Check: " << pos->IsChecked() << " Correct answer: 1" << endl;
  delete pos;
  pos = 0x0;
  
  cout << endl << "Creating Position with black king on e5 and white pawn on f4" << endl;
  pos = new Position();
  pos->CreatePiece(1,-1,5,5);
  pos->CreatePiece(6,1,6,4);
  cout << "Test for Check: " << pos->IsChecked() << " Correct answer: 1" << endl;
  delete pos;
  pos = 0x0;

  cout << endl << "Creating Position with white king on e5 and black pawn on d6" << endl;
  pos = new Position(0x0,0x0,-1);
  pos->CreatePiece(1,1,5,5);
  pos->CreatePiece(6,-1,4,6);
  cout << "Test for Check: " << pos->IsChecked() << " Correct answer: 1" << endl;
  delete pos;
  pos = 0x0;
  
  cout << endl << "Finished Check test" << endl << endl;
  cout << endl << "Test Position: Begin Test Reversed Move" << endl << endl;
  
  pos = new Position(0x0,0x0,1,true,true,true,true,0);
  pos->CreatePiece(king,whiteNumber,5,1);
  pos->CreatePiece(rook,whiteNumber,1,1);
  pos->CreatePiece(bishop,whiteNumber,4,5);
  pos->CreatePiece(king,blackNumber,5,8);
  pos->CreatePiece(rook,blackNumber,8,8);
  pos->WriteOutPosition();
  
  m = new Move(5,1,3,1);
  ReverseMove* rm = new ReverseMove();
  cout << "Castling long and reversing the move" << endl;
  pos->ExecuteMove(m,rm);
  pos->RetractMove(rm);
  pos->WriteOutPosition();
  delete m;
  m = 0x0;
  delete rm;
  rm = 0x0;
  delete pos;
  pos = 0x0;
  
  cout << endl << "Testing all aspects of retracting a move" << endl;
  pos = new Position(0x0,0x0,whiteNumber,false,false,false,false);
  pos->CreatePiece(pawn,whiteNumber,2,2);
  pos->CreatePiece(bishop,whiteNumber,4,5);
  pos->CreatePiece(pawn,whiteNumber,8,7);
  
  pos->CreatePiece(knight,blackNumber,8,6);
  pos->CreatePiece(pawn,blackNumber,3,4);
  pos->CreatePiece(rook,blackNumber,1,7);
//   pos->WriteOutPosition();
  for (int i=1;i<=8;++i) {
    for (int j=1;j<=8;++j) {
      if (pos->GetPieceOnField(i,j)) {
        pos->GetPieceOnField(i,j)->WriteOutPiece();
        cout << endl;
      }
    }
  }
  ReverseMoveStack* rmstack = new ReverseMoveStack();

  EvalMoveList* eml = new EvalMoveList();
  eml->AddMove(new Move(8,7,8,8,0,rook));
  eml->AddMove(new Move(8,6,6,7));
  eml->AddMove(new Move(4,5,6,7));
  eml->AddMove(new Move(1,7,6,7));
  eml->AddMove(new Move(2,2,2,4));
  eml->AddMove(new Move(3,4,2,3));
  
  EvalMoveList* active = eml;
  while (active) {
    rm = new ReverseMove();
    pos->ExecuteMove(active->GetMove(),rm);
//     pos->WriteOutPosition();
    rmstack->AddReverseMove(rm);
    active = active->GetNext();
  }
  delete eml;
  eml = 0x0;
  
//   pos->WriteOutPosition();
  
  rm = rmstack->GetReverseMove();
  while (rm) {
    pos->RetractMove(rm);
    rm = rmstack->GetReverseMove();
//     pos->WriteOutPosition();
  }
  for (int i=1;i<=8;++i) {
    for (int j=1;j<=8;++j) {
      if (pos->GetPieceOnField(i,j)) {
        pos->GetPieceOnField(i,j)->WriteOutPiece();
        cout << endl;
      }
    }
  }
//   pos->WriteOutPosition();

  delete rmstack;
  rmstack = 0x0;
  delete pos;
  pos = 0x0;  
 
  cout << endl << "Test Position: End Test Reversed Move" << endl << endl;
  
  cout << endl << "Test Piece: Start Test MakeMoveTree" << endl << endl;
  pos = new Position();
  
  pos->CreatePiece(king,whiteNumber,3,3);
  m = pos->GetPieceOnField(3,3)->CreateMoveIfPieceCanMoveToField(3,4,pos);
  cout << m;
  if (m)
    cout << " " << m->GetStartFile() << " " << m->GetStartRank() << " " << m->GetTargetFile() << " " << m->GetTargetRank() << " " << m->GetIDofCapturedPiece();
  cout << endl;
    
  delete m;
  pos->CreatePiece(knight,blackNumber,3,4);
  m = pos->GetPieceOnField(3,3)->CreateMoveIfPieceCanMoveToField(3,4,pos);
  cout << m;
  if (m)
    cout << " " << m->GetStartFile() << " " << m->GetStartRank() << " " << m->GetTargetFile() << " " << m->GetTargetRank() << " " << m->GetIDofCapturedPiece();
  cout << endl;
  
  delete m;
  pos->CreatePiece(pawn,whiteNumber,3,2);
  m = pos->GetPieceOnField(3,3)->CreateMoveIfPieceCanMoveToField(3,2,pos);
  cout << m;
  if (m)
    cout << " " << m->GetStartFile() << " " << m->GetStartRank() << " " << m->GetTargetFile() << " " << m->GetTargetRank() << " " << m->GetIDofCapturedPiece();
  cout << endl;
  
  delete m;
  pos->CreatePiece(pawn,blackNumber,6,2);
  m = pos->GetPieceOnField(6,2)->CreateMoveIfPieceCanMoveToField(6,1,pos);
  cout << m;
  if (m)
    cout << " " << m->GetStartFile() << " " << m->GetStartRank() << " " << m->GetTargetFile() << " " << m->GetTargetRank() << " " << m->GetIDofCapturedPiece();
  cout << endl;
  
  delete m;
  m = pos->GetPieceOnField(6,2)->CreateMoveIfPieceCanMoveToField(7,1,pos);
  cout << m;
  if (m)
    cout << " " << m->GetStartFile() << " " << m->GetStartRank() << " " << m->GetTargetFile() << " " << m->GetTargetRank() << " " << m->GetIDofCapturedPiece();
  cout << endl;
    
  delete m;
  pos->CreatePiece(bishop,whiteNumber,5,1);
  m = pos->GetPieceOnField(6,2)->CreateMoveIfPieceCanMoveToField(5,1,pos);
  cout << m;
  if (m)
    cout << " " << m->GetStartFile() << " " << m->GetStartRank() << " " << m->GetTargetFile() << " " << m->GetTargetRank() << " " << m->GetIDofCapturedPiece();
  cout << endl;
   
  delete m; 
  pos->CreatePiece(knight,blackNumber,8,6);
  pos->CreatePiece(pawn,blackNumber,7,5);
  pos->CreatePiece(pawn,whiteNumber,8,5);
  pos->SetEnPassantFile(7);
  m = pos->GetPieceOnField(8,5)->CreateMoveIfPieceCanMoveToField(7,6,pos);
  cout << m;
  if (m)
    cout << " " << m->GetStartFile() << " " << m->GetStartRank() << " " << m->GetTargetFile() << " " << m->GetTargetRank() << " " << m->GetIDofCapturedPiece();
  cout << endl;
  
  pos->CreatePiece(king,blackNumber,5,8);
  pos->CreatePiece(rook,blackNumber,1,8);
  pos->SetColorCastle(blackNumber,false,true);
  pos->WriteOutPosition();
  
//   for (int i=1;i<=8;++i) {
//     for (int j=1;j<=8;++j) {
//       Piece* p = pos->GetPieceOnField(i,j);
//       if (p) {
//         cout << endl;
//         p->WriteOutPiece();
//         cout << endl;
//         eml = p->MakeMoveList(pos);
//         eml->WriteOutMoves();
//         cout << endl;
//         delete eml;
//       }
//     }
//   }  
  
  eml = pos->MakeMoveList();
  eml->WriteOutMoves();
  cout << endl;
  delete eml;
  eml = 0x0;
  
//   delete pos;
//   pos = new Position();
//   pos->CreatePiece(knight,whiteNumber,2,1);
//   eml = pos->MakeMoveList();
//   eml->WriteOutMoves();
//   delete eml;

  pos->SetUpStartPosition();
  pos->WriteOutPosition();
  eml = pos->MakeMoveList();
  eml->WriteOutMoves();
  
  m = new Move(5,2,5,4);
  pos->ExecuteMove(m);
  delete eml;
  eml = pos->MakeMoveList();
  cout << endl;
  eml->WriteOutMoves();
  delete m;
  m = new Move(4,7,4,5);
  pos->ExecuteMove(m);
  delete eml;
  eml = pos->MakeMoveList();
  cout << endl;
  eml->WriteOutMoves();
  delete eml;
  delete m;
  m = new Move(6,1,2,5);
  pos->ExecuteMove(m);
  eml = pos->MakeMoveList(),
  cout << endl;
  eml->WriteOutMoves();
  delete m;
  m = new Move(8,7,8,6);
  cout << pos->IsMoveLegal(m) << endl;
  
  
  delete eml;
  eml=0x0;
  delete pos;
  pos = 0x0;
  delete m;
  m = 0x0;
  cout << endl << endl << "End Test MakeMoveList" << endl << endl;
  cout << endl << endl << "End Test Position functions" << endl << endl;
  return;
}

void Test::TestChessGameFunctions() {
  cout << endl << "Test Game functions" << endl << endl;
  cout << endl << "Check StaleMate" << endl;
  Position* p = new Position();
  p->SetColorToMove(blackNumber);
  p->CreatePiece(king,blackNumber,1,1);
  p->CreatePiece(queen,whiteNumber,2,3);
  ChessGame* game = new ChessGame(0x0,0x0,p);
  game->DoMove();
  delete game;
  cout << "End check stalemate" << endl;
  
  cout << endl << "Check checkmate" << endl;
  p = new Position();
  p->SetColorToMove(blackNumber);
  p->CreatePiece(king,blackNumber,1,1);
  p->CreatePiece(queen,whiteNumber,2,2);
  p->CreatePiece(king,whiteNumber,3,3);
  game = new ChessGame(0x0,0x0,p);
  game->DoMove();
  cout << "End check checkmate" << endl;  
  cout << "Check complete position evaluation" << endl;
  p->CreatePiece(knight,blackNumber,4,1);
  game->WriteOutPosition();
  cout << "End Check complete position evaluation" << endl;
  
  delete game;
  
  p = new Position();
  p->SetColorToMove(whiteNumber);
  p->CreatePiece(king,whiteNumber,3,2);
  p->CreatePiece(king,blackNumber,1,5);
  p->CreatePiece(queen,whiteNumber,3,6);
  p->WriteOutPosition();
  game = new ChessGame(0x0,0x0,p);
  cout << "Läuft " << getCurrentMemoryUbuntu() << endl;
  EvalMoveList* eml = game->GetEvalMoveList(2);
  cout << getCurrentMemoryUbuntu() << endl;
  eml->OrderMoveList(p->GetColorToMove());
//   eml->WriteOutMoves();
  cout << endl;
//   EvalMove* em = game->GiveBestMoveWithEvaluation(5);
//   em->WriteOutMove();
//   cout << game->GetWhiteEvaluation()->EvaluatePosition(p) << endl;
//   EvalMove* em = game->GiveBestMoveWithEvaluation(1);
//   cout << em->GetGameEnd() << endl;
//   game->DoMove(em->GetMove());
//   p->WriteOutPosition();
//   cout << game->GetWhiteEvaluation()->EvaluatePosition(p) << endl;
//   cout << endl;
//   em->WriteOutMove();
//   cout << endl;
//   game->DoMove(em->GetMove());
//   delete eml;
//   delete em;
//   eml = game->GetEvalMoveList(1);
//   eml->OrderMoveList(p->GetColorToMove());
//   eml->WriteOutMoves();
  delete game;
  game = 0x0;
  p = 0x0;
}

void Test::TestMemoryConsumption() {
 cout << endl << "Begin Testing Memory consumtion" << endl << endl; 
 cout << "Test Memory consumption PieceList" << endl;
 PieceList* pl = new PieceList();
 pl->SetOwner(true);
 int mem = getCurrentMemoryUbuntu();
 cout << mem << endl;
 int i=0;
 while (mem == getCurrentMemoryUbuntu() && i<10e1) {
  pl->CreatePiece(blackNumber,king,4,5);
  int id = pl->GetPieceOnField(4,5)->GetID();
  pl->DeletePiece(id);
  i++;
 }
 cout << getCurrentMemoryUbuntu() << " " << i << endl;
 cout << "End Test Memory consumption PieceList" << endl;
 
 cout << "Test Memory consumption Position::Execute/Retract Move" << endl;
 Position* pos = new Position();
 pos->CreatePiece(whiteNumber,king,1,1);
 pos->CreatePiece(blackNumber,king,8,8);
 Move* m = new Move(1,1,2,2);
 i=0;
 mem = getCurrentMemoryUbuntu();
 cout << mem << endl;
 while (mem == getCurrentMemoryUbuntu() && i<10e4) {
   ReverseMove* rm = new ReverseMove();
   pos->ExecuteMove(m,rm);
   pos->RetractMove(rm);
   delete rm; 
   i++;
 }
 cout << getCurrentMemoryUbuntu() << " " << i << endl;
 return;
}