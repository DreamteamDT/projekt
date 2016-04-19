# Makefile för SDL i windows 
# Följ den här för att kunna kompilera i cmd https://www.youtube.com/watch?v=EIywMxRiXAQ
# Skriv mingw32-make för att kompilera i cmd

# Alla C filer 
OBJS = main.c alien.c spaceman.c world.c

# Kompilatorn
CC = gcc

# Peka dit alla .h filer finns på hårddisken (ändra så det passar din dator)
INCLUDE_PATHS = -IC:\min_dl\include\SDL2 -IC:\min_dl\image\include\SDL2 -IC:\min_dl\ttf\include\SDL2 -IC:\min_dl\net\include\SDL2

# Peka dit alla libraries finns på hårddisken (ändra så det passar din dator)
LIBRARY_PATHS = -LC:\min_dl\lib -LC:\min_dl\image\lib -LC:\min_dl\ttf\lib -LC:\min_dl\net\lib

# Kompilator flaggor
COMPILER_FLAGS = -w -std=c99

# Linker flaggor
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net

# namnet för .exe filen
OBJ_NAME = main

# Det som skrivs i cmd, $() översätts till det du skrivit ovanför
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)