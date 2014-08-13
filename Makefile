SRC_DIR := ./src
TEST_SRC_DIR := ./test
SAMPLE_SRC_DIR := ./sample

INCLUDE_DIR := -I./include


BUILD_DIR := ./build
TESTBIN_DIR := $(BUILD_DIR)/test

LIB_DIR := ./lib 
LIB := -ljconer

THIRD_INC_DIR := -I/home/justin/pkg/curl/include
THIRD_LIB_DIR := -L/home/justin/pkg/curl/lib
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

.PHONY:all target test sample $(BUILD_DIR) $(TESTBIN_DIR)
all: test target sample
test: $(TESTBIN_DIR) $(TEST_TARGET) target
target: $(BUILD_DIR) $(OBJ) $(ARCHIVE)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CPP) -c $< $(CFLAG) -o $@ $(INCLUDE_DIR) $(THIRD_INC_DIR)

$(ARCHIVE):$(OBJ)
	$(AR) $(ARFLAG) $@ $(OBJ)

$(TESTBIN_DIR):
	mkdir -p $@

$(TESTBIN_DIR)/%:$(TEST_SRC_DIR)/%.cpp $(OBJ)
	$(CPP) $^ $(CFLAG) $(LFLAG) -o $@ $(INCLUDE_DIR) $(THIRD_INC_DIR) $(THIRD_LIB_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TESTBIN_DIR) $(ARCHIVE)
