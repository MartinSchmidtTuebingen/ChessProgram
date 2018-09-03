#ifndef REVERSEMOVE_H
#define REVERSEMOVE_H

class Test;
#include "Piece.h"

class ReverseMove {
public:
  //Konstruktur and Destructor
  ReverseMove(short startf = 0, short startr = 0, short targetf = 0, short targetr = 0, short id = 0, short type = noPiece, bool prom = false, bool castlong = true, bool castshort = true, short ep = 0);
  ~ReverseMove();
  short GetStartFile() const { return startfile;};
  short GetStartRank() const { return startrank;};
  short GetTargetFile() const {return targetfile;};
  short GetTargetRank() const {return targetrank;};
  void SetFields(short startf, short startr, short targetf, short targetr);
  short GetIDofCapturedPiece() const {return idcapturedpiece;};
  void SetIDofCapturedPiece(short id) {idcapturedpiece = id;};
  short GetTypeofCapturedPiece() const {return typecapturedpiece;};
  void SetTypeofCapturedPiece(short type) {typecapturedpiece = type;};
  void SetPromotion(bool flag);
  bool GetPromotion() const {return promotion;};
  void SetCastlingLong(bool castlong) {castlinglong = castlong;};
  bool GetCastlingLong() const {return castlinglong;};
  void SetCastlingShort(bool castshort) {castlingshort = castshort;};
  bool GetCastlingShort() const {return castlingshort;};
  void SetEnPassantFile(short ep) {enpassantfile = ep;};
  short GetEnPassantFile() const {return enpassantfile;};
private:
  friend class Test;
  short startfile;
  short startrank;  
  short targetfile;
  short targetrank;
  short idcapturedpiece;
  short typecapturedpiece;
  bool promotion;
  bool castlinglong;
  bool castlingshort;
  short enpassantfile;
};

#endif