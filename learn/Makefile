V=0
UNAME := $(shell uname)

INCLUDES := -Isrc/Utilities -Isrc/Cameras -Isrc/Tracers -Isrc/World -Isrc/BRDFs
INCLUDES += -Isrc -Isrc/GeometricObjects -Isrc/Materials -Isrc/Lights -Isrc/Samplers

ifeq ($(UNAME), Linux)
CXXFLAGS := -std=c++0x $(WXINCLUDES) $(INCLUDES) -MMD
CXX=g++
else
#CXXFLAGS := -std=c++11 $(WXINCLUDES) $(INCLUDES) -MMD
#CXXFLAGS := -std=c++11 -DDEBUG -g $(WXINCLUDES) $(INCLUDES) -MMD -o
CXXFLAGS := -std=c++11 -DDEBUG -g $(WXINCLUDES) $(INCLUDES) -MMD -Ofast -msse3
#CXXFLAGS := -std=c++11 -DDEBUG -g $(WXINCLUDES) $(INCLUDES) -MMD
CXX=clang++
endif

all: test

SRCS := $(wildcard src/*.c*)
SRCS += $(wildcard src/*/*.c*)
OBJS=$(subst .cpp,.o,$(SRCS))

DEPS = $(SRCS:.cpp=.d)

#-include $(DEPS)
#test: $(SRCS)
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -o test $(SRCS)

test: $(OBJS)
	clang++ $(CXXFLAGS) $(LIBRARIES) -o test $(OBJS)

clean:
	$(RM) $(OBJS) $(DEP)


