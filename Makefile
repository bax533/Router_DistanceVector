TARGET = router

OBJS = Common.o \
	Manager.o

CXX = g++
CXXFLAGS = -std=c++17 
DFLAGS = -ggdb -fsanitize=address -fsanitize=undefined
RFLAGS = -DNDEBUG

all: $(TARGET)

release: CXXFLAGS += $(RFLAGS)
release: $(TARGET)

debug: CXXFLAGS += $(DFLAGS)
debug: $(TARGET)

$(TARGET): $(OBJS) main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o

