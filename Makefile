## Makefile for MAE (Motor de Ajedrez Evolutivo)
## August 27, 2007

CPP = g++
CXXFLAGS = -I./headers -g -Wall -Werror -O2 -std=c++11
LD = -lm
OBJECTS = mae.o \
	common.o \
	FileReader.o MaeBoard.o Board.o Record.o \
	SimpleEvaluator.o \
	Move.o SimpleMoveGenerator.o \
	Command.o CommandReader.o CommandExecuter.o \
	AlphaBetaSearch.o MinimaxSearch.o Timer.o Dictionary.o \
	Pawn.o Rook.o Knight.o Bishop.o Queen.o King.o Piece.o \
	Chromosome.o FitnessEvaluator.o GeneticAlgorithm.o

TEST_EXEC = test_piece test_timer
MAIN_EXEC = mae
TARBALL_NAME = MaE

# WHERE TO FIND FILES BASED ON EXTENSION
vpath %.h ./headers
vpath %.cc ./code

# IMPLICIT RULES TO BUILD TARGETS
%.o : %.cc
	$(CPP) $(CXXFLAGS) -c $<

% : %.o
	$(CPP) $(CXXFLAGS) $^ -o $@ $(LD)

all: $(MAIN_EXEC)

# MAIN EXECUTABLE
$(MAIN_EXEC) : $(OBJECTS)

test_piece : test_piece.o \
	Board.o FileReader.o \
	Move.o \
	common.o \
	Pawn.o Rook.o Knight.o Bishop.o Queen.o King.o

test_timer : test_timer.o \
	Timer.o \
	common.o

test: test.o common.o

# TEST FILES
mae.o : mae.cc \
	common.h \
	MaeBoard.h Board.h Square.h BoardStatus.h \
	Piece.h \
	Move.h MoveGenerator.h \
	Command.h CommandReader.h CommandExecuter.h \
	Search.h AlphaBetaSearch.h MinimaxSearch.h Timer.h Dictionary.h

test_piece.o : test_piece.cc \
	common.h \
	Board.h Square.h BoardStatus.h \
	Piece.h Pawn.h Rook.h Knight.h Bishop.h Queen.h King.h \
	Move.h

test_timer.o : test_timer.cc \
	Timer.h \
	common.h

test.o : test.cc common.h

# USEFUL TO VARIOUS MODULES
common.o : common.cc common.h


# USER & XBOARD COMMUNICATION
Command.o : Command.cc Command.h \
	common.h \
	Board.h \
	Move.h \
	Piece.h

CommandReader.o : CommandReader.cc CommandReader.h \
	common.h \
	Command.h \
	Board.h \
	Piece.h \
	Move.h

CommandExecuter.o : CommandExecuter.cc CommandExecuter.h \
	common.h \
	Command.h \
	Board.h \
	Piece.h \
	Timer.h \
	MoveGenerator.h SimpleMoveGenerator.h \
	Search.h \
	GeneticAlgorithm.h FitnessEvaluator.h Chromosome.h

# SEARCH MODULE
Timer.o : Timer.cc Timer.h \
	common.h

AlphaBetaSearch.o : AlphaBetaSearch.cc AlphaBetaSearch.h \
	common.h \
	Search.h \
	Board.h \
	Move.h \
	Piece.h \
	MoveGenerator.h \
	PositionEvaluator.h \
	Dictionary.h

MinimaxSearch.o : MinimaxSearch.cc MinimaxSearch.h \
	common.h \
	Search.h \
	Board.h \
	Move.h \
	Piece.h \
	MoveGenerator.h \
	PositionEvaluator.h

SimpleEvaluator.o : SimpleEvaluator.cc SimpleEvaluator.h \
	common.h \
	PositionEvaluator.h \
	Board.h \
	Piece.h

Dictionary.o : Dictionary.cc Dictionary.h \
	common.h \
	Board.h

# BOARD REPRESENTATION MODULE
Record.o : Record.cc Record.h \
	common.h

Board.o : Board.cc Board.h \
	common.h \
	Piece.h

MaeBoard.o : MaeBoard.cc MaeBoard.h Board.h \
	common.h \
	Square.h BoardStatus.h Record.h \
	Piece.h Pawn.h Knight.h Bishop.h Rook.h King.h Queen.h \
	Move.h \
	FileReader.h

FileReader.o : FileReader.cc FileReader.h \
	common.h \
	Board.h	\
	Piece.h

# MOVE GENERATION
Move.o : Move.cc Move.h \
	common.h \
	Piece.h \
	Board.h

SimpleMoveGenerator.o : SimpleMoveGenerator.cc SimpleMoveGenerator.h \
	common.h \
	Move.h MoveGenerator.h \
	Piece.h \
	Board.h

Pawn.o : Pawn.cc Pawn.h \
	common.h \
	Piece.h \
	Board.h

Rook.o : Rook.cc Rook.h \
	common.h \
	Piece.h \
	Board.h

Bishop.o : Bishop.cc Bishop.h \
	common.h \
	Piece.h \
	Board.h

Knight.o : Knight.cc Knight.h \
	common.h \
	Piece.h \
	Board.h

King.o : King.cc King.h \
	common.h \
	Piece.h \
	Board.h

Queen.o : Queen.cc Queen.h \
	common.h \
	Piece.h Rook.h Bishop.h \
	Board.h

Piece.o : Piece.cc Piece.h

# GENETIC ALGORITHMS MODULE
Chromosome.o : Chromosome.h Chromosome.cc \
	common.h \
	Board.h Piece.h \

FitnessEvaluator.o : FitnessEvaluator.h FitnessEvaluator.cc \
	Chromosome.h \
	Search.h \

GeneticAlgorithm.o : GeneticAlgorithm.h GeneticAlgorithm.cc \
	common.h \
	Chromosome.h \
	FitnessEvaluator.h

# TARBALL DISTRIBUTION
$(TARBALL_NAME) : clean Makefile initial.in
	mkdir mae
	cp -r Makefile initial.in t1 t2 t3 t4 games/ code/ headers/ mae/
	tar -czvf $(TARBALL_NAME)_`date +%F`.tar.gz mae/
	rm -rf mae/

# PHONY TARGETS
.PHONY: clean

clean : clean-backups
	-rm -f *.o $(MAIN_EXEC) $(TEST_EXEC)

clean-backups :
	-find . -name "*~" -type f -print0 | xargs -0 rm -f
