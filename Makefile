CC= gcc
CFLAGS = -g -gdwarf-2 -Wall -std=gnu99
LD= gcc
LDFLAGS = -L.
AR= ar
ARFLAGS = rcs

# Define source files
SRCS= main.c execute.c search.c utilities.c filter.c

# Create an object file list from source files
OBJS= $(SRCS:.c=.o)

# Define the target executable
TARGET= search

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) -lm

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET) test_search.sh
	@echo Testing $<...
	@./test_search.sh

clean:
	@echo Cleaning...
	@rm -f $(TARGET) $(OBJS) *.log *.input

.PHONY: all test clean