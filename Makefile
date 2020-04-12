CXX=g++

CXXFLAGS=-Iinclude -ISFML-2.5.1/include -std=c++17
LDFLAGS=-Wl,-rpath=SFML-2.5.1/lib
LIBS=-LSFML-2.5.1/lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

IDIR=include/
SDIR=src/
ODIR=src/
LDIR=lib/

SOURCES:=$(wildcard $(SDIR)*.cc)	
OBJECTS:=$(SOURCES:$(SDIR)%.cc=$(ODIR)%.o)
INCLUDES:=$(wildcard $(IDIR)*.h)
# $(info  SOURCE is $(SOURCE))
# $(info  OBJ is $(OBJ))


less: $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS) ${LDFLAGS} $(LIBS)

$(ODIR)main.o: $(SDIR)main.cc $(INCLUDES)

$(ODIR)ai.o: $(IDIR)ai.h $(IDIR)game.h

$(ODIR)block.o: $(IDIR)block.h\
			$(IDIR)wall.h\
			$(IDIR)const_globals.h

$(ODIR)game.o: $(IDIR)game.h\
			$(IDIR)const_globals.h\
			$(IDIR)helpers.h

$(ODIR)gui.o: $(IDIR)gui.h\
			$(IDIR)const_globals.h\
			$(IDIR)block.h\
			$(IDIR)wall.h\
			$(IDIR)helpers.h

$(ODIR)helpers.o: $(IDIR)helpers.h\
			$(IDIR)const_globals.h\

$(ODIR)less.o: $(IDIR)less.h\
			$(IDIR)game.h\
			$(IDIR)const_globals.h\
			$(IDIR)helpers.h

$(ODIR)player.o: $(IDIR)player.h\
			$(IDIR)const_globals.h\
			$(IDIR)helpers.h

$(ODIR)wall.o: $(IDIR)wall.h\
			$(IDIR)const_globals.h

.PHONY: clean

clean:
	rm $(ODIR)*.o