#include <cmath>
#include <iostream>
using namespace std;

#include "Position.h"
#include "Chess.h"
#include "SupportFunctions.h"
#include "Piece.h"
#include "PieceList.h"
#include "Move.h"
#include "ReverseMove.h"


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
  castlingallowed[0] = whitecancastleshort;
  castlingallowed[1] = whitecancastlelong;
  castlingallowed[2] = blackcancastleshort;
  castlingallowed[3] = blackcancastlelong;
//   CorrectCastling(whiteNumber);
//   CorrectCastling(blackNumber);
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
  for (int i=0;i<(MaxFile * MaxRank);i++) {
    board[i] = 0x0;
  }
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
  for (short i=0;i<4;i++)
    castlingallowed[i]=true;
  
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
  
  if (!p && (!file || !rank))
    return;
  
//   cout << "SetBoardPointer: Before setting file and rank" << endl;
  if (!file || !rank) {
    file = p->GetFile();
    rank = p->GetRank();
  }
  else if (p) {
    p->SetFile(file);
    p->SetRank(rank);
  }
//   cout << "SetBoardPointer: Before setting pointer" << endl;
  
  if (!board) {
    board = new Piece*[MaxFile * MaxRank];
    for (int i=0;i<(MaxFile * MaxRank);i++) {
      board[i] = 0x0;
    }
  }
  board[(file - 1) * MaxFile + (rank -1)] = p;
  return;
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
  return castlingallowed[(color==whiteNumber ? 0 : 2) + (shortside ? 0 : 1)];
}

void Position::SetColorCastle(short color, bool shortside, bool flag) {
  castlingallowed[(color==whiteNumber ? 0 : 2) + (shortside ? 0 : 1)] = flag;
  return;
}

bool Position::IsCastlingPossibleFromPosition(short color, bool shortside) {
  short BackRank = color==whiteNumber ? 1 : MaxRank;
  short kingMiddleField = shortside ? 6 : 4;
  short kingTargetField = shortside ? 7 :3;
  short RookFile = shortside ? MaxFile : 1;

  Piece* possibleKing = GetPieceOnField(5,BackRank);
  if (!possibleKing)
    return false;
  
  if (color==whiteNumber)
    cout << "Läuft1" << endl;
  
  Piece *possibleRook = GetPieceOnField(RookFile, BackRank);
  if (!possibleRook)
    return false;
  
  
  if (color==whiteNumber)
    cout << "Läuft" << endl;
  
  if (!(possibleKing->GetType() == king && possibleKing->GetColor() == color 
   && possibleRook->GetType() == rook && possibleRook->GetColor() == color))
    return false;
  if (color==whiteNumber)
    cout << "Läuft" << endl;
  
  if (GetPieceOnField(kingMiddleField,BackRank) || GetPieceOnField(kingTargetField,BackRank))
    return false;
  if (color==whiteNumber)
    cout << "Läuft" << endl;

  if (!shortside && GetPieceOnField(2,BackRank))
    return false;
  if (color==whiteNumber)
    cout << "Läuft" << endl;
  
  if (IsChecked(color))
    return false;
  if (color==whiteNumber)
    cout << "Läuft" << endl;
  
  if (color==whiteNumber)
    cout << "Läuft" << endl;
  
  Move* m = new Move(5,BackRank,kingMiddleField,BackRank);
  DisplacePiece(m);
  if (IsChecked()) {
    m = new Move(kingMiddleField,BackRank,5,BackRank);
    DisplacePiece(m);
  if (color==whiteNumber)
    cout << "Läuft" << endl;
    return false;
  }
  else {
    m = new Move(kingMiddleField,BackRank,kingTargetField,BackRank);
    DisplacePiece(m);
    bool check = IsChecked(color);
    m = new Move(kingTargetField,BackRank,5,BackRank);
    DisplacePiece(m);
  if (color==whiteNumber)
    cout << check << endl;
    return !check;
  }
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
  
  short rank = GetColorToMove()==whiteNumber ? 5 : 4;

  Piece* pawnToBeTaken = GetPieceOnField(file, rank);
  if (!pawnToBeTaken || !(pawnToBeTaken->GetType() == pawn) || pawnToBeTaken->GetColor() == GetColorToMove())
    return false;
  
  Piece* possibleLeftPawn = GetPieceOnField(file-1, rank);
  Piece* possibleRightPawn = GetPieceOnField(file+1, rank);
  
  return (possibleLeftPawn && possibleLeftPawn->GetType() == pawn && possibleLeftPawn->GetColor() == GetColorToMove()) 
  || (possibleRightPawn && possibleRightPawn->GetType() == pawn && possibleRightPawn->GetColor() == GetColorToMove());
}  

Piece* Position::GetPieceOnField(short searchedfile, short searchedrank) const {
  if (OutOfBound(searchedfile, searchedrank)) {
    return 0x0;
  }
  return board[(searchedfile-1) * MaxFile + (searchedrank-1)];
}

void Position::DeletePieceOnField(short searchedfile, short searchedrank) {
  Piece *p = GetPieceOnField(searchedfile, searchedrank);
  if (!p)
    return;

  PieceList* activelist = p->GetColor()==whiteNumber ? white : black;
  activelist->DeletePiece(p->GetID());
  SetBoardPointer(0x0, searchedfile, searchedrank);
}

bool Position::IsLegal() const {
  cout << "Attention: Position::IsValid() return always true at the moment." << endl;
  return true;
}

bool Position::IsChecked(short color) {
  short old = GetColorToMove();
  SetColorToMove(-color);
  bool result = IsChecked();
  SetColorToMove(old);
  return result;
}

bool Position::IsChecked() const {
  if (!white || !black) {
    return false;
  }
  Piece *k = GetColorToMove()==whiteNumber ? black->GetKing() : white->GetKing();
  
  if (!k)
    return false;
  
  const short kingfile = k->GetFile();
  const short kingrank = k->GetRank();
  k = 0x0;
 
//   cout << "Check diagonal check" << endl;
  //Check check from the diagonals
  if (DiagonalCheck(kingfile,kingrank))
    return true;   
  
//   cout << "Check line check" << endl;
  //Check check from lines
  if (LineCheck(kingfile,kingrank)) 
    return true;
   
//   cout << "Check knight check: " << KnightCheck(kingfile,kingrank) << endl;
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

    if (p->GetColor() == colortomove && CheckLineCheck(p->GetType(),i-kingfile)) 
      return true;
    else 
      break;
  }
  for (int i=kingfile-1;i>=1;i--) {
    p = GetPieceOnField(i, kingrank);

    if(!p) 
      continue;
     
    if (p->GetColor() == colortomove && CheckLineCheck(p->GetType(),kingfile-i)) 
      return true;
    else 
      break;
  }
  for (int i=kingrank+1;i<=MaxRank;i++) {
    p = GetPieceOnField(kingfile, i);
    
    if(!p) 
      continue;

    if (p->GetColor() == colortomove && CheckLineCheck(p->GetType(),i-kingrank))
      return true;
    else 
      break;
  }
  for (int i=kingrank-1;i>=1;i--) {
    p = GetPieceOnField(kingfile, i);
    
    if(!p) 
      continue;

    if (p->GetColor() == colortomove && CheckLineCheck(p->GetType(),kingrank-i))
      return true;
    else 
      break;
  }
  return false;
}

bool Position::CheckLineCheck(short opponenttype, short distance) const {
  return (opponenttype == king && distance == 1) || (opponenttype == rook || opponenttype == queen);
}


bool Position::KnightCheck(short kingfile, short kingrank) const {
  for (int i=-2;i<=2;i++) {
    for (int j=-2;j<=2;j++) {
      if ((abs(i)+abs(j)) != 3)
        continue;
      
      Piece* p = GetPieceOnField(kingfile+i,kingrank+j);
      return (p && (p->GetColor() == colortomove || p->GetType() == knight));
    }
  }
  return false;
}

bool Position::IsMoveLegal(Move* m){
  cout << "Attention: Legality only checks at the moment if there is a piece on the start field that has the right color, whether is a piece of the same color on the end field and if the own king is checked after the move" << endl;
  Piece* p = GetPieceOnField(m->GetStartFile(), m->GetStartRank());
  Piece* cp = GetPieceOnField(m->GetTargetFile(), m->GetTargetRank());
  if (!p || p->GetColor() != GetColorToMove())
    return false;
  else if (cp && p->GetColor() == cp->GetColor())
    return false;
  else {
    ReverseMove* rm = new ReverseMove();
    ExecuteMove(m, rm);
    if (IsChecked())
      return false;
    RetractMove(rm);
  }
  return true;
}

bool Position::IsMovePromotion(Move* m) {
  short otherbackrank = GetColorToMove()==whiteNumber ? MaxRank : 1;
  Piece* possiblepawn = GetPieceOnField(m->GetStartFile(), m->GetStartRank());
  return (possiblepawn && possiblepawn->GetType() == pawn && m->GetTargetRank() == otherbackrank);
}  

void Position::CreatePiece(Piece *p) {
  if (!p)
    return;
  SetBoardPointer(p);
  if (p->GetColor() == whiteNumber) {
    if (white) 
      white->CreatePiece(p);
    else 
      white = new PieceList(p);
  }
  else {
    if (black) 
      black->CreatePiece(p);
    else 
      black = new PieceList(p);
  }
}

void Position::CreatePiece(short type, short color, short filenumber, short ranknumber) {
  Piece *p = new Piece(type,color,filenumber,ranknumber);
  CreatePiece(p);
}

void Position::DisplacePiece(Move *m) {
  if (!m)
    return;
  
  short startfile = m->GetStartFile();
  short startrank = m->GetStartRank();
  
  Piece* p = GetPieceOnField(startfile, startrank);
  if (!p)
    return;

  p->MovePiece(m);
  SetBoardPointer(p);
  SetBoardPointer(0x0,startfile,startrank);
}
  

void Position::CapturePiece(Move *m, ReverseMove* rm) {
  if (!m)
    return;
  
  short targetfile=m->GetTargetFile();
  short targetrank=m->GetTargetRank();
  
  PieceList *passivelist = GetColorToMove()==whiteNumber ? black : white;
  short enpassantrank = GetColorToMove()==whiteNumber ? 5 : 4;
  short targetrankforenpassant = enpassantrank + GetColorToMove();
  
  Piece* cp = 0x0;
  if (m->GetIDofCapturedPiece()) {
    cp = passivelist->FindPiece(m->GetIDofCapturedPiece());
    if (!cp)
      cout << "Warning: Given ID for captured piece, no piece found with this ID" << endl;
  }
  
  if (!cp)
    cp = GetPieceOnField(targetfile,targetrank);
  
  if (!cp) {
    //Find possible e.p.
    Piece* p = GetPieceOnField(m->GetStartFile(),m->GetStartRank());
    short movedpiecetype = p->GetType();
    if ((targetfile==enpassantfile) && (movedpiecetype==pawn) && (targetrank==targetrankforenpassant)) 
      cp = GetPieceOnField(targetfile,enpassantrank);
    p = 0x0;
  }
  
  if (!cp) 
    return;
  
  m->SetIDofCapturedPiece(cp->GetID());
  if (rm) {
    rm->SetIDofCapturedPiece(cp->GetID());
    rm->SetTypeofCapturedPiece(cp->GetType());
  }
  
  SetBoardPointer(0x0,cp->GetFile(),cp->GetRank());
  passivelist->SetOwner(true);
  passivelist->DeletePiece(m);
  passivelist->SetOwner(false);
  return;
}
  

void Position::RetractMove(ReverseMove* rm) {
  if (!rm)
    return;
  
  short startfile = rm->GetStartFile();
  short targetfile = rm->GetTargetFile();
  Piece* movedPiece = GetPieceOnField(startfile, rm->GetStartRank());
  if (!movedPiece)
    return;
  
  SetEnPassantFile(rm->GetEnPassantFile());
  SetColorCastle(movedPiece->GetColor(),true,rm->GetCastlingShort());
  SetColorCastle(movedPiece->GetColor(),false,rm->GetCastlingLong());
  Piece* cp = movedPiece->RetractMove(rm);
  SetBoardPointer(movedPiece, targetfile, rm->GetTargetRank());
  if (movedPiece->GetType() == king && abs(targetfile-startfile)==2) {
    short rookstartfile = startfile==3 ? 4 : 6;
    short rooktargetfile = startfile==3 ? 1: 8;
    Piece* r = GetPieceOnField(rookstartfile,rm->GetStartRank());
    if (r && r->GetType()==rook && r->GetColor() == movedPiece->GetColor()) {
      Move* rookmove = new Move(rookstartfile,rm->GetStartRank(),rooktargetfile,rm->GetStartRank());
      DisplacePiece(rookmove);
    }
  }
  if (cp)
    CreatePiece(cp);
  SetColorToMove(movedPiece->GetColor());
  return;
}

void Position::ExecuteMove(Move* m, ReverseMove* rm) {
  if (GetColorToMove() != whiteNumber && GetColorToMove() != blackNumber) {
    cout << "Define Color to Move" << endl;
    delete rm;
    rm = 0x0;
    return;
  }
  if (!m) {
    delete rm;
    rm = 0x0;
    return;
  }
  
  short groundrank = GetColorToMove()==whiteNumber ? 1 : MaxRank;
  const short startfile = m->GetStartFile();
  const short startrank = m->GetStartRank();
  const short targetfile = m->GetTargetFile();
  const short targetrank = m->GetTargetRank();
  cout << GetField(startfile,startrank) << "-" << GetField(targetfile,targetrank) << endl; 
  Piece *p = GetPieceOnField(startfile, startrank);
  if (!p) {
    cout << "Error: No Piece to move" << endl;
    delete rm;
    rm = 0x0;
    return;
  }
  
  if (p->GetColor() != GetColorToMove()) 
    cout << "Warning: Wrong color to move" << endl;

  if (rm) {
    rm->SetFields(targetfile, targetrank, startfile, startrank);
    rm->SetCastlingShort(CanColorCastle(GetColorToMove(),true));
    rm->SetCastlingLong(CanColorCastle(GetColorToMove(),false));
    rm->SetEnPassantFile(GetEnPassantFile());
  }
  const short movedpiecetype = p->GetType();
  
  PieceList *activelist = GetColorToMove()==whiteNumber ? white : black;
  if ((movedpiecetype == king) && (startfile == 5) && (startrank == groundrank) && (targetfile == 3 || targetfile == 7)) {
    short rookstartfile = targetfile==3 ? 1 : 8;
    short rooktargetfile = targetfile==3 ? 4 : 6;
    
    if ((rookstartfile==1 && !IsCastlingPossibleFromPosition(colortomove,false)) || rookstartfile==8 && !IsCastlingPossibleFromPosition(colortomove,true)) {
      cout << "Warning: Cannot castle in this position" << endl;
      delete rm;
      rm = 0x0;
      return;
    }
    
    if ((rookstartfile==1 && !CanColorCastle(colortomove,false)) || rookstartfile==8 && !CanColorCastle(colortomove,true))
      cout << "Warning: Castling not allowed" << endl;
    
    Piece *r = GetPieceOnField(rookstartfile,groundrank);
    if (r) {
      if (r->GetColor() != p->GetColor() || r->GetType() != rook) {
        cout << "Warning: Rook to castle not present" << endl;
      }
      else {
        Move *rookcastlemove = new Move(rookstartfile,groundrank,rooktargetfile,groundrank);
        DisplacePiece(rookcastlemove);  
      }
    }
  }
  
  CapturePiece(m,rm);
  DisplacePiece(m);
  
  if (movedpiecetype == king) {
    SetColorCastle(colortomove,true,false);
    SetColorCastle(colortomove,false,false);
  }
  
  if ((movedpiecetype == rook) && (startrank == groundrank) && ((startfile == 1) || (startfile == MaxFile)))
    SetColorCastle(GetColorToMove(),startfile==1 ? false : true,false);
  
  enpassantfile = 0;
  if ((movedpiecetype == pawn) && (abs(targetrank - startrank) == 2) && startrank == groundrank + GetColorToMove()) {
    enpassantfile = targetfile;
  }      
  SetColorToMove(-GetColorToMove());    
  activelist = 0x0;
  return;
}

EvalMoveList* Position::MakeMoveList() const {
  PieceList* activelist = GetColorToMove()==whiteNumber ? white : black;
  delete activelist;
  activelist->MakeMoveList(this);
}

void Position::WriteOutPosition() {
  bool check = IsChecked(GetColorToMove());
  bool castlings[4];
  for (short i=0;i<2;i++) {
    for (short j=0;j<2;j++) 
      castlings[2*i+j]=IsCastlingPossibleFromPosition(i==0 ? whiteNumber : blackNumber,j==0);
  }
  bool enpassantallowed=enpassantfile && IsEnPassantPossibleOnFile(enpassantfile);  
  
  cout << endl << "Summary of the position:" << endl;
  cout << "It is " << (GetColorToMove()==whiteNumber ? "whites" : "blacks") << " turn." << endl;
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
  
  if (check) 
    cout << (GetColorToMove()==whiteNumber ? "White" : "Black") << " is checked." << endl;
  
  for (short i=0;i<2;i++) {
    for (short j=0;j<2;j++) {
      if (CanColorCastle(i==0 ? whiteNumber : blackNumber,j==0)) {
        cout << (i==0 ? "White" : "Black") << " is allowed to castle " << (j==0 ? "short" : "long"); 
        if (castlings[2*i+j])
          cout << " and can in this position";
        cout << "." << endl;
      }
    }
  }
  
  if (enpassantallowed) 
    cout << "The pawn on " << GetField(enpassantfile, GetColorToMove()==whiteNumber ? 5 : 4) << " could be taken en passant." << endl;
}