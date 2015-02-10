V=0
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CXXFLAGS := -std=c++0x $(WXINCLUDES) $(INCLUDES) -MMD
CXX=g++
else
CXXFLAGS := -std=c++11 $(WXINCLUDES) $(INCLUDES) -MMD
CXX=clang++
endif


#CXXFLAGS := -std=c++11 -DDEBUG -g $(WXINCLUDES) $(INCLUDES) -MMD
INCLUDES := -I./Utilities

all: test

SRCS := $(wildcard *.c*)
SRCS += $(wildcard */*.c*)
OBJS=$(subst .cpp,.o,$(SRCS))

DEPS = $(SRCS:.cpp=.d)

#-include $(DEPS)
test: $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o test $(SRCS)

clean:
	$(RM) $(OBJS) $(DEP)


