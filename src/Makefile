CXX = g++
CXXFLAGS = -std=c++17

GCOV_FLAGS = --coverage
GTEST_FLAGS = -lgtest -pthread
ALL_FLAGS = $(CXXFLAGS) $(GCOV_FLAGS) $(GTEST_FLAGS)

SRC = model/model.cc
OBJ = $(SRC:.cc=.o)

FILES = model/model.cc controller/controller.cc view/mainwindow.cc view/graph.cc main.cc
HEADERS = model/model.h controller/controller.h view/mainwindow.h view/graph.h

TEST_FILE = tests/tests.cc
TEST_EXEC = tests/test

APP:=$(shell find build -maxdepth 6 -name "SmartCalc")

ifeq ($(shell uname -s),Linux)
	OPEN_CMD = xdg-open
endif

ifeq ($(shell uname -s),Darwin)
	OPEN_CMD = open
endif

all: install

install: clean
	mkdir build
	cd build && cmake ..
	cd build && make

launch:
	./$(APP)

uninstall:	
	rm -rf build/

tests: $(OBJ) $(TEST_FILE) $(SRC)
	$(CXX) $(TEST_FILE) $(OBJ) -o $(TEST_EXEC) $(ALL_FLAGS)
	./$(TEST_EXEC)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

coverage: tests
	lcov -o polish.info -c -d .
	genhtml -o report polish.info
	$(OPEN_CMD) report/index.html

style_check:
	cp ../materials/linters/.clang-format ./
	clang-format -i $(TEST_FILE) $(FILES) $(HEADERS)

dvi:
	doxygen
	$(OPEN_CMD) html/index.html

dist: clean
	cd .. && tar -czvf calc.tar src

clean:
	rm -rf $(TEST_EXEC) $(OBJ) .clang-format
	rm -rf *.gcda *.gcno *.info
	rm -rf html/ build/ report/

.PHONY : install uninstall launch coverage style_check dvi dist clean
