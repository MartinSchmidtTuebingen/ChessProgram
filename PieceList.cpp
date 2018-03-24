#include <iostream>
using namespace std;

#include "PieceList.h"
#include "Chess.h"
#include "Move.h"
#include "EvalMoveList.h"
#include "Position.h"

PieceList::PieceList() {
  this->p = 0x0;
  this->next = 0x0;
  this->owner = false;
}

PieceList::PieceList(Piece *np, PieceList *nnext, bool nowner) {
  this->p = np;
  this->next = nnext;
  this->owner = nowner;
}

PieceList::PieceList(const PieceList *pl) {
  this->p = new Piece(pl->GetPiece());
  this->owner = pl->GetOwner();
  if (pl->GetNext()) {
    this->next = new PieceList(pl->GetNext());
  }
  else {
    this->next = 0x0;
  }
}

PieceList::PieceList(short piecetype, short piececolor, short piecefilenumber, short pieceranknumber, PieceList *nnext, bool nowner) {
  Piece *np = new Piece(piecetype, piececolor, piecefilenumber, pieceranknumber);
  this->p = np;
  this->next = nnext;
  this->owner = nowner;
}

PieceList::PieceList(const PieceList& copy) {
  this->p = new Piece(copy.GetPiece());
  this->owner = copy.GetOwner();
  if (copy.GetNext()) {
    this->next = new PieceList(copy.GetNext());
  }
  else {
    this->next = 0x0;
  }
}

PieceList::~PieceList() {
  ClearList();
}

void PieceList::ClearList() {
  if (next) {
    next->ClearList();
    delete next;
    next = 0x0;
  }
  if (GetOwner() == true) {
    delete p;
  }
  p = 0x0;
}

void PieceList::CreatePiece(Piece *np) {
  if (p) {
    PieceList *pl = new PieceList(np, next);
    next = pl;
  }
  else {
    p = np;
  }
}

void PieceList::CreatePiece(short type, short color, short filenumber, short ranknumber) { 
  if (p) {
    PieceList *pl = new PieceList(type, color, filenumber, ranknumber, next);
    next = pl;
  }
  else {
    Piece *np = new Piece(type,color,filenumber,ranknumber);
    p = np;    
  }
}

void PieceList::SetUpStartPosition(short color) {
  if (MaxFile != 8 || MaxRank != 8)
    cout << "SetUpStartPosition only works for Standard Board" << endl;
  
  if (p || next) {
    SetOwner(true);
    ClearList();
  }
  
  short groundrank,pawnrank;
  groundrank = pawnrank = 0;
  if (color == whiteNumber) {
    groundrank = 1;
    pawnrank = 2;
  }
  if (color == blackNumber) {
    groundrank = 8;
    pawnrank = 7;
  }
  CreatePiece(3,color,1,groundrank);
  CreatePiece(5,color,2,groundrank);
  CreatePiece(3,color,3,groundrank);
  CreatePiece(2,color,4,groundrank);
  CreatePiece(1,color,5,groundrank);
  CreatePiece(4,color,6,groundrank);
  CreatePiece(5,color,7,groundrank);
  CreatePiece(3,color,8,groundrank);
  for (int i=1;i<=8;i++) {
    CreatePiece(6,color,i,pawnrank);
  }
}

bool PieceList::IsPiece(short ID) const {
  if (p) {
    if (ID == p->GetID()) {
      return true;
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
}
    
short PieceList::IsAnyPieceOnField(short searchedfile, short searchedrank) const {
  if (p->IsOnField(searchedfile, searchedrank)) {
    return p->GetType();
  }
  else {
    //If this is the last piece in the list, return false
    if(!next) {
      return noPiece;
    }
    //Otherwise test the next element of the list
    else {
      return next->IsAnyPieceOnField(searchedfile, searchedrank);
    }
  }
}

short PieceList::GetType(short ID) const {
  if (p->IsPiece(ID)) {
    return p->GetType();
  }
  else {
    return next->GetType(ID);
  }
}

Piece* PieceList::GetPieceOnField(short searchedfile, short searchedrank) const {
  if (searchedfile < 1 || searchedfile > MaxFile || searchedrank < 1 || searchedrank > MaxRank) 
    return 0x0;
  if (p->IsOnField(searchedfile, searchedrank)) {
    return p;
  }
  else {
    if (next) {
      return next->GetPieceOnField(searchedfile, searchedrank);
    }
    else {
      return 0x0;
    }
  }
}

Piece *PieceList::GetKing() const {
  if (!p)
    return 0x0;
  
  if (p->GetType() == king) {
    return p;
  }
  else {
    if (next)
      return next->GetKing();
    else
      return 0x0;
  }
}
  

void PieceList::MovePiece(Move *m) {
  short file = m->GetStartFile();
  short rank = m->GetStartRank();
  if ((p->GetFile() == file) && (p->GetRank() == rank)) {
    p->MovePiece(m);
  }
  else {
    if(next) next->MovePiece(m);
  }
}

void PieceList::DeletePiece(Move *m) {
  DeletePiece(m->GetIDofCapturedPiece());
}

void PieceList::DeletePiece(short ID) {
  if (ID == 0)
    return;  
  if (IsPiece(ID)) {
    delete p;
    p = 0x0;
    if (next) {
      PieceList *oldnext;
      oldnext = next;      
      p = oldnext->GetPiece();
      next = oldnext->GetNext();
      oldnext->SetNext(0x0);
      oldnext->SetOwner(false);
      delete oldnext;
      oldnext = 0x0;
    }
  }
  else {
    if (next) {
      next->DeletePiece(ID);
    }
    else {
      cout << "Error: Piece to delete not found" << endl;
    }
  }
}

float PieceList::GetValuePieces(const Evaluation *eval) const {
  float result = p->GetValue(eval);
  if (next) result += next->GetValuePieces(eval);
  return result;
}

EvalMoveList* PieceList::MakeMoveList(const Position *p) const {
  EvalMoveList *ml = p->MakeMoveList();
  if (next) { 
    EvalMoveList* mlnext = next->MakeMoveList(p);
    ml->Append(ml);
  }
  return ml;
}
  

void PieceList::WriteOutPieces() const {
  if (p) {
    p->WriteOutPiece();
    cout << endl;
    if (next) {
      next->WriteOutPieces();
    }
  }
}
  
