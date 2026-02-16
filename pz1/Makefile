# Compilers
CC = gcc
CXX = g++

# Flags
CFLAGS = -Wall -Wextra
CXXFLAGS = -Wall -Wextra

# This forces targets to rebuild every timem6
.PHONY: FORCE

# Compile and run C
%: %.c FORCE
	$(CC) $(CFLAGS) -o $@ $<
	./$@

# Compile and run C++
%: %.cpp FORCE
	$(CXX) $(CXXFLAGS) -o $@ $<
	./$@

# Clean compiled files
clean:
	rm -f *.o
