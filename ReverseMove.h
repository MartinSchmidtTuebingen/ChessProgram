#ifndef REVERSEMOVE_H
#define REVERSEMOVE_H

class Test;

class ReverseMove {
public:
  //Konstruktur and Destructor
  ReverseMove(short startfile, short startrank, short targetfile, short targetrank, short idcapturedpiece = 0, short promotion = 0);
  ~ReverseMove();
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
  short idcapturedpiece;
  bool 
  bool promotion;
};

#endif