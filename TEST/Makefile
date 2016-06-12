# Generated automatically from Makefile.in by configure.
# $Id: Makefile,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $

prefix       = /home/cbilderback
host_os      = linux
srcdir       = .
top_srcdir   = .
enable_debug = no

# Set up the paths we'll be needing
INCPATHS = -I.. -I/home/cbilderback/Include
LIBDIRS = -L..

# Libraries to link in
LIBS = -lMathLib # -umalloc -lefence

# Linker flags
LDFLAGS = $(LIBDIRS)
LOADLIBES = $(LIBS)

# Set up the DEBUG flag
ifeq ($(enable_debug),yes)
DEBUG = -g
else
DEBUG = -O2
endif

# Compiler defs
CC = gcc
CXX = g++ -fpermissive
CXXFLAGS = $(DEBUG) $(INCPATHS)


SRCS=	interpolatortest.cpp

OBJS=$(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp

# Suffix rules
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o test1 $(LIBDIRS) $(LIBS)


clean::
	rm -f test* core *~

realclean: clean
	rm -rf $(OBJS)



