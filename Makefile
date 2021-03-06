# the compiler to use.
CC = g++

# compiler compilation options.
CFLAGS = -std=c++11 -Wall -Wextra

# libraries to link against.
LFLAGS = -lmingw32 -lws2_32

# the path to object files and executable.
BUILD_PATH = build

# the path to source files.
SRC_PATH = .

# a set of source files from the source file folder.
SRC = $(wildcard $(SRC_PATH)/*.cpp)

# a set of object files based on the resolved source files.
OBJ = $(SRC:$(SRC_PATH)/%.cpp=$(BUILD_PATH)/%.o)

# rule to compile from source to object files.
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

# rule to compile the executable.
all: $(OBJ)
	$(CC) -o $(BUILD_PATH)/test.exe $(OBJ) $(CFLAGS) $(LFLAGS)
