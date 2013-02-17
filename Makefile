################################################################################
# IPK 1
################################################################################

################################################################################
# promenne prekladu


NAME = main
CPP  = g++
CC   = g++
SCFLAGS = -g -pedantic -Wall -W -O2 -DINLINE
INCS =
CXXINCS =
CXXFLAGS = -Wall $(SCFLAGS)
LOGIN = Tuxilero
TARBALL = Makefile main.cpp
#CFLAGS = $(INCS)  -fmessage-length=0

.PHONY: gzip
.PHONY: setLd
.PHONY: clear
.PHONY: clean
.PHONY: clnobj


################################################################################
# obecny cil kompilace
all: $(NAME)

################################################################################
# linkovani vysledneho souboru
main: main.o
	$(CC) -o $@ main.o


################################################################################
# kompilace hlavniho souboru
main.o: main.cpp
	$(CC) -c main.cpp -o main.o $(CXXFLAGS)

################################################################################
# kompilace modulu

# dynamicka knihovna

################################################################################
# cil vycisteni souboru prekladu
clean: clear
clear:
	rm -f *.o $(NAME) $(LIBS) $(LOGIN).tar

clnobj:
	rm -f *.o

setLD:
	export LD_LIBRARY_PATH="."

$(LOGIN).tar: $(TARBALL)
	tar -cf $(LOGIN).tar $(TARBALL)
	
gzip: $(LOGIN).tar
#	gzip $(LOGIN).tar

################################################################################
################################################################################
