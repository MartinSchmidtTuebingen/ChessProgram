#include "ReverseMoveStack.h"

ReverseMoveStack::ReverseMoveStack() {
  first = 0x0;
}

ReverseMoveStack::~ReverseMoveStack() {
  Node* last = first;
  while (first) {
    last = first;
    first = first->GetNext();
    delete last;
  }
}

void ReverseMoveStack::AddReverseMove(ReverseMove* rm) {
  Node* node = new Node();
  node->SetReverseMove(rm);
  node->SetNext(first);
  first = node;
}

ReverseMove* ReverseMoveStack::GetReverseMove() {
  ReverseMove* rm = 0x0;
  if (first) {
    Node* old = first;
    first = first->GetNext();
    rm = old->GetReverseMove();
    delete old;
  }
  return rm;
}
