# Makefile

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

# Links all of the executables together
all: png2txt txt2png cs220_paint

# Links together files needed to create png2text executable
png2txt: png2txt.o pnglite.o cs220_paint.o
	$(CC) -o png2txt png2txt.o pnglite.o cs220_paint.o -lz -lm

# Links together files needed to create txt2png executable
txt2png: txt2png.o pnglite.o cs220_paint.o
	$(CC) -o txt2png txt2png.o pnglite.o cs220_paint.o -lz -lm

# Links together files needed to create cs220_paint                                    
cs220_paint: cs220_paint.o driver.o
	$(CC) -o cs220_paint cs220_paint.o driver.o -lm

# Creates object files
png2txt.o: png2txt.c cs220_paint.h
	$(CC) $(CFLAGS) -c png2txt.c

txt2png.o: txt2png.c cs220_paint.h
	$(CC) $(CLFAGS) -c txt2png.c

pnglite.o: pnglite.c pnglite.h
	$(CC) $(CFLAGS) -c pnglite.c

cs220_paint.o: cs220_paint.c cs220_paint.h
	$(CC) $(CFLAGS) -c cs220_paint.c

driver.o: driver.c cs220_paint.h
	$(CC) $(CFLAGS) -c driver.c


# Removes all object files and the executables
# so we can start fresh
clean:
	rm -f *.o png2txt txt2png cs220_paint
