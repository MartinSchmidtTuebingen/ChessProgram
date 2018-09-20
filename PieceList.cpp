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
  if (p && GetOwner())
    delete p;
  p = 0x0;
  if (next)
    delete next;
  
  next = 0x0;
}

void PieceList::ClearList() {
  if (next) {
    next->ClearList();
    delete next;
    next = 0x0;
  }
  if (GetOwner()) {
    delete p;
  }
  p = 0x0;
}

void PieceList::CreatePiece(Piece *np) {
  if (!np)
    return;
  
  if (p) {
    if (!next)
     next = new PieceList();
    
    next->CreatePiece(np);
  }
  else {
    p = np;
  }
}

void PieceList::CreatePiece(short type, short color, short filenumber, short ranknumber) { 
  Piece* np = new Piece(type,color,filenumber,ranknumber);
  CreatePiece(np);
}

void PieceList::SetUpStartPosition(short color) {
  if (MaxFile != 8 || MaxRank != 8)
    cout << "SetUpStartPosition only works for Standard Board" << endl;
  
  if (p || next) {
    SetOwner(true);
    ClearList();
  }
  
  short groundrank = color==whiteNumber ? 1 : MaxRank;
  short pawnrank = groundrank + color;
  int piecetypes[8] = {rook,knight,bishop,queen,king,bishop,knight,rook};
  for (int i=1;i<=8;i++) 
    CreatePiece(piecetypes[i-1],color,i,groundrank);  
  for (int i=1;i<=8;i++) 
    CreatePiece(pawn,color,i,pawnrank);
  
  return;
}

bool PieceList::IsPiece(short ID) const {
  if (p) 
    return ID==p->GetID();
  else 
    return false;
}

Piece* PieceList::FindPiece(short ID) const {
  if (IsPiece(ID))
    return GetPiece();
  else {
    if (next)
      return next->FindPiece(ID);
    else
      return 0x0;
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
    if(next) 
      next->MovePiece(m);
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
      if (!next->GetNext() && next->IsPiece(ID)) {
        delete next;
        next = 0x0;
      }
      else
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

EvalMoveList* PieceList::MakeMoveList(Position *pos) const {
  if (!GetPiece()) {
    if (next)
      return next->MakeMoveList(pos);
    else
      return 0x0;
  }
    
  EvalMoveList *eml = GetPiece()->MakeMoveList(pos);
  if (next) { 
    EvalMoveList* emlnext = next->MakeMoveList(pos);
    eml->Append(emlnext);
  }
  return eml;
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
  
