#ifndef POSITION_H
#define POSITION_H

class PieceList;
class Piece;
class Move;
class Evaluation;
class Test;
class EvalMoveList;

class Position {
public:
  //Contructors and destructor
  Position(PieceList *white = 0x0, PieceList *black = 0x0, short colortomove = 1, bool whitecancastleshort = false, bool whitecancastlelong = false, bool blackcancastleshort = false, bool blackcancastlelong = false, short enpassantfile = 0);
  ~Position();
  //Getters
  short GetColorToMove() const {return colortomove;};
  void SetColorToMove(short color);
  short GetEnPassantFile() const {return enpassantfile;};
  void SetEnPassantFile(short file) {enpassantfile = file;};
  void SetBoardPointer(Piece *p, short file = 0, short rank = 0);
  void SetBoardPointer(PieceList *pl);
  void CorrectCastling(short color);
  bool CanColorCastle(short color, bool shortside) const;
  void SetColorCastle(short color, bool shortside, bool flag);
  bool IsCastlingPossibleFromPosition(short color, bool shortside) const;
  void CorrectEnPassantFile();
  bool EnPassantPossible() const;
  bool IsEnPassantPossibleOnFile(short file) const;
  //Return a Piece on a certain field. Return null pointer if there is no piece on the field or the searched field is out of the board
  Piece* GetPieceOnField(short searchedfile, short searchedrank) const;
  //Checks if the color which is NOT to move is checked
  bool IsChecked() const;
  //Checks if the specified color is in a check.
  bool IsChecked(short color);
  bool IsMoveLegal(Move* m) const;
  bool IsMovePromotion(Move* m);
  bool IsLegal() const;
  void DeletePieceOnField(short searchedfile, short searchedrank);
  //The following support functions should be private after testing 
  bool DiagonalCheck(short kingfile, short kingrank) const;
  bool CheckDiagonalCheck(short opponenttype, short distance, short colorpawncheck) const;
  bool LineCheck(short kingfile, short kingrank) const;
  bool CheckLineCheck(short opponenttype, short distance) const;
  bool KnightCheck(short kingfile, short kingrank) const;
  void SetUpStartPosition();
  void CreatePiece(Piece *p);
  void CreatePiece(short type, short color, short filenumber, short ranknumber);
  //Capturing a Piece. Should not be visible. Declare as private after testing
  void CapturePiece(Move *m, PieceList *pl);
  //Changing the pieces
  void ExecuteMove(Move *m);
  void WriteOutPosition();
  //Making the list of possible moves in this position
  EvalMoveList* MakeMoveList() const;
private:
  friend class Test;
  PieceList *white;
  PieceList *black;
  Piece* *board; 
  short colortomove;
  bool whitecancastleshort;
  bool whitecancastlelong;
  bool blackcancastleshort;
  bool blackcancastlelong;
  short enpassantfile;
};

#endif