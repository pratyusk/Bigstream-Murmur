# the compiler: g++
CC = g++

# flags: C++11 support + GDB + treat warnings as errors
CFLAGS = -std=c++0x -g -Wall -Werror

# executable: test case
TARGET = testMurmur

default: all

# make all
all: $(TARGET)

# g++ command
$(TARGET): MurmurHash3.o $(TARGET).cpp
	$(CC) $(CFLAGS) MurmurHash3.o -o $(TARGET) $(TARGET).cpp
	chmod +x $(TARGET)

# required object files
MurmurHash3.o: MurmurHash3.cpp MurmurHash3.h
	$(CC) $(CFLAGS) -c MurmurHash3.cpp

# clean executables
clean:
	$(RM) $(TARGET) *.o