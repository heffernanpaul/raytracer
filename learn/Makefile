V=0
CXX=clang++

#CXXFLAGS := -std=c++0x $(WXINCLUDES) $(INCLUDES) -MMD
CXXFLAGS := -std=c++11 -DDEBUG -g $(WXINCLUDES) $(INCLUDES) -MMD

all: test

SRCS := $(wildcard *.c*)
OBJS=$(subst .cpp,.o,$(SRCS))

DEPS = $(SRCS:.cpp=.d)

#-include $(DEPS)
test: $(SRCS)
	clang++ $(CXXFLAGS) -o test $(SRCS)

clean:
	$(RM) $(OBJS) $(DEP)


