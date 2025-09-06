# Written by: Christopher Gholmieh
# Variables (Assignment):

# Include:
COMPILER_INCLUDE = -Iinclude -Iutilities

# Source:
COMPILER_SOURCE = $(wildcard ./source/*.cpp ./source/**/*.cpp)

# Flags:
COMPILER_FLAGS = -Wnull-dereference -fsanitize=undefined,address -Wextra -Werror -Wall -fPIE -pie
LIBNOTIFY_FLAGS = $(shell pkg-config --cflags --libs libnotify)

# Links:
COMPILER_LINKS = -lcrypto -lyaml-cpp


# Engine:
engine: ./scoring-engine.cpp
	g++ $(COMPILER_INCLUDE) $(COMPILER_LINKS) $(COMPILER_SOURCE) $(COMPILER_FLAGS) ./scoring-engine.cpp -o ./build/scoring-engine

# Applicator:
applicator: ./engine-applicator.cpp
	g++ $(COMPILER_INCLUDE) $(COMPILER_LINKS) $(COMPILER_SOURCE) $(COMPILER_FLAGS) ./engine-applicator.cpp -o ./build/engine-applicator

all: ./scoring-engine.cpp ./engine-applicator.cpp
	g++ $(COMPILER_INCLUDE) $(COMPILER_LINKS) $(COMPILER_SOURCE) $(COMPILER_FLAGS) $(LIBNOTIFY_FLAGS) ./scoring-engine.cpp -o ./build/scoring-engine
	g++ $(COMPILER_INCLUDE) $(COMPILER_LINKS) $(COMPILER_SOURCE) $(COMPILER_FLAGS) $(LIBNOTIFY_FLAGS) ./engine-applicator.cpp -o ./build/engine-applicator

clean:
	rm ./build/*