CXX=g++

CXXFLAGS=-Iinclude -ISFML-2.5.1/include -std=c++17
LDFLAGS=-Wl,-rpath=SFML-2.5.1/lib
LIBS=-LSFML-2.5.1/lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

IDIR=include
SDIR=src
ODIR=obj
LDIR=lib

SOURCES:=$(wildcard $(SDIR)/*.cc)	
OBJECTS:=$(SOURCES:$(SDIR)/%.cc=$(ODIR)/%.o)
INCLUDES:=$(wildcard $(IDIR)/*.h)
# $(info  SOURCE is $(SOURCE))
# $(info  OBJ is $(OBJ))

less: $(OBJECTS) ${INCLUDES}
	$(CXX) -o $@ ${OBJECTS} $(CXXFLAGS) ${LDFLAGS} $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.cc ${INCLUDES} | ${ODIR}
	$(CXX) -o $@ $(CXXFLAGS) -c $<

${ODIR}:
	@mkdir -p ${ODIR}

.PHONY: clean ${ODIR}

clean:
	rm -rf ${ODIR} less