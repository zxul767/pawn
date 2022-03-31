## Pawn -- née MAE
## (Motor de Ajedrez Evolutivo, Spanish for Evolutionary Chess Engine)

# -----------------------------------------------------------------------------
# COLORED OUTPUT
# -----------------------------------------------------------------------------
NO_COLOR=\033[0m
OK_COLOR=\033[32;01m
ERROR_COLOR=\033[31;01m
INFO_COLOR=\033[96;01m
WARN_COLOR=\033[33;01m

# -----------------------------------------------------------------------------
# GENERAL COMPILER SETTINGS
# -----------------------------------------------------------------------------
# compiler
CXX = g++
# compiler flags
CXXFLAGS = -g -Wall -Wextra -Werror -O3 -std=c++14
# preprocessor flags
CPPFLAGS =

# where to find header files
INCLUDE_DIR = -I./src
# math and standard c++ libraries to link
LIBS = -lm -lstdc++

# dependency metainformation extension (auto-generated by compiler)
#
# these flags help generate dependency information files as a side-effect of
# compilation. See the man pages of g++ for more information (be sure to see
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/)
#
DEP_EXT = d
DEP_DIR = .$(DEP_EXT)
DEP_FLAGS = -MT $@ -MMD -MF $(DEP_DIR)/$*.Td

# -----------------------------------------------------------------------------
# PROJECT SETTINGS
# -----------------------------------------------------------------------------
# source files extension
SOURCE_EXT = cpp
HEADER_EXT = hpp
# name (without extension) of the main file
PROJECT = main

# name of the main executable
MAIN_BIN_NAME = pawn

# directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# source files
NON_MAIN_SOURCES = $(shell find $(SRC_DIR) -name '*.$(SOURCE_EXT)' | grep -v $(PROJECT).$(SOURCE_EXT))
SOURCES = $(shell find $(SRC_DIR) -name '*.$(SOURCE_EXT)')
SRC_DIRS = $(shell find $(SRC_DIR) -type d | sed 's/$(SRC_DIR)/./g' )
# object files
OBJS = $(patsubst $(SRC_DIR)/%.$(SOURCE_EXT), $(OBJ_DIR)/%.o, $(SOURCES))
NON_MAIN_OBJS = $(patsubst $(SRC_DIR)/%.$(SOURCE_EXT), $(OBJ_DIR)/%.o, $(NON_MAIN_SOURCES))
# dependency metadata files
DEPENDENCIES = $(patsubst $(SRC_DIR)/%.$(SOURCE_EXT), $(DEP_DIR)/%.$(DEP_EXT), $(SOURCES))

# -----------------------------------------------------------------------------
# PATTERN RULES
# -----------------------------------------------------------------------------
# create a pattern rule with an empty recipe, so that Make won’t fail if the
# dependency file doesn’t exist
$(DEP_DIR)/%.$(DEP_EXT): ;

# COMPILATION
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(SOURCE_EXT) $(DEP_DIR)/%.$(DEP_EXT) | ensure_folders
	@echo "$(INFO_COLOR)Compiling $<...$(NO_COLOR)"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEP_FLAGS) -c $< -o $@
	$(POSTCOMPILE)

# LINKING
$(BIN_DIR)/$(PROJECT): $(OBJS)
	@echo "$(INFO_COLOR)Linking main executable $@...$(NO_COLOR)"
	$(CXX) $(OBJS) $(LIBS) -o $@
	cp $@ $(BIN_DIR)/$(MAIN_BIN_NAME)
	@echo "$(INFO_COLOR)Main executable is at $(OK_COLOR)$(BIN_DIR)/$(MAIN_BIN_NAME)$(NO_COLOR)"

# ensure that dependency files don't get corrupted if compilation ever fails
POSTCOMPILE = mv -f $(DEP_DIR)/$*.T$(DEP_EXT) $(DEP_DIR)/$*.$(DEP_EXT)

# -----------------------------------------------------------------------------
# UNIT TESTING
# -----------------------------------------------------------------------------
UNIT_TEST_PROJECT = $(MAIN_BIN_NAME)_unit_test

UNIT_TEST_INCLUDE_DIR = $(INCLUDE_DIR)
UNIT_TEST_LIBS = $(LIBS)

UNIT_TEST_DIR = test/unit
UNIT_TEST_SRC_DIR = $(UNIT_TEST_DIR)/$(SRC_DIR)
UNIT_TEST_OBJ_DIR = $(UNIT_TEST_DIR)/$(OBJ_DIR)
UNIT_TEST_BIN_DIR = $(UNIT_TEST_DIR)/$(BIN_DIR)

# source files
UNIT_TEST_SOURCES = $(shell find $(UNIT_TEST_SRC_DIR) -name '*.$(SOURCE_EXT)')
# object files
UNIT_TEST_OBJS = $(patsubst $(UNIT_TEST_SRC_DIR)/%.$(SOURCE_EXT), $(UNIT_TEST_OBJ_DIR)/%.o, $(UNIT_TEST_SOURCES))

# compilation
$(UNIT_TEST_OBJ_DIR)/%.o: $(UNIT_TEST_SRC_DIR)/%.$(SOURCE_EXT) $(DEP_DIR)/%.$(DEP_EXT) | ensure_folders
	@echo "$(INFO_COLOR)Compiling unit test $<...$(NO_COLOR)"
	$(CXX) $(UNIT_TEST_INCLUDE_DIR) $(CPPFLAGS) $(CXXFLAGS) $(DEP_FLAGS) -c $< -o $@
	$(POSTCOMPILE)

# linking
$(UNIT_TEST_BIN_DIR)/$(UNIT_TEST_PROJECT): $(UNIT_TEST_OBJS) $(NON_MAIN_OBJS)
	@echo "$(INFO_COLOR)Linking main unit test runner $@...$(NO_COLOR)"
	$(CXX) $(UNIT_TEST_OBJS) $(NON_MAIN_OBJS) $(UNIT_TEST_LIBS) -o $@

# -----------------------------------------------------------------------------
# PERFORMANCE TESTING
# -----------------------------------------------------------------------------
PERF_TEST_PROJECT = $(MAIN_BIN_NAME)_perf_test

PERF_TEST_INCLUDE_DIR = $(INCLUDE_DIR)
PERF_TEST_LIBS = $(LIBS)

PERF_TEST_DIR = test/perf
PERF_TEST_SRC_DIR = $(PERF_TEST_DIR)/$(SRC_DIR)
PERF_TEST_OBJ_DIR = $(PERF_TEST_DIR)/$(OBJ_DIR)
PERF_TEST_BIN_DIR = $(PERF_TEST_DIR)/$(BIN_DIR)

# source files
PERF_TEST_SOURCES = $(shell find $(PERF_TEST_SRC_DIR) -name '*.$(SOURCE_EXT)')
# object files
PERF_TEST_OBJS = $(patsubst $(PERF_TEST_SRC_DIR)/%.$(SOURCE_EXT), $(PERF_TEST_OBJ_DIR)/%.o, $(PERF_TEST_SOURCES))

# COMPILATION
$(PERF_TEST_OBJ_DIR)/%.o: $(PERF_TEST_SRC_DIR)/%.$(SOURCE_EXT) $(DEP_DIR)/%.$(DEP_EXT) | ensure_folders
	@echo "$(INFO_COLOR)Compiling performance test $<...$(NO_COLOR)"
	$(CXX) $(PERF_TEST_INCLUDE_DIR) $(CPPFLAGS) $(CXXFLAGS) $(DEP_FLAGS) -c $< -o $@
	$(POSTCOMPILE)

# LINKING
$(PERF_TEST_BIN_DIR)/$(PERF_TEST_PROJECT): $(PERF_TEST_OBJS) $(NON_MAIN_OBJS)
	@echo "$(INFO_COLOR)Linking main performance test runner $@...$(NO_COLOR)"
	$(CXX) $(PERF_TEST_OBJS) $(NON_MAIN_OBJS) $(PERF_TEST_LIBS) -o $@

# -----------------------------------------------------------------------------
# MAKEFILE TARGETS
# -----------------------------------------------------------------------------
# build main executable file
all: $(BIN_DIR)/$(PROJECT) | ensure_folders

rebuild: clean all

perf_test: $(PERF_TEST_BIN_DIR)/$(PERF_TEST_PROJECT) | ensure_folders
	@echo "$(INFO_COLOR)Running performance tests ...$(NO_COLOR)"
	./$(PERF_TEST_BIN_DIR)/$(PERF_TEST_PROJECT)

ifeq ($(COMPACT),true)
  REPORTER = | ./compact_reporter.py
else
  REPORTER =
endif

unit_test: $(UNIT_TEST_BIN_DIR)/$(UNIT_TEST_PROJECT) | ensure_folders
	@echo "$(INFO_COLOR)Running unit tests ...$(NO_COLOR)"
	@./$(UNIT_TEST_BIN_DIR)/$(UNIT_TEST_PROJECT) --success $(REPORTER)

ensure_folders:
	@$(call create-folders)

# mark the dependency files as "precious" to make, so they won’t be automatically deleted
# as intermediate files.
.PRECIOUS: $(DEP_DIR)/%.$(DEP_EXT)

# -----------------------------------------------------------------------------
# PHONY TARGETS
# -----------------------------------------------------------------------------
.PHONY: distclean clean clean-backups tarball

# TARBALL DISTRIBUTION
TARBALL_TEMP_DIR = "$(PROJECT)_tarball_`date +%F-%H-%M`"
TARBALL_EXTRA_FILES = Makefile initial.in README.md

tarball: clean Makefile initial.in
	@mkdir -p $(TARBALL_TEMP_DIR)
	@cp -rf --parents $(TARBALL_EXTRA_FILES) $(SRC_DIR)/*.$(SOURCE_EXT) \
	$(SRC_DIR)/*.$(HEADER_EXT) $(TARBALL_TEMP_DIR)

	@find $(TARBALL_TEMP_DIR) -printf "%P\n" \
	| tar --create --gzip --file $(MAIN_BIN_NAME)_`date +%F-%H-%M`.tar.gz \
	--no-recursion --directory $(TARBALL_TEMP_DIR) --files-from=-

	@rm -rf $(TARBALL_TEMP_DIR)
	@echo "$(INFO_COLOR)Created $(MAIN_BIN_NAME)_`date +%F-%H-%M`.tar.gz$(NO_COLOR)"

# CLEANING
clean: clean-backups
	@echo "$(WARN_COLOR)Cleaning intermediate object files and metadata...$(NO_COLOR)"
	rm -rf $(OBJ_DIR) $(UNIT_TEST_OBJ_DIR) $(PERF_TEST_OBJ_DIR) $(DEP_DIR)

distclean: clean
	@echo "$(WARN_COLOR)Cleaning binaries...$(NO_COLOR)"
	rm -rf $(BIN_DIR) $(UNIT_TEST_BIN_DIR) $(PERF_TEST_BIN_DIR)

clean-backups:
	@echo "$(WARN_COLOR)Cleaning backups...$(NO_COLOR)"
	find . -name "*~" -type f -print0 | xargs -0 rm -f

# FUNCTIONS
define create-folders
	mkdir -p $(BIN_DIR)
	mkdir -p $(DEP_DIR)
	mkdir -p $(UNIT_TEST_OBJ_DIR) $(UNIT_TEST_BIN_DIR)
	mkdir -p $(PERF_TEST_OBJ_DIR) $(PERF_TEST_BIN_DIR)

	mkdir -p $(OBJ_DIR)
	for dir in $(SRC_DIRS); \
	do \
		mkdir -p $(OBJ_DIR)/$$dir; \
	done
endef

# DEPENDENCIES
# dependencies for each source file are automatically generated by the compiler
# (see DEP_FLAGS above)
-include ${DEPENDENCIES}
