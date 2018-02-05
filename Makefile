# Compiler
CC = clang++
OPTS = -c -Wall -pedantic -std=c++14 -I../aerocore


# Project name
PROJECT = asteroids

all: $(PROJECT)

debug: OPTS += -DDODEBUG -g -O0
debug: $(PROJECT)

release: OPTS += -O2
release: $(PROJECT)

# Directories
OBJDIR = obj
SRCDIR = src

# Libraries
LIBS = -lSDL2 -lSDL2_gfx

# Files and folders
SRCS    = $(shell find $(SRCDIR) -name '*.cpp')
SRCDIRS = $(shell find $(SRCDIR) -type d | sed 's/$(SRCDIR)/./g' )
OBJS    = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Targets
$(PROJECT): buildrepo $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(OPTS) -c $< -o $@

clean:
	rm -rf $(PROJECT) $(OBJDIR)

buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef
