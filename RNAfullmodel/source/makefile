PROJECT = project

OBJCASH = arithmetic.o basic.o color.o filter.o io.o logical.o \
margolus.o movie.o neighbors.o noise.o png.o ps.o random.o shift.o\
x11.o

OBJCASH2 = cash2.o mersenne.o 

OBJMAIN = main.o foldcash.o para.o annotmol.o rna_string.o fold_wrapper_lib.o genotype.o g-observer.o test.o phenotype.o p-observer.o color-manager.o plot_grace.o observer.o display.o observer_wrapper.o

#CC = /home/nobuto/nobuto2/applications/bin/gcc
#CPP = gfilt
CC = /usr/bin/gcc
CPP = /usr/bin/g++

#CCOPTION = -feliminate-unused-debug-types -fdefer-pop -foptimize-sibling-calls -fcse-follow-jumps -fcse-skip-blocks -fexpensive-optimizations -fthread-jumps -fstrength-reduce -fpeephole -fforce-mem -ffunction-cse -fkeep-static-consts -fcaller-saves -fpcc-struct-return -fgcse -fgcse-lm -fgcse-sm -frerun-cse-after-loop -frerun-loop-opt -fdelete-null-pointer-checks -fschedule-insns2 -fsched-interblock -fsched-spec -fbranch-count-reg -freorder-blocks -frename-registers -fcprop-registers -fcommon -fgnu-linker -fregmove -foptimize-register-move -fargument-alias -fstrict-aliasing -fmerge-constants -fident -fpeephole2 -fguess-branch-probability -fmath-errno -ftrapping-math -m80387 -mhard-float -mno-soft-float -mieee-fp -mfp-ret-in-387 -mcpu=i386 -march=i386 -Wall # Safety but somehow slow.


CCOPT = -O0 -ggdb -Wall # For debugging.
#CCOPT = -O0 -ggdb -Wall -lefence # For memory check.
#CCOPT = -O3 -pg -Wall# For profiling.
#CCOPT =  -O3 -S -fverbose-asm -Wall # something else
#CCOPT = -v -Q -O3 # something else
#CCOPT = -O3 -Wall # Most optimized.

LDIR = -L/usr/X11R6/lib
LIBS = -lpng -lz -lX11 -lm -lgrace_np -lRNA -fopenmp
#IDIR = -I/usr/include/ViennaRNA
IDIR = -I/home/sandro/Downloads/ViennaRNA-2.4.10/src/ViennaRNA

all: $(OBJCASH) $(OBJCASH2) $(OBJMAIN) source.tar.gz
	$(CPP) $(OBJCASH) $(OBJCASH2) $(OBJMAIN) $(CCOPT) -o $(PROJECT) $(LIBS) $(LDIR)


$(OBJCASH): cash.h  makefile
$(OBJCASH2): cash2.h mersenne.h  makefile
main.o: my.h cash.h cash2.h mersenne.h makefile foldcash.h phenotree.h constants.h para.h rna_string.h annotmol.h
foldcash.o: foldcash.h makefile cash2.h
para.o: para.h constants.h makefile
fold_bintree.o: fold_bintree.h constants.h makefile
annotmol.o: annotmol.h cash2.h constants.h para.h makefile fold_wrapper_lib.h
rna_string.o: rna_string.h constants.h makefile

fold_wrapper_lib.o: fold_wrapper_lib.h rna_fold_lib.hpp makefile constants.h para.h
rna_fold_lib.o: rna_fold_lib.hpp makefile constants.h para.h
genotype.o: genotype.hpp makefile
g-observer.o: g-observer.hpp makefile genotype.hpp p-observer.hpp
test.o: makefile g-observer.hpp p-observer.hpp
phenotype.o: makefile color-manager.hpp rna_fold_lib.hpp
p-observer.o: makefile phenotype.hpp p-observer.hpp color-manager.hpp
color-manager.o: makefile color-manager.hpp para.h
plot_grace.o: makefile plot_grace.hpp para.h
observer.o: makefile observer.hpp p-observer.hpp g-observer.hpp plot_grace.hpp para.h cash.h cash2.h
display.o: makefile display.hpp para.h
observer_wrapper.o: makefile observer.hpp cash2.h
source.tar.gz: $(OBJCASH) $(OBJCASH2) $(OBJMAIN)
	tar -zcf source.tar.gz *.c *.h *.cpp *.hpp makefile

.cpp.o:
	$(CPP) -c $(CCOPT) $(CPPOPT) $(IDIR) $<
.c.o:
	$(CC) -c $(CCOPT) $(IDIR) $<
