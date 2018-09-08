#include <iostream>
#include <string>
using namespace std;

#include "Piece.h"
#include "Move.h"
#include "ReverseMove.h"
#include "Evaluation.h"
#include "Chess.h"
#include "SupportFunctions.h"
#include "EvalMoveList.h"
#include "Position.h"

Piece::Piece(short type, short color, short file, short rank) {
  if ((type > pawn) || (color > 1) || (color < -1) || (file > MaxFile) || (rank > MaxRank)) {
    cout << "Error in Piece Constructor" << endl;
    cout << "Type: " << type << endl;
    cout << "Color: " << color << endl;
    cout << "File: " << file << endl;
    cout << "Rank: " << rank << endl;
    return;
  }
  this->type = type;
  this->color = color;
  this->file = file;
  this->rank = rank;
  this->ID = NextID();
}

Piece::Piece(const Piece *p) {
  this->type = p->GetType();
  this->color = p->GetType();
  this->file = file;
  this->rank = rank;
  this->ID = p->GetID();
}

Piece::~Piece() {
}

short Piece::IsOnField(short searchedfile, short searchedrank) const{
  if ((file==searchedfile) && (rank==searchedrank)) {
    if (type == king) {
      return -ID;
    }
    else {
      return ID;
    }
  }
  else {
    return 0;
  }
}

float Piece::GetValue(const Evaluation *eval) const {
  if (!eval) {
    Evaluation *dummyevaluation = new Evaluation();
    switch(type) {
    case queen: return dummyevaluation->GetQueenValue();
    case rook: return dummyevaluation->GetRookValue();
    case bishop: return dummyevaluation->GetBishopValue();
    case knight: return dummyevaluation->GetKnightValue();
    case pawn: return dummyevaluation->GetPawnValue();
    default: return 0.0;
    }
    delete dummyevaluation;
    dummyevaluation = 0x0;
  }
  else {
    switch(type) {
    case queen: return eval->GetQueenValue();
    case rook: return eval->GetRookValue();
    case bishop: return eval->GetBishopValue();
    case knight: return eval->GetKnightValue();
    case pawn: return eval->GetPawnValue();
    default: return 0.0;
    }
  }    
}

void Piece::MovePiece(Move *m) {
  SetFile(m->GetTargetFile());
  SetRank(m->GetTargetRank());
  short promotion = m->GetPromotion();
  short opponentgroundrank = GetColor()==whiteNumber ? MaxRank : 1;
  if (promotion) {
    if (!(GetType() == pawn) && rank == opponentgroundrank) 
      cout << "Error: Only pawns can do a promotion." << endl;
    else 
      SetType(promotion);
  }
  else if (GetType() == pawn && rank == opponentgroundrank) {
    cout << "Promote pawn automatically to queen" << endl;
    SetType(queen);
  }
}

Piece* Piece::RetractMove(ReverseMove *rm) {
  SetFile(rm->GetTargetFile());
  SetRank(rm->GetTargetRank());
  if (rm->GetPromotion()) 
    SetType(pawn);
  
  short typecp = rm->GetTypeofCapturedPiece();
  
  Piece* cp = 0x0;
  short opponentenpassantrank = GetColor()==whiteNumber ? 5 : 4;
  if (typecp >= king && type <= pawn) {
    if (rm->GetEnPassantFile() != rm->GetStartFile() || GetType() != pawn || rm->GetStartRank() != opponentenpassantrank+GetColor()) 
      cp = new Piece(typecp,-1 * GetColor(),rm->GetStartFile(), rm->GetStartRank());
    else
      cp = new Piece(pawn,-1 * GetColor(),rm->GetStartFile(), opponentenpassantrank);

    cp->SetID(rm->GetIDofCapturedPiece());
  }  
  return cp;
}

EvalMoveList* Piece::MakeMoveList(Position* const p) const {
  EvalMoveList* ml = new EvalMoveList();
  if (type == king) {
    for (int i=-1;i<=1;i++) {
      for (int j=-1;j<=1;j++) {
	if (!(i == 0 && j == 0)) {
	  Move* m = new Move(file,rank,file+i,file+j);
	  if (p->IsMoveLegal(m))
	    ml->SetMove(m);
	  else
	    delete m;
	  
	  m = 0x0;
	}
      }
    }
  }
  else if (type == queen) {
    for (int i=1;i<=MaxFile-file;i++) {
      Move* m = new Move(file,rank,file+i,file);
      if (p->IsMoveLegal(m))
        ml->SetMove(m);
      else
        delete m;
      
      m = 0x0;
      if(p->GetPieceOnField(file+i,rank))
	break;
    }
    for (int i=1;i<=file-1;i++) {
      Move* m = new Move(file,rank,file-i,file);
      if (p->IsMoveLegal(m))
        ml->SetMove(m);
      else
        delete m;
      
      m = 0x0;
      if(p->GetPieceOnField(file-i,rank))
	break;
    }
    //TODO: From here
    for (int i=1;i<=MaxFile-file;i++) {
      Move* m = new Move(file,rank,file+i,file);
      if (p->IsMoveLegal(m))
        ml->SetMove(m);
      else
        delete m;
      
      m = 0x0;
      if(p->GetPieceOnField(file+i,rank))
	break;
    }
    for (int i=1;i<=MaxFile-file;i++) {
      Move* m = new Move(file,rank,file+i,file);
      if (p->IsMoveLegal(m))
        ml->SetMove(m);
      else
        delete m;
      
      m = 0x0;
      if(p->GetPieceOnField(file+i,rank))
        break;
    }
    for (int i=1;i<=MaxFile-file;i++) {
      Move* m = new Move(file,rank,file+i,file);
      if (p->IsMoveLegal(m))
        ml->SetMove(m);
      else
        delete m;
      
      m = 0x0;
      if(p->GetPieceOnField(file+i,rank))
	break;
    }
    for (int i=1;i<=MaxFile-file;i++) {
      Move* m = new Move(file,rank,file+i,file);
      if (p->IsMoveLegal(m))
        ml->SetMove(m);
      else
        delete m;
      
      m = 0x0;
      if(p->GetPieceOnField(file+i,rank))
	break;
    }
    for (int i=1;i<=MaxFile-file;i++) {
      Move* m = new Move(file,rank,file+i,file);
      if (p->IsMoveLegal(m))
        ml->SetMove(m);
      else
        delete m;
      
      m = 0x0;
      if(p->GetPieceOnField(file+i,rank))
	break;
    }
    for (int i=1;i<=MaxFile-file;i++) {
      Move* m = new Move(file,rank,file+i,file);
      if (p->IsMoveLegal(m))
        ml->SetMove(m);
      else
        delete m;
      
      m = 0x0;
      if(p->GetPieceOnField(file+i,rank))
	break;
    }
    
  }
  else if (type == rook) {
    
  }
  else if (type == bishop) {
    
  }
  else if (type == knight) {
    
  }
  else if (type == pawn) {
    
  }
  return ml;
  return 0x0;
}

void Piece::WriteOutPiece() {
  if (color == whiteNumber) {
    cout << "White ";
  }
  else {
    cout << "Black ";
  }
  string field = GetField(file, rank);
  switch (type) {
    case 1: 
      cout << "King on " << field << ".";
      break;
    case 2:
      cout << "Queen on " << field << ".";
      break;
    case 3:
      cout << "Rook on " << field << ".";
      break;
    case 4:
      cout << "Bishop on " << field << ".";
      break;
    case 5:
      cout << "Knight on " << field << ".";
      break;
    case 6:
      cout << "Pawn on " << field << ".";
      break;
  }
}
/*
MoveTree Piece::MakeMoveTree(Position p) {
  
}*/
