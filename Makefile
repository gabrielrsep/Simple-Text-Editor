LIBS=-lncurses
HEADERS=headers
SOURCE=sources
OUT=out

SOURCE_FILES=${wildcard ${SOURCE}/*.cpp}
O_FILES=${patsubst $(SOURCE)/%.cpp, $(OUT)/%.o, ${SOURCE_FILES}}

FLAGS=-g -Wall

screen: ${O_FILES} screen.cpp
	g++ ${FLAGS} screen.cpp ${LIBS} -o $@ ${O_FILES} -I${HEADERS}

${OUT}/%.o: ${SOURCE}/%.cpp
	g++ ${FLAGS} -c $< -o $@

clean:
	rm ${OUT}/* screen