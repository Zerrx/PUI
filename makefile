#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C++ compiler to use
CC = g++

# define any compile-time flags
CFLAGS = -Wall

# TODO: Learn how to include files in executables and create installers! Currently, we are using iexpress.exe
# Auxiliary Application MAJOR (i.e. unnamed and required) files (INTRODUCTIONS, FORMS, et cetera)
#AUXFILES = auxfiles\INTRODUCTIONS

# define any directories containing header files other than /usr/include
#
INCLUDES = -Iinclude

# define the C source files
#SRCS = emitter.c error.c init.c lexer.c main.c symbol.c parser.c
# define the C+ source files
SRCS = categories\construction\Construction.cpp categories\construction\StairStringer\StairStringer.cpp categories\sporting\bodybuilding\Bodybuilding.cpp categories\sporting\bodybuilding\CreateNewWeeklyDirectory.cpp categories\sporting\bodybuilding\CreateShorthandRoutine.cpp lib\ConfigurationFiles\conf.cpp lib\ConfigurationFiles\ConfigurationFile.cpp lib\DateTime\DateTime.cpp lib\File\FileIO.cpp lib\File\FileSystem.cpp lib\Form\FormControls.cpp lib\Interface\Interface.cpp PUI.cpp lib\Math\PUIMath.cpp lib\PUI\PUINamespace.cpp lib\ShellControls\ShellControls.cpp lib\ShellControls\WindowsConsole.cpp lib\Writing\Writing.cpp

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
OBJS = $(SRCS:.cpp=.o)

# define the executable file 
MAIN = PUI.exe

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all: $(MAIN)
	@echo  PUI.exe has been compiled!

# To make and include Resources\resource.o, do the following in msys:
#		windres resource.rc resource.o

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS)


# this is a suffix replacement rule for building .o's from .cpp's
# it uses automatic variables $<: the name of the prerequisite of
# the rule (a .cpp file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

#clean:
#	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it