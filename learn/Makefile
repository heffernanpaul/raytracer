V=0
CXX=clang++

CXXFLAGS := -std=c++11 $(WXINCLUDES) $(INCLUDES) -MMD
#CXXFLAGS := -std=c++11 -DDEBUG -g $(WXINCLUDES) $(INCLUDES) -MMD
INCLUDES := -I./Utilities

all: test

SRCS := $(wildcard *.c*)
SRCS += $(wildcard */*.c*)
OBJS=$(subst .cpp,.o,$(SRCS))
INCLUDES := $(addprefix -I, $(wildcard *))

DEPS = $(SRCS:.cpp=.d)

#-include $(DEPS)
test: $(SRCS)
	clang++ $(CXXFLAGS) $(INCLUDES) -o test $(SRCS)

clean:
	$(RM) $(OBJS) $(DEP)


