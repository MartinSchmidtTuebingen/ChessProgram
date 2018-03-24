#ifndef MOVE_H
#define MOVE_H

class Test;

class Move {
public:
  //Konstruktur and Destructor
  Move(short startfile, short startrank, short targetfile, short targetrank, short idcapturedpiece = 0, short promotion = 0);
  ~Move();
  short GetStartFile() const { return startfile; };
  short GetStartRank() const { return startrank; };
  short GetTargetFile() const {return targetfile;};
  short GetTargetRank() const {return targetrank;};
  short GetIDofCapturedPiece() const {return idcapturedpiece;};
  void SetIDofCapturedPiece(short ID) {idcapturedpiece = ID;};
  void SetPromotion(short flag);
  short GetPromotion() const {return promotion;};
private:
  friend class Test;
  short startfile;
  short startrank;  
  short targetfile;
  short targetrank;
  //Set to 0 if no piece has been captured
  short idcapturedpiece;
  //Set to 0 if there is no promotion. Any other type denotes the type of figure the pawn is promoted to
  short promotion;
  //Sets the ID of the castled rook
};

#endif