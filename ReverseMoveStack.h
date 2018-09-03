#ifndef REVERSEMOVESTACK_H
#define REVERSEMOVESTACK_H

class ReverseMove;

class Node {
public:
  Node(ReverseMove* revm = 0x0, Node* nnext = 0x0) {rm = revm; next = nnext;};
  ~Node() {};
  ReverseMove* GetReverseMove() const {return rm;};
  void SetReverseMove(ReverseMove* revm) {rm = revm;};
  Node* GetNext() const {return next;};
  void SetNext(Node* nnext) {next = nnext;};
private:
  friend class Test;
  ReverseMove* rm;
  Node* next;
};

class ReverseMoveStack {
public:
  //Constructors and destructor
  ReverseMoveStack();
  ~ReverseMoveStack();
  ReverseMove* GetReverseMove();
  void AddReverseMove(ReverseMove* rm);
private:
  friend class Test;
  Node* first;
};



#endif