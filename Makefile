OBJS = Chess.o Piece.o PieceList.o Move.o ReverseMove.o Position.o SupportFunctions.o Evaluation.o Test.o GameManager.o EvalMove.o ChessGame.o EvalMoveList.o ReverseMoveStack.o 

Chess.out:  $(OBJS)
	g++ -o Chess.out $(OBJS) 

SupportFunctions.o : SupportFunctions.cpp SupportFunctions.h
	g++ -c SupportFunctions.cpp -std=gnu++11	
	
Move.o : Move.cpp Move.h Piece.h SupportFunctions.h

ReverseMove.o : ReverseMove.cpp ReverseMove.h Piece.h

Piece.o : Piece.cpp Piece.h Move.h Evaluation.h Chess.h EvalMoveList.h Position.h SupportFunctions.h 

PieceList.o : PieceList.cpp PieceList.h Piece.h Move.h Evaluation.h Chess.h EvalMoveList.h Position.h

Position.o : Position.cpp Position.h Piece.h PieceList.h Move.h ReverseMove.h SupportFunctions.h Chess.h EvalMoveList.h

Evaluation.o : Evaluation.cpp Evaluation.h Position.h PieceList.h

EvalMove.o : EvalMove.cpp EvalMove.h

EvalMoveList.o : EvalMoveList.cpp EvalMoveList.h EvalMove.h

ReverseMoveStack.o : ReverseMoveStack.cpp ReverseMoveStack.h 

ChessGame.o : ChessGame.cpp ChessGame.h EvalMove.h Evaluation.h Chess.h EvalMoveList.h Position.h Move.h ReverseMove.h

GameManager.o : GameManager.cpp GameManager.h Position.h Move.h SupportFunctions.h EvalMove.o ChessGame.o

Chess.o : Chess.cpp Chess.h Piece.h PieceList.h Move.h Evaluation.h Position.h GameManager.h

Test.o : Test.cpp Test.h Chess.h Evaluation.h Move.h EvalMove.h EvalMoveList.h ReverseMove.h ReverseMoveStack.h Piece.h PieceList.h Position.h SupportFunctions.h ChessGame.h


