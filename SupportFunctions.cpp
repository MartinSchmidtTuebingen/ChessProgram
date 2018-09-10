#include <random>
#include "SupportFunctions.h"
#include <string>
#include <sstream>
using namespace std;

#include "Piece.h"
#include "Chess.h"

int RandomNumber(int maxnumber) {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(1,maxnumber);
  return dist(mt);
}

short NextID() {
  static short int ID = 0;
  ID++;
  return ID;
}

bool OutOfBound(short searchedfile, short searchedrank) {
  return (searchedfile < 1 || searchedfile > MaxFile || searchedrank < 1 || searchedrank > MaxRank);
}

string GetColor(short color) {
  if (color == whiteNumber) 
    return "White";
  if (color == blackNumber)
    return "Black";
  
  return "";
}

string GetField(short file, short rank) {
  return GetFile(file)+GetRank(rank);
}

string GetRank(short rank) {
  stringstream rankstream;
  rankstream << rank;
  return rankstream.str();
}

string GetFile(short file) {
  switch (file) {
    case 1:
      return "a";
    case 2:
      return "b";
    case 3:
      return "c";
    case 4:
      return "d";
    case 5:
      return "e";
    case 6:
      return "f";
    case 7:
      return "g";
    case 8:
      return "h";  
    default:
      return "";
  }
}
  
string GetPieceShortName(short type) {
  switch(type) {
    case king:
      return "K";
    case queen:
      return "Q";
    case rook:
      return "R";
    case bishop:
      return "B";
    case knight:
      return "N";
    default:
      return "";
  }
}

short GetTypeFromString(string input) {
  if (input.length() == 2) {
    return pawn;
  }
  else if (input.length() == 3) {
    string::iterator i;
    i = input.begin();
    string s;
    s = *i;
    return GetTypeFromSingleString(s);
  }
}

short GetTypeFromSingleString(string input) {
    if (input== "K" || input == "k") {
      return king;
    }
    else if (input== "Q" || input == "q" || input == "D" || input == "d") {
      return queen;
    }
    else if (input== "R" || input == "r" || input == "T" || input == "t") {
      return rook;
    }
    else if (input== "B" || input == "b" || input == "B" || input == "b") {
      return bishop;
    }
    else if (input== "N" || input == "n" || input == "S" || input == "s") {
      return knight;
    }
    else {
      return noPiece;
    }
}  

short GetFileFromString(string input) {
  string::iterator i;
  i = input.end();
  i = i-2;
  string s;
  s = *i;
  return GetFileFromSingleString(s);
}

short GetFileFromSingleString(string input) {
  if (input == "a" || input == "A") {
    return 1;
  }
  else if (input == "b" || input == "B" ){
    return 2;
  }
  else if (input == "c" || input == "C" ){
    return 3;
  }
  else if (input == "d" || input == "D" ){
    return 4;
  }
  else if (input == "e" || input == "E" ){
    return 5;
  }
  else if (input == "f" || input == "F" ){
    return 6;
  }
  else if (input == "g" || input == "G" ){
    return 7;
  }
  else if (input == "h" || input == "H" ){
    return 8;
  }  
  else
    return 0;
}  

short GetRankFromString(string input) {
  string::iterator i;
  i = input.end();
  i = i-1;
  string s;
  s = *i;  
  return GetRankFromSingleString(s);
}

short GetRankFromSingleString(string input) {
  if ( input == "1") {
    return 1;
  }
  else if ( input == "2") {
    return 2;
  }
  else if ( input == "3") {
    return 3;
  }
  else if ( input == "4") {
    return 4;
  }
  else if ( input == "5") {
    return 5;
  }
  else if ( input == "6") {
    return 6;
  }
  else if ( input == "7") {
    return 7;
  }
  else if ( input == "8") {
    return 8;
  }  
  else
    return 0;  
}  