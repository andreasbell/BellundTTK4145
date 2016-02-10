# Which compiler to use
CC = clang-3.6

# Compiler flags go here.
CFLAGS = -std=c11 -g -Wall -Wextra
CPPFLAGS = -std=c++11 -g -Wall -Wextra

# Linker flags go here.
LDFLAGS = -lcomedi -lm

# list of sources
ELEVSRC = elev.c io.c *.c

# program executable file name.
TARGET = elevator

# top-level rule, to compile everything.
all: $(TARGET)

# Define all object files.
ELEVOBJC = $(ELEVSRC:.c=.o) 
#ELEVOBJCPP = $(ELEVSRC:.cpp=.o)

# rule to link the program
$(TARGET): $(ELEVOBJC) #$(ELEVOBJCPP)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compile: create object files from C source files.
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

# Compile: create object files from C source files.
#%.o : %.c
#	$(CC) $(CPPFLAGS) -c $< -o $@ 

# rule for cleaning re-compilable files.
clean:
	rm -f $(TARGET) $(ELEVOBJC)

rebuild:	clean all

.PHONY: all rebuild clean