C_VERSION=c99
LIBS=-lglfw -ldl -lm
SRC_FILES=src/*.c

all: clean build run

clean:
	rm -rf ./bin

build:
	mkdir -p ./bin
	clang -std=${C_VERSION} -Wall ${LIBS} -o ./bin/playground $(SRC_FILES)

run:
	./bin/playground
