#include <cmath>
#include <iostream>
using namespace std;

#include "Position.h"
#include "Chess.h"
#include "SupportFunctions.h"
#include "Piece.h"
#include "PieceList.h"
#include "Move.h"


Position::Position(PieceList *white, PieceList *black, short colortomove, bool whitecancastleshort, bool whitecancastlelong, bool blackcancastleshort, bool blackcancastlelong, short enpassantfile) {
  this->white = white;
  this->black = black;
  this->board = new Piece*[MaxFile * MaxRank];
  for (int i=0;i<(MaxFile * MaxRank);i++) {
    board[i] = 0x0;
  }
  SetBoardPointer(white);
  SetBoardPointer(black);
  this->colortomove = colortomove;
  this->whitecancastleshort = whitecancastleshort;
  this->whitecancastlelong = whitecancastlelong;
  this->blackcancastleshort = blackcancastleshort;
  this->blackcancastlelong = blackcancastlelong;
  CorrectCastling(whiteNumber);
  CorrectCastling(blackNumber);
  this->enpassantfile = enpassantfile;
  CorrectEnPassantFile();
}


Position::~Position() {
  if (white) {
    white->SetOwner(true);
  }
  delete white;
  if (black) {
    black->SetOwner(true);
  }
  delete black;
  delete board;
  board = 0x0;
}

void Position::SetColorToMove(short color) {
  if (color == whiteNumber || color == blackNumber) {
    colortomove = color;
  }
  else {
    cout << "Attention: Not a correct number for the color." << endl;
  }
  return;
}

void Position::SetUpStartPosition() {
  colortomove = whiteNumber;
  whitecancastlelong = whitecancastleshort = blackcancastlelong = blackcancastleshort = true;
  enpassantfile = 0;
  delete board;
  board = new Piece*[MaxFile * MaxRank]; 
  if (white) {
    white->ClearList();
  }
  else {
    white = new PieceList();
  }
  white->SetUpStartPosition(whiteNumber);
  SetBoardPointer(white);
  if (black) {
    black->ClearList();
  }
  else {
    black = new PieceList();
  }
  black->SetUpStartPosition(blackNumber);
  SetBoardPointer(black);
}

void Position::SetBoardPointer(PieceList *pl) {
  if (!pl) 
    return;
  
  if (!board) {
    board = new Piece*[MaxFile * MaxRank];
  }
  
  Piece *p = pl->GetPiece();
  if (p) {
    SetBoardPointer(p);
  }
  SetBoardPointer(pl->GetNext());

}

void Position::SetBoardPointer(Piece *p, short file, short rank) {
  if (p && !file && !rank) {
    file = p->GetFile();
    rank = p->GetRank();
  }
  if (!board) {
    board = new Piece*[MaxFile * MaxRank];
  }
  board[(file - 1) * MaxFile + (rank -1)] = p;
}

void Position::CorrectCastling(short color) {
  if (!CanColorCastle(color, true) && !CanColorCastle(color, false))
    return;
  
  if (!IsCastlingPossibleFromPosition(color, true)) {
    SetColorCastle(color, true, false);
    cout << "Castling corrected: " << GetColor(color) << " can not castle short." << endl;
  }
  
  if (!IsCastlingPossibleFromPosition(color, false)) {
    SetColorCastle(color, false, false);
    cout << "Castling corrected: " << GetColor(color) << " can not castle long." << endl;
  }
}

bool Position::CanColorCastle(short color, bool shortside) const {
  if (color == whiteNumber) {
    if (shortside) {
      return whitecancastleshort;
    }
    else {
      return whitecancastlelong;
    }
  }
  if (color == blackNumber) {
    if (shortside) {
      return blackcancastleshort;
    }
    else {
      return blackcancastlelong;
    }
  }
  return false;
}

void Position::SetColorCastle(short color, bool shortside, bool flag) {
  if (color == whiteNumber) {
    if (shortside) {
      whitecancastleshort = flag;
    }
    else {
      whitecancastlelong = flag;
    }
  }
  else {
    if (shortside) {
      blackcancastleshort = flag;
    }
    else {
      blackcancastlelong = flag;
    }
  }
}

bool Position::IsCastlingPossibleFromPosition(short color, bool shortside) const {
  short BackRank = 0;
  short RookFile = shortside ? MaxFile : 1;
  if (color == whiteNumber) {
    BackRank = 1;
  }
  if (color == blackNumber) {
    BackRank = MaxRank;
  }
  
  Piece* possibleKing = GetPieceOnField(5,BackRank);
  if (!possibleKing)
    return false;
  
  Piece *possibleRook = GetPieceOnField(RookFile, BackRank);
  if (!possibleRook)
    return false;
  
  if (possibleKing->GetType() == king && possibleKing->GetColor() == color 
   && possibleRook->GetType() == rook && possibleRook->GetColor() == color)
    return true;
  
  return false;
}

void Position::CorrectEnPassantFile() {
  if (enpassantfile && !IsEnPassantPossibleOnFile(enpassantfile)) {
    cout << "No en passant at the " << GetFile(enpassantfile) << "-file. Setting enpassantfile to 0." << endl;
    enpassantfile = 0;
  }
  return;
}

bool Position::EnPassantPossible() const {
  short file=1;
  bool possible = false;
  while (!possible && file <=MaxFile) {
    possible = IsEnPassantPossibleOnFile(file);
    file++;
  }
  return possible; 
}


bool Position::IsEnPassantPossibleOnFile(short file) const {
  if (!enpassantfile)
    return false;
  
  short rank;
  if (colortomove == whiteNumber) {
    rank = 5;
  }
  if (colortomove == blackNumber) {
    rank = 4;
  }

  Piece* pawnToBeTaken = GetPieceOnField(file, rank);
  if (!pawnToBeTaken || !(pawnToBeTaken->GetType() == pawn) || pawnToBeTaken->GetColor() == colortomove)
    return false;
  
  Piece* possibleLeftPawn = GetPieceOnField(file-1, rank);
  Piece* possibleRightPawn = GetPieceOnField(file+1, rank);
  
  if (!(possibleLeftPawn && possibleLeftPawn->GetType() == pawn && possibleLeftPawn->GetColor() == colortomove) 
  || (possibleRightPawn && possibleRightPawn->GetType() == pawn && possibleRightPawn->GetColor() == colortomove)) {
    return false;
  }
  return true;
}  

Piece* Position::GetPieceOnField(short searchedfile, short searchedrank) const {
  if (OutOfBound(searchedfile, searchedrank)) {
    return 0x0;
  }
  return board[(searchedfile-1) * MaxFile + (searchedrank-1)];
}

void Position::DeletePieceOnField(short searchedfile, short searchedrank) {
  Piece *p = GetPieceOnField(searchedfile, searchedrank);
  if (p->GetColor() == whiteNumber) {
    white->DeletePiece(p->GetID());
  }
  if (p->GetColor() == blackNumber) {
    black->DeletePiece(p->GetID());
  }
  SetBoardPointer(0x0, searchedfile, searchedrank);
}

bool Position::IsLegal() const {
  cout << "Attention: Position::IsValid() return always true at the moment." << endl;
  return true;
}

bool Position::IsChecked(short color) {
  short old = GetColorToMove();
  bool result = false;
  if (color == whiteNumber) {
    SetColorToMove(blackNumber);
    result = IsChecked();
    SetColorToMove(old);
  }
  else if (color == blackNumber) {
    SetColorToMove(whiteNumber);
    result = IsChecked();
    SetColorToMove(old);
  }
  return result;
}

bool Position::IsChecked() const {
  if (!white || !black) {
    return false;
  }
  Piece *p = 0x0;
  if (colortomove == whiteNumber) {
    p = black->GetKing();
  }
  if (colortomove == blackNumber) {
    p = white->GetKing();
  }
  if (!p)
    return false;
  
  const short kingfile = p->GetFile();
  const short kingrank = p->GetRank();
  p = 0x0;
 
  //Check check from the diagonals
  if (DiagonalCheck(kingfile,kingrank))
    return true;   
  
  //Check check from lines
  if (LineCheck(kingfile,kingrank)) 
    return true;
    
  //Check Knightcheck
  if (KnightCheck(kingfile,kingrank)) 
    return true;
  
  return false;
}

bool Position::DiagonalCheck(short kingfile, short kingrank) const {
  short opponenttype;
  const Piece *p = 0x0;
  //Check up right (as seen from white)
  for (int i=1;i<=min(MaxFile-kingfile,MaxRank-kingrank);i++) {
    p = GetPieceOnField(kingfile+i, kingrank+i);
    
    if(!p) 
      continue;
    
    if (p->GetColor() == colortomove && CheckDiagonalCheck(p->GetType(),i,blackNumber)) {
      return true;
    }
    else {
      break;
    }
  }
  //Check down right (as seen from white)
  for (int i=1;i<=min(MaxFile-kingfile,kingrank-1);i++) {
    p = GetPieceOnField(kingfile+i, kingrank-i);
    
    if(!p) 
      continue;
    
    if (p->GetColor() == colortomove && CheckDiagonalCheck(p->GetType(),i,whiteNumber)) {
      return true;
    } 
    else {
      break;
    }
  }
  //Check down left (as seen from white)
  for (int i=1;i<=min(kingfile-1,kingrank-1);i++) {
    p = GetPieceOnField(kingfile-i, kingrank-i);
    
    if(!p) 
      continue;

    if (p->GetColor() == colortomove && CheckDiagonalCheck(p->GetType(),i,whiteNumber)) {
      return true;
    }   
    else {
      break;
    }
  }
  //Check up left (as seen from white)
  for (int i=1;i<=min(kingfile-1,MaxRank-kingrank);i++) {
    p = GetPieceOnField(kingfile-i, kingrank+i);
    
    if(!p) 
      continue;

    if (p->GetColor() == colortomove && CheckDiagonalCheck(p->GetType(),i,blackNumber)) {
      return true;
    }
    else {
      break;
    }
  }  
  return false;
}

bool Position::CheckDiagonalCheck(short opponenttype, short distance, short colorpawncheck) const {
  switch (opponenttype) {
    case pawn:
      if (colortomove==colorpawncheck && distance==1)
	return true;
      return false;
    case bishop:
      return true;
    case knight:
      return false;
    case queen:
      return true;
    case rook:
      return false;
    case king:
      if (distance==1)
	return true;
      return false;
  }  
  return false;
}

bool Position::LineCheck(short kingfile, short kingrank) const {
  short opponenttype;
  const Piece *p = 0x0;
  for (int i=kingfile+1;i<=MaxFile;i++) {
    p = GetPieceOnField(i, kingrank);
 
    if(!p) 
      continue;

    if (p->GetColor() == colortomove && CheckLineCheck(p->GetType(),i-kingfile)) {
      return true;
    }
    else {
      break;
    }
  }
  for (int i=kingfile-1;i>=1;i--) {
    p = GetPieceOnField(i, kingrank);

    if(!p) 
      continue;
     
    if (p->GetColor() == colortomove && CheckLineCheck(p->GetType(),kingfile-i)) {
      return true;
    }
    else {
      break;
    }
  }
  for (int i=kingrank+1;i<=MaxRank;i++) {
    p = GetPieceOnField(kingfile, i);
    
    if(!p) 
      continue;

    if (p->GetColor() == colortomove && CheckLineCheck(p->GetType(),i-kingrank)) {
      return true;
    }
    else {
      break;
    }
  }
  for (int i=kingrank-1;i>=1;i--) {
    p = GetPieceOnField(kingfile, i);
    
    if(!p) 
      continue;

    if (p->GetColor() == colortomove && CheckLineCheck(p->GetType(),kingrank-i)) {
      return true;
    }
    else {
      break;
    }
  }
  return false;
}

bool Position::CheckLineCheck(short opponenttype, short distance) const {
  if (opponenttype == king && distance == 1)
    return true;
  if (opponenttype == rook || opponenttype == queen)
    return true;
  return false;  
}


bool Position::KnightCheck(short kingfile, short kingrank) const {
  const Piece *p = 0x0;
  short file,rank;
  for (int i=-2;i<=2;i++) {
    for (int j=-2;j<=2;j++) {
      if ((abs(i)+abs(j)) != 3)
	continue;
      
      file = kingfile+i;
      rank = kingrank+j;
      
      p = GetPieceOnField(file,rank);
      if(p) {
	if (p->GetColor() == colortomove || p->GetType() == knight)
	  return true;
      }
    }
  }
  return false;
}

bool Position::IsMoveLegal(Move* m) const {
  cout << "Attention: Legality only checks at the moment if there is a piece on the given field at that it has the right color." << endl;
  Piece* p = GetPieceOnField(m->GetStartFile(), m->GetStartRank());
  if (p && p->GetColor() == GetColorToMove()) {
    return true;
  }
  else {
    return false;
  }
}

bool Position::IsMovePromotion(Move* m) {
  short otherbackrank = 0;
  if (colortomove == whiteNumber)
    otherbackrank = MaxRank;
  if (colortomove == blackNumber)
    otherbackrank = 1;
  
  Piece* possiblepawn = GetPieceOnField(m->GetStartFile(), m->GetStartRank());
  if (possiblepawn && possiblepawn->GetType() == pawn && m->GetTargetRank() == otherbackrank)
    return true;
  else
    return false;
}  

void Position::CreatePiece(Piece *p) {
  SetBoardPointer(p);
  if (p->GetColor() == whiteNumber) {
    if (white) {
      white->CreatePiece(p);
    }
    else {
      white = new PieceList(p);
    }
  }
  else {
    if (black) {
      black->CreatePiece(p);
    }
    else {
      black = new PieceList(p);
    }
  }
}

void Position::CreatePiece(short type, short color, short filenumber, short ranknumber) {
  Piece *p = new Piece(type,color,filenumber,ranknumber);
  SetBoardPointer(p);
  if (color == whiteNumber) {
    if (white) {
      white->CreatePiece(p);
    }
    else {
      white = new PieceList(p);
    }
  }
  else {
    if (black) {
      black->CreatePiece(p);
    }
    else {
      black = new PieceList(p);
    }
  }
}

void Position::CapturePiece(Move *m, PieceList *pl) {
  short ID = m->GetIDofCapturedPiece();
  //Test if a piece has been taken. If yes, proceed
  if (ID) {
    //Test, if already the first element of the list is captured. If, then set the appropriate list pointer (white or black) to the next element of the list, delete the first element and set the temporary pointer plexamined to the nullpointer. Furthermore, the appropriate pointer on the board has to be set to zero Then: method is finished
    if (pl->IsPiece(ID)) {
      PieceList *newpl = pl->GetNext();
      delete pl;
      pl = newpl;
      newpl = 0x0;
    }
    else {
      //If it is not the first element, than we have to enter a loop. First, we have to define the previous element. We need this so that we can connect the next element (after the captured one) to the previous
      PieceList *previous;
      previous = pl;
      bool captured = false;                    //Loop variable
      //The Loop runs until the piece has been captured. We should only enter the loop if the move generating functions has found an appropriate piece. Therefore, if the loop runs until the last pointer, the segmentation fault is an error because wrong written method there
      while (!captured) {
	pl = previous->GetNext();
	if (!pl) cout << "Error: No piece to capture." << endl;   //Only for testing. Should be removed later.
	if (pl->IsPiece(ID)) {
	  previous->SetNext(pl->GetNext());	  
	  delete pl;
	  pl = 0x0;
	  captured = true;
	}
	else {
	  previous = pl;
	}
      }
      previous = 0x0;
    }
  }
}

void Position::ExecuteMove(Move *m) {
  if (colortomove != whiteNumber && colortomove != blackNumber) {
    cout << "Define Color to Move" << endl;
    return;
  }
  if (!m)
    return;
  
  const short startfile = m->GetStartFile();
  const short startrank = m->GetStartRank();
  const short targetfile = m->GetTargetFile();
  const short targetrank = m->GetTargetRank();
  Piece* cp = 0x0;
  if (!m->GetIDofCapturedPiece()) {
    cp = GetPieceOnField(targetfile, targetrank);
      
    if(cp) 
      m->SetIDofCapturedPiece(cp->GetID());	
  }
  cp = 0x0;
  Piece *p = GetPieceOnField(startfile, startrank);
  const short movedpiecetype = p->GetType();
  PieceList *activelist = 0x0;
  PieceList *passivelist = 0x0;
  short passivecolor, groundrank, enpassantrank, targetrankforenpassant;
  if (colortomove == whiteNumber) {
    activelist = white;
    passivelist = black;
    passivecolor = blackNumber;
    groundrank = 1;
    enpassantrank = 5;
    targetrankforenpassant = 6;
  }
  else {
    activelist = black;
    passivelist = white;
    passivecolor = whiteNumber;
    groundrank = 8;
    enpassantrank = 4;
    targetrankforenpassant = 3;
  }
  activelist->MovePiece(m);
  if (enpassantfile && (movedpiecetype == pawn) && (targetfile == enpassantfile) && (targetrank == targetrankforenpassant)) {
    cp = GetPieceOnField(targetfile, enpassantrank);
    m->SetIDofCapturedPiece(cp->GetID());
    //If the pawn has been taken en passant, the Board Pointer has to be set here to 0
    SetBoardPointer(0x0, enpassantfile, enpassantrank);
  }
  cp = 0x0;
  passivelist->SetOwner(true);
  passivelist->DeletePiece(m);
  passivelist->SetOwner(false);
  if ((movedpiecetype == king) && (startfile == 5) && (startrank == groundrank)) {
    short rookstartfield, rooktargetfield;
    rookstartfield = rooktargetfield = 0;
    if (targetfile == 7) {
      rookstartfield = 8;
      rooktargetfield = 6;
    }
    if (targetfile == 3) {
      rookstartfield = 1;
      rooktargetfield = 4;
    }
    if (rookstartfield) {
      Piece *r = GetPieceOnField(rookstartfield,groundrank);
      Move *rookcastlemove = new Move(rookstartfield,groundrank,rooktargetfield,groundrank);
      activelist->MovePiece(rookcastlemove);
      SetBoardPointer(r);      
      SetColorCastle(colortomove,true,false);
      SetColorCastle(colortomove,false,false);
    }	
  }
  if ((movedpiecetype == rook) && (startfile == 8) && (startrank == groundrank)) {
    SetColorCastle(colortomove,true,false);
  }
  if ((movedpiecetype == rook) && (startfile == 1) && (startrank == groundrank)) {
    SetColorCastle(colortomove,false,false);
  }
  enpassantfile = 0;
  if ((movedpiecetype == pawn) && (abs(targetrank - startrank) == 2)) {
    enpassantfile = targetfile;
  }      
  colortomove = passivecolor;    
  SetBoardPointer(p);
  activelist = 0x0;
}

EvalMoveList* Position::MakeMoveList() const {
  if (colortomove == whiteNumber) {
    return white->MakeMoveList(this);
  }
  if (colortomove == blackNumber) {
    return black->MakeMoveList(this);
  }
}

void Position::WriteOutPosition() {
  cout << endl << "Summary of the position:" << endl;
  if (colortomove == whiteNumber) {
    cout << "It is whites turn." << endl;
  }
  else {
    cout << "It is blacks turn." << endl;
  }
  cout << "White Pieces:" << endl;
  if (white)
    white->WriteOutPieces();
  else
    cout << "No white pieces." << endl;
  cout << "Black Pieces:" << endl;
  if (black)
    black->WriteOutPieces();
  else 
    cout << "No white pieces." << endl;
  bool check = IsChecked(colortomove);
  if (check) {
    if (GetColorToMove() == whiteNumber) {
      cout << "White is checked." << endl;
    }
    else if (GetColorToMove() == blackNumber) {
      cout << "Black is checked." << endl;
    }
  }
  if (whitecancastleshort && IsCastlingPossibleFromPosition(1,true)) 
    cout << "White can castle short." << endl;
  if (whitecancastlelong && IsCastlingPossibleFromPosition(1,false))
    cout << "White can castle long." << endl;
  if (blackcancastleshort && IsCastlingPossibleFromPosition(-1,true))
    cout << "Black can castle short." << endl;
  if (blackcancastlelong && IsCastlingPossibleFromPosition(-1,false))
    cout << "Black can castle long." << endl;
  if (enpassantfile && IsEnPassantPossibleOnFile(enpassantfile)) {
    if (colortomove == whiteNumber) {
      cout << "The pawn on " << GetField(enpassantfile, 5) << " could be taken en passant." << endl;
    }
    else {
      cout << "The pawn on " << GetField(enpassantfile, 4) << " could be taken en passant." << endl;
    }
  }
}