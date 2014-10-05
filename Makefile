CC = gcc
CXX = g++

# C Flags
CFLAGS = -O2 -Wall -march=native -Wno-unknown-pragmas -ggdb
# C++ Flags
CXXFLAGS = $(CFLAGS)

LDFLAGS = -lm

# use special MPI/FFTW libs
SRCS_C = $(wildcard *.c)
SRCS_CXX = $(wildcard *.cpp)
SRCS = $(SRCS_C) $(SRCS_CXX)
OBJS_C = $(SRCS_C:%.c=%.o)
OBJS_CXX = $(SRCS_CXX:%.cpp=%.o)
OBJS = $(OBJS_C) $(OBJS_CXX)
HDRS = $(wildcard *.h)
TEMP = $(SRCS:%=%~) $(HDRS:%=%~)

EXENAME = dcf77gen

$(EXENAME): $(OBJS)
	$(CXX) $(OBJS) -o $(EXENAME) $(LDFLAGS)

$(OBJS): $(HDRS) Makefile

main.o: $(HDRS) Makefile $(SRCS_C) $(SRCS_CXX)

.PHONY: clean

clean:
	rm -f $(OBJS) $(TEMP)

%.o : %.c
	$(CC) $*.c -c $(CFLAGS)

%.o : %.cpp
	$(CXX) $*.cpp -c $(CXXFLAGS)