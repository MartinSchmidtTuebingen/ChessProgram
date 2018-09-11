#include <cmath>
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

Move* Piece::CreateMoveIfPieceCanMoveToField(short targetfile, short targetrank, Position* pos) {
  Move* m = 0x0;
  if (GetType() != pawn) {
    Piece* cp = pos->GetPieceOnField(targetfile,targetrank);
    if (cp) {
      if (GetColor() != cp->GetColor()) 
        m = new Move(GetFile(),GetRank(),targetfile,targetrank,cp->GetID());
    }
    else
      m = new Move(GetFile(),GetRank(),targetfile,targetrank);
  }
  else if (GetType() == pawn) {
    short opponentgroundrank = GetColor()==whiteNumber ? MaxRank : 1;
    short enpassantrank = GetColor()==whiteNumber ? 6 :3;
    Piece* cp = pos->GetPieceOnField(targetfile,targetrank);
    if (GetFile() == targetfile) {
      if (!cp)
        m = new Move(GetFile(),GetRank(),targetfile,targetrank);
    }
    else {
      if (cp) {
        if (cp->GetColor() != GetColor())
          m = new Move(GetFile(),GetRank(),targetfile,targetrank,cp->GetID());
      }
      else if (targetfile==pos->GetEnPassantFile() && targetrank==enpassantrank){
        cp = pos->GetPieceOnField(targetfile,enpassantrank - GetColor());
        if (cp)
          m = new Move(GetFile(),GetRank(),targetfile,targetrank,cp->GetID());
        else
          cout << "Position illegal: En passant set, but no pawn on the field " << GetField(targetfile,enpassantrank - GetColor()) << endl;
      }
    }
    if (m && targetrank==opponentgroundrank)
      m->SetPromotion(queen);
  }
  if (m && pos->IsCheckedAfterMove(m)) {
    delete m;
    m = 0x0;
  }
  
  return m;
}

EvalMoveList* Piece::MakeMoveList(Position* pos) {
  EvalMoveList* eml = new EvalMoveList();
  int filedirections[8] = {1,-1,-1,1,1,-1,0,0};
  int rankdirections[8] = {1,-1,1,-1,0,0,1,-1};
  int start=0;
  int end=8;
  int startfile=GetFile();
  int startrank=GetRank();
  short maxDistance = max(MaxFile,MaxRank);
  int groundrank = GetColor()==whiteNumber ? 1 : MaxRank;
  switch(GetType()) {
    case king: 
      if (pos->CanColorCastle(GetColor(),true) && pos->IsCastlingPossibleFromPosition(GetColor(), true))
        eml->AddMove(new Move(5,groundrank,7,groundrank));
      if (pos->CanColorCastle(GetColor(),false) && pos->IsCastlingPossibleFromPosition(GetColor(), false)) 
        eml->AddMove(new Move(5,groundrank,3,groundrank));
      maxDistance = 1;
      break;
    case queen: 
      break;
    case rook: 
      start=4;
      break;
    case bishop: 
      end=4;
      break;
    case knight:
      break;
    case pawn:
      maxDistance = 1;
      end=3;
      filedirections[2]=0;
      for (int i=0;i<3;i++)
        rankdirections[i]=GetColor();
      break;
  }
  if (GetType()==knight) {
    for (int i=-2;i<=2;i++) {
      for (int j=-2;j<=2;j++) {
        short newfile=startfile+i;
        short newrank=startrank+j;
        if ((abs(i)+abs(j)) != 3 || newfile > MaxFile || newfile < 1 || newrank > MaxRank || newrank < 1)
          continue;
        
        Move* m = CreateMoveIfPieceCanMoveToField(newfile,newrank,pos);
        if (m)
          eml->AddMove(m);
      }
    }
  }
  else if (GetType() != noPiece) {
    for (int i=start;i<end;i++) {
      int rfile = startfile + filedirections[i];
      int rrank = startrank + rankdirections[i];
      bool free = true;
      int distance=1;
      while (free && rfile <= MaxFile && rfile >= 1 && rrank <= MaxRank && rrank >= 1 && distance <= maxDistance) {
        Move* m = CreateMoveIfPieceCanMoveToField(rfile,rrank,pos);
        if (m) {
          eml->AddMove(m);
          if (m->GetPromotion()) {
            for (int j=rook;j<=knight;j++)
              eml->AddMove(new Move(startfile,startrank,rfile,rrank,m->GetIDofCapturedPiece(),j));
          }
        }
  
        if (pos->GetPieceOnField(rfile,rrank))
          free=false;
        
        rfile += filedirections[i];
        rrank += rankdirections[i];
        distance++;
      }
    }
  }
  return eml;
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
