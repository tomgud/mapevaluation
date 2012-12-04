TARGET	:= mapeval
ENDING_SOURCE := cpp
ENDING_HEADER := h
Headers       := $(wildcard *.$(ENDING_HEADER))
Sources       := $(wildcard *.$(ENDING_SOURCE))
Objects       := $(patsubst %.$(ENDING_SOURCE),%.o,$(Sources))

CXXFLAGS = -g -O3 -ffast-math -Wall -Weffc++ -std=c++0x
LDFLAGS = -lm
CCC	= c++
SUF=

DESTPATH = $(TARGET)$(SUF)

all: $(DESTPATH)

.cpp.o:
	$(CCC) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $< 
	
$(DESTPATH): $(Objects)
	$(warning Building...)
	$(CCC) $(CPPFLAGS) $(CXXFLAGS) $(Objects) -o $(DESTPATH) $(LDFLAGS)
	
clean:
	@/bin/rm -f *.o mapeval

realclean: clean
	@$(RM) $(DESTPATH) $(Objects)
