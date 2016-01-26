## Makefile for MAE (Motor de Ajedrez Evolutivo, Spanish for Evolutionary Chess Engine)

# COMPILER
CPP = g++
CXXFLAGS = -g -Wall -Werror -O2 -std=c++11
LD = -lm

# PROJECT NAME
PROJECT = mae
TARBALL_TEMP_DIR = $(PROJECT)_tarball

# DIRECTORIES
SRCDIR = src
OBJDIR = bin

# LIBRARIES
LIBS =

# FILES AND FOLDERS
SRCS = $(shell find $(SRCDIR) -name '*.cc')
SRCDIRS = $(shell find $(SRCDIR) -type d | sed 's/$(SRCDIR)/./g' )
OBJS = $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.o, $(SRCS))

# TARGETS
all: $(PROJECT)

$(PROJECT): ensure_repo $(OBJS)
	$(CPP) $(OBJS) $(LIBS) $(LD) -o $@

ensure_repo:
	@$(call create-repo)

# CREATE OBJ DIRECTORY STRUCTURE
define create-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef

# IMPLICIT RULES TO BUILD TARGETS
$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(CPP) $(CXXFLAGS) -c $< -o $@

# PHONY TARGETS
.PHONY: clean clean-backups directories tarball

# DEPENDENCIES
# TODO: create these rules automatically from the source files

# MAIN
mae.o : mae.cc \
	common.h \
	MaeBoard.h Board.h Square.h BoardStatus.h \
	Piece.h \
	Move.h MoveGenerator.h \
	Command.h CommandReader.h CommandExecuter.h \
	Search.h AlphaBetaSearch.h MinimaxSearch.h Timer.h Dictionary.h

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

# GENETIC ALGORITHM MODULE
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
tarball : clean Makefile initial.in
	mkdir -p $(TARBALL_TEMP_DIR)
	cp -rf Makefile initial.in README.md src/ $(TARBALL_TEMP_DIR)
	tar -czvf $(PROJECT)_`date +%F-%H-%M-%S`.tar.gz $(TARBALL_TEMP_DIR)
	rm -rf $(TARBALL_TEMP_DIR)

# CLEANING
clean : clean-backups
	rm -Rf $(PROJECT) $(OBJDIR)

clean-backups :
	find . -name "*~" -type f -print0 | xargs -0 rm -f
