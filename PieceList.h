#ifndef PIECELIST_H
#define PIECELIST_H

#include "Piece.h"
class Move;
class Evaluation;
class Test;

class PieceList {
public:
  //Constructors and destructor
  PieceList();
  PieceList(Piece *np, PieceList *nnext = 0x0, bool nowner = false);
  PieceList(short piecetype, short piececolor, short piecefilenumber, short pieceranknumber, PieceList *nnext, bool nowner = false);
  PieceList(const PieceList *pl);
  PieceList(const PieceList& copy);
  ~PieceList();
  //Getters and Setters
  PieceList* GetNext() const {return next;};
  short GetType() const {p->GetType();};
  void SetNext(PieceList *nnext) {next = nnext;};
  Piece* GetPiece() const {return p;};
  bool GetOwner() const {return owner; };
  void SetOwner(bool flag) {owner = flag; if(next) next->SetOwner(flag);}
  //Changing the List 
  void ClearList();
  void CreatePiece(Piece *np);
  void CreatePiece(short type, short color, short filenumber, short ranknumber);
  void SetUpStartPosition(short color);
  //Other Functions
  bool IsPiece(short ID) const;
  short GetType(short ID) const;
  Piece* FindPiece(short ID) const;
  short IsAnyPieceOnField(short searchedfile, short searchedrank) const;
  Piece* GetPieceOnField(short searchedfile, short searchedrank) const;
  Piece *GetKing() const;
  void MovePiece(Move *m);
  void DeletePiece(Move *m); 
  void DeletePiece(short ID);  
  float GetValuePieces(const Evaluation *eval = 0x0) const;
  EvalMoveList* MakeMoveList(const Position* const p) const;
  void WriteOutPieces() const;
private:
  friend class Test;
  Piece *p;
  PieceList *next;
  bool owner;
};

#endif