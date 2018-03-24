#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "GameManager.h"
#include "Chess.h"
#include "Move.h"
#include "Position.h"
#include "SupportFunctions.h"
#include "EvalMove.h"
#include "ChessGame.h"

GameManager::GameManager() {
  SetStatus(idle);
  pos = new Position();
  SetDepth(4);
}

GameManager::~GameManager() { 
}

void GameManager::StartGame() {
  while (GetStatus() != leave) {
    RequestInput();
    ChooseAction();
  }
  cout << "Thanks for Playing." << endl;
}

void GameManager::RequestInput() {
  if (GetStatus() == setupposition)
    return;
  if (GetStatus() != startgame)
    cout << "What to do next?" << endl;
  if (GetStatus() == idle) {
    cout << "P: Play a game" << endl;
    cout << "S: Set up a position" << endl;
    cout << "Q: Quit Game" << endl;
  }
  if (GetStatus() == positionsetted) {
    cout << "P: Play a game" << endl;
    cout << "A: Analyse" << endl;
    cout << "S: Set up another position" << endl;
    cout << "C: Change position" << endl;
    cout << "W: Write Out Position" << endl;
    cout << "Q: Quit Game" << endl;
  }
  if (GetStatus() == analyse) {
    cout << "W: WriteOutPosition" << endl;
    cout << "M: Give Moves" << endl;
    cout << "E: Give Evaluation" << endl;
    cout << "L: Leave Analysis" << endl;
    cout << "Q: Quit Game" << endl;
  }
  if (GetStatus() == startgame) {
    cout << "Do you want to start from the above position?" << endl;
    cout << "Y: Yes:" << endl;
    cout << "C: Change position" << endl;
    cout << "S: Set Startposition" << endl;
    cout << "L: Leave Game" << endl;
    cout << "Q: Quit" << endl;
  }
    
  string input;
  
  cin >> input;
  
  if (input == "Q" || input == "q") {
    SetStatus(leave);
  }
  else if (input == "S" || input == "s") {
    if (GetStatus() == idle || GetStatus() == positionsetted)
      SetStatus(setupposition);
    if (GetStatus() == startgame) {
      pos->SetUpStartPosition();
      SetStatus(playgame);
    }
  }
  else if (input == "A" || input == "a") {
    SetStatus(analyse);
  }
  else if (input == "C" || input == "c") {
    ChangePosition();
  }
  else if (input == "W" || input == "w") {
    pos->WriteOutPosition();
  }
  else if (input == "M" || input == "m") {
    PerformAnalysis();
  }
  else if (input == "E" || input == "e") {
    cout << "Attention: Not implemented" << endl;
    cout << "0.0" << endl;
    return;
  }
  else if (input == "L" || input == "l") {
    SetStatus(positionsetted);
  }
  else if (input == "P" || input == "p") {
    SetStatus(startgame);
  }
  else if (input == "Y" || input == "y") {
    SetStatus(playgame);
  }
  else {
    cout << "Input not recognised. Try again!" << endl;
    RequestInput();
  }
}

void GameManager::ChooseAction() {
  if (GetStatus() == leave)
    return;
  
  if (GetStatus() == setupposition) {
    SetUpPosition();
    SetStatus(positionsetted);
    return;
  }
  if (GetStatus() == analyse) {
    if (!pos || !pos->IsLegal()) {
      cout << "Analyse cannot be done without a valid position. Give a valid position!" << endl;
      SetStatus(positionsetted);
    }
    return;
  }
  if (GetStatus() == startgame) {
    pos->WriteOutPosition();
    if (!pos->IsLegal()) {
      cout << "Position is not legal. Please change it." << endl;
      SetStatus(positionsetted);
    }
  } 
  if (GetStatus() == playgame) {
    cout << "Which color do you choose? (W/B). Press C for a computer duel. Anything else to break" << endl;
    string input;
    cin >> input;
    int humancolor = 0;
    if (input == "W" || input == "w") {
      humancolor = whiteNumber;
    }
    else if (input == "S" || input == "s" || input == "B" || input == "b") {
      humancolor = blackNumber;
    }
    else if (input == "C" || input == "c") {
      humancolor = 0;
    }
    ManageGame(humancolor);
  }
  return;
}

void GameManager::SetUpPosition() {
  cout << "Delete old position" << endl;
  delete pos;
  pos = new Position();
  cout << "Give white pieces." << endl;
  SetUpPieces(whiteNumber);
  cout << "Give black pieces." << endl;
  SetUpPieces(blackNumber);
  ChangeAttributes();
  cout << "Position complete. It is:" << endl;
  pos->WriteOutPosition();
  return;
}

void GameManager::SetUpPieces(short color) {
  if (!pos) 
    pos = new Position();
  
  bool finished = false;
  string input;
  while (!finished) {
    cout << "Create piece (like Nf3 for a knight on f3 or d6 for a pawn on d6). Q quits the creation of pieces." << endl;
    cin >> input;
    if (input == "Q" || input == "q") {
      finished = true;
    }
    else {
      short type = GetTypeFromString(input);
      short file = GetFileFromString(input);
      short rank = GetRankFromString(input);
      if (!type || !file || !rank) {
	cout << "Input not recognised. Try again!" << endl;
	continue;
      }
      cout << type << " " << file << " " << rank << endl;
      pos->CreatePiece(type, color, file, rank);
    }
  }
  if (color == whiteNumber) {
    cout << "Creation of white pieces finished." << endl;
  }
  else {
    cout << "Creation of black pieces finished." << endl;
  }
  return;
}

void GameManager::DeletePieces() {
  if (!pos)
    return;
  
  bool finished = false;
  string input;
  short targetfile = 0;
  short targetrank = 0;
  while (!finished) {
    cout << "Give field of piece you want to delete. Q quits deletion." << endl;
    cin >> input;
    if (input == "Q" || input == "q") {
      finished = true;
    }
    else {
      if (!input.length() == 2) {
	cout << "Input not recognised. Try again!" << endl;
	continue;
      }
      string::iterator i = input.begin();
      string s;
      s = *i;
      targetfile = GetFileFromSingleString(s);
      i++;
      s = *i;
      targetrank = GetRankFromSingleString(s);
    }
    if (!targetfile || !targetrank) {
      cout << "Input not recognised. Try again!" << endl;
      continue;
    }
    else {
      pos->DeletePieceOnField(targetfile, targetrank);
    }
  }
}

void GameManager::ChangeAttributes() {
  short input;
  cout << "Whose turn is it? Give 1 for white and -1 for black." << endl;
  cin >> input;
  pos->SetColorToMove(input);
  if (pos->IsCastlingPossibleFromPosition(whiteNumber, true)) {
    cout << "Can White castle short? 1 for yes, 0 for no." << endl;
    cin >> input;
    pos->SetColorCastle(whiteNumber, true, input);
  }
  if (pos->IsCastlingPossibleFromPosition(whiteNumber, false)) {
    cout << "Can White castle long? 1 for yes, 0 for no." << endl;
    cin >> input;
    pos->SetColorCastle(whiteNumber, false, input); 
  }
  if (pos->IsCastlingPossibleFromPosition(blackNumber, true)) {
    cout << "Can Black castle short? 1 for yes, 0 for no." << endl;
    cin >> input;
    pos->SetColorCastle(blackNumber, true, input);
  }
  if (pos->IsCastlingPossibleFromPosition(blackNumber, false)) {
    cout << "Can Black castle long? 1 for yes, 0 for no." << endl;
    cin >> input;
    pos->SetColorCastle(blackNumber, false, input); 
  }
  
  if (pos->EnPassantPossible()) {
    input = -1;
    cout << "Give in file of pawn which can be taken en passant. 0 for no possibility." << endl;
    while (input == -1) {
      cin >> input;    
      if (input == 0) {
	pos->SetEnPassantFile(0);
      }
      else {
	if (!pos->IsEnPassantPossibleOnFile(input)) {
	  cout << "Sorry, en passant is not possible on this file. Try again. 0 for no possibility." << endl;
	  input == -1;
	}
	else {
	  pos->SetEnPassantFile(input);
	}
      }
    }
  }
  return;
}

void GameManager::PerformAnalysis() {
  Move* m = 0x0;
  do {
  m = RequestMove();
  pos->ExecuteMove(m);
  }
  while (m);
  return;
}

Move* GameManager::RequestMove() {
  cout << endl << "Give move in format <Startfile><Startrank><Targetfile><Targetrank>(<Promotion>). For example: a7a8D. Q quits and returns." << endl;
  Move* m = 0x0;
  string input;
  cin >> input;
  if (input == "Q" || input == "q") {
    return 0x0;
  }
  short startfile = 0;
  short startrank = 0;
  short targetfile = 0;
  short targetrank = 0;
  short promotion;
  if (input.length() == 4 || input.length() == 5) {
    string s;
    string::iterator i;
    i = input.begin();
    s = *i;
    startfile = GetFileFromSingleString(s);
    i++;
    s = *i;
    startrank = GetRankFromSingleString(s);
    i++;
    s = *i;
    targetfile = GetFileFromSingleString(s);
    i++;
    s = *i;
    targetrank = GetRankFromSingleString(s);
    if (input.length() == 5) {
      i++;
      s = *i;
      promotion = GetTypeFromSingleString(s);
    }
  }
  if (startfile && startrank && targetfile && targetrank) {
     m = new Move(startfile, startrank, targetfile, targetrank);
     if (pos->IsMoveLegal(m)) {
       if (pos->IsMovePromotion(m)) {
	 while (!promotion) {
	 cout << "This move is an promotion. Please specify the type of new piece with its starting letter." << endl;
	 cin >> input;
	 string::iterator j;
	 j = input.begin();
	 string s;
	 s = *j;
	 promotion = GetTypeFromSingleString(s);
       }
       m->SetPromotion(promotion);
      }
    }
    else {
      cout << "Move not possible. Try again." << endl;
      m = RequestMove();
    }
  }
  else {
    cout << "Input not recognised. Try again." << endl;
    m = RequestMove();
  }
  return m;
}

void GameManager::ChangePosition() {
  string input;
  cout << "Do you want to create new Pieces? Y/N" << endl;
  cin >> input;
  if (input == "Y" || input == "y") {
    cout << "Creating new white pieces." << endl;
    SetUpPieces(whiteNumber);
    cout << "Creating new black pieces." << endl;
    SetUpPieces(blackNumber);
  }
  cout << "Do you want to delete Pieces? Y/N" << endl;
  if (input == "Y" || input == "y") {
    DeletePieces();
  }
  cout << "Do you want to change the color which moves, the castling rights or where a pawn can be taken en passant? Y/N" << endl;
  if (input == "Y" || input == "y") {
    ChangeAttributes();
  }
  cout << "New Position completed." << endl;
}

void GameManager::ManageGame(int humancolor) {
  EvalMove* em = 0x0;
  Move* m = 0x0;
  ChessGame* game = new ChessGame();
  int movenumber ;
  if (pos->GetColorToMove() == whiteNumber) {
    movenumber = 1;
  }
  if (pos->GetColorToMove() == blackNumber) {
    movenumber = 2;
  }
    
  if (humancolor == 0) {
    do {
      delete m;
      delete em;
      em = game->GiveBestMoveWithEvaluation(pos);
      m = em->GetMove();
      pos->ExecuteMove(m);
      movenumber++;
    }
    while (false);
  }
  else if (humancolor == whiteNumber || humancolor == blackNumber) {
    cout << "Starting the game." << endl;
    do {
      delete m;
      delete em;
      if (pos->GetColorToMove() == humancolor) {
	m = RequestMove();
	pos->ExecuteMove(m);
      }
      else {
	em = game->GiveBestMoveWithEvaluation(pos, GetDepth());
	em->WriteOut();
	m = em->GetMove();
	pos->ExecuteMove(m);
      }
      if (em && em->GameOver()) {
	delete m;
	m = 0x0;
      }
      movenumber++;
    }
    while (m); 
    delete m;
    delete em;
    m = 0x0;
    em = 0x0;
  }
  return;
}


