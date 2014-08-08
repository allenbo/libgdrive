SRC_DIR := ./src
TEST_SRC_DIR := ./test
INCLUDE_DIR := ./include
BUILD_DIR := ./build
TESTBIN_DIR := $(BUILD_DIR)/test
LIB_DIR := ./lib 

LIB := -ljconer

CPP := g++
CC := gcc
AR := ar

CFLAG := -O2
LFLAG := -O2 -lcurl -L$(LIB_DIR) $(LIB) -flto
ARFLAG := -rcs

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst %.cpp,%.o, $(subst $(SRC_DIR),$(BUILD_DIR), $(SRC)))
TEST_SRC := $(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_TARGET := $(patsubst %.cpp, %, $(subst $(TEST_SRC_DIR),$(TESTBIN_DIR), $(TEST_SRC)))

ARCHIVE := libgdrive.a

.PHONY:all target test $(BUILD_DIR) $(TESTBIN_DIR)
all: test target
test: $(TESTBIN_DIR) $(TEST_TARGET) target
target: $(BUILD_DIR) $(OBJ) $(ARCHIVE)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CPP) -c $< $(CFLAG) -o $@ -I$(INCLUDE_DIR)

$(ARCHIVE):$(OBJ)
	$(AR) $(ARFLAG) $@ $(OBJ)

$(TESTBIN_DIR):
	mkdir -p $@

$(TESTBIN_DIR)/%:$(TEST_SRC_DIR)/%.cpp $(OBJ)
	$(CPP) $^ $(CFLAG) $(LFLAG) -o $@ -I$(INCLUDE_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TESTBIN_DIR) $(ARCHIVE)
