#ifndef PIECE_H
#define PIECE_H

class Evaluation;
class Move;
class Test;
class EvalMoveList;
class Position;
class ReverseMove;

const short noPiece = 0;
const short king = 1;
const short queen = 2;
const short rook = 3;
const short bishop = 4;
const short knight = 5;
const short pawn = 6;

class Piece {
public:
  //Konstruktur and Destructor
  Piece(short type, short color, short file, short rank);
  Piece(const Piece *p); //Copys p exactly, including the ID
  ~Piece();
  //Getters
  short GetID() const {return ID;};
  short GetType() const {return type;};
  short GetFile() const {return file;};
  void SetFile(short f) {file = f;};
  short GetRank() const {return rank;};
  void SetRank(short r) {rank = r;};
  short GetColor() const {return color;};
  //A check if the piece is on a given field
  short IsOnField(short searchedfile, short searchedrank) const;
  //A check if the piece is the piece the function is looking for
  bool IsPiece(short searchedID) const {return ID==searchedID;};
  //Returns the value of the piece. Constants given in the Evaluation class
  float GetValue(const Evaluation *eval = 0x0) const;
  //Move the piece according to the Move m
  void MovePiece(Move *m);
  Piece* RetractMove(ReverseMove *rm);
  Move* CreateMoveIfPieceCanMoveToField(short targetfile, short targetrank, Position* pos);
  //Returns a pointer to a list of moves which can be done by this piece
  EvalMoveList* MakeMoveList(Position* pos);
  void WriteOutPiece();
 
private:
  friend class Test;
  //Type: Any from 1 to 6 (K,Q,R,B,N,P)
  short type;
  //color: blackNumber for Black and whiteNumber for White (defined in Chess.h). Easier to handle and remember than boolean
  short color;
  //The file- and ranknumbers of the piece
  short file;
  short rank;
  //An ID which is unique in the actual play
  short ID;
  void SetID(short nid) {ID = nid;};
  void SetType(short ty) {type = ty;};
};

#endif