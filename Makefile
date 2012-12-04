CC      = gcc
CXX     = g++
TARGET	= mapeval
CCFLAGS = -ffast-math -std=c++0x
CXXFLAGS = -ffast-math -std=c++0x
VISIBILITY = 

INCLUDES =  -I../ \
		 -I./ \
		 -ITiles \
		 -IMap \
		 -ICore \
		 -IIO \
		 -ISearch \
		 -IEvolution \

DEFINES = -DLINUX

OBJECTS  = mapeval.o \
	Core/player_manager.o \
	Core/player.o \
	Evolution/evolution.o \
	Evolution/fitness.o \
	Evolution/genome.o  \
	Evolution/ind_grade.o \
	IO/civ5mapreader.o \
	IO/output_writer.o \
	Map/coords.o \
	Map/civ5map.o \
	Map/feature.o \
	Map/feature_pos.o \
	Map/resource.o \
	Map/resource_pos.o \
	Map/map.o \
	Map/tile_block.o \
	Map/yield.o \
	Search/a_star.o \
	Search/search_tile.o \
	Search/tile_comparator.o \
	Tiles/coast.o \
	Tiles/desert.o \
	Tiles/grassland.o \
	Tiles/hill.o \
	Tiles/mountain.o \
	Tiles/ocean.o \
	Tiles/plains.o \
	Tiles/snow.o \
	Tiles/tile.o \
	Tiles/tundra.o \

BIN_DIR_ROOT=bin
BIN_DIR = $(BIN_DIR_ROOT)

debug: BIN_DIR = $(BIN_DIR_ROOT)/debug
debug: CCFLAGS += -g3 -O0 -Wall -Weffc++ 
debug: CXXFLAGS += -g3 -O0 -Wall -Weffc++ 
debug: DEFINES += -DDEBUG
debug: $(TARGET)

release: BIN_DIR = $(BIN_DIR_ROOT)/release
release: CCFLAGS += -O3
release: CXXFLAGS += -O3
release: DEFINES += -DNDEBUG
release: $(TARGET)

####### Build rules
$(TARGET): $(OBJECTS) 
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(OBJECTS) -o $(BIN_DIR)/$(TARGET) $(SHAREDLIBS) $(STATICLIBS)

####### Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

clean: 
	rm -f $(OBJECTS) $(TARGET) core
