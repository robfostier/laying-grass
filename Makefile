# Détection de l'OS
ifeq ($(OS),Windows_NT)
    EXT=.exe
    RM=del /Q
    MKDIR=mkdir
	SEP=\\
else
    EXT=
    RM=rm -f
    MKDIR=mkdir -p
	SEP=/
endif

# Variables
CC=g++
CFLAGS=-Wall -Wextra -Iinclude -std=c++17
LDFLAGS=-lm

SRCDIR=src
OBJDIR=obj
EXECDIR=bin

SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXEC=$(EXECDIR)/devilsplan$(EXT)

# Règles
all: $(EXEC)

$(EXEC): $(OBJS) | $(EXECDIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(EXECDIR):
	$(MKDIR) $(EXECDIR)

clean:
	-$(RM) $(OBJDIR)$(SEP)* $(EXECDIR)$(SEP)*

run: all
	$(EXEC)

.PHONY: all clean run
