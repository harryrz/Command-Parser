SRCS = parser.cpp Shape.cpp 
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

parser: $(OBJS)
	g++ -std=c++11 $^ -o $@
%.o:	%.cpp
	g++ -std=c++11 -c -g $< -o $@
clean:
	rm -f *.o  parser core

