V=0
CXX=/usr/bin/g++


LIBRARIES := -L/System/Library/Frameworks/OpenGL.framework/Libraries -L/usr/local/lib -g -O2
LIBRARIES += -framework Cocoa -framework OpenGL -framework GLUT -framework IOKit
LIBRARIES +=  -framework CoreVideo /usr/local/lib/libglfw3.a /usr/local/lib/libSOIL.a /usr/lib/libglew.a -framework AGL
LIBRARIES +=  -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio /usr/local/lib/libassimp.dylib
INCLUDES := -I/usr/include -I/usr/local/include $(addprefix -I, $(wildcard raytracer/*))
INCLUDES +=  $(addprefix -I, $(wildcard raytracer/GeometricObjects/*))

WXLIBS := -L/usr/local/lib   -framework IOKit -framework Carbon -framework Cocoa -framework AudioToolbox -framework System -framework OpenGL /usr/local/lib/libwx_osx_cocoau_xrc-3.0.a /usr/local/lib/libwx_osx_cocoau_webview-3.0.a /usr/local/lib/libwx_osx_cocoau_qa-3.0.a /usr/local/lib/libwx_baseu_net-3.0.a /usr/local/lib/libwx_osx_cocoau_html-3.0.a /usr/local/lib/libwx_osx_cocoau_adv-3.0.a /usr/local/lib/libwx_osx_cocoau_core-3.0.a /usr/local/lib/libwx_baseu_xml-3.0.a /usr/local/lib/libwx_baseu-3.0.a -framework WebKit -lexpat -lwxregexu-3.0 -lwxtiff-3.0 -lwxjpeg-3.0 -lwxpng-3.0 -lz -lpthread -liconv -llzma

LIBRARIES += $(WXLIBS)
LDLIBS := $(LIBRARIES)
WXINCLUDES := -I/usr/local/lib/wx/include/osx_cocoa-unicode-static-3.0 -I/usr/local/include/wx-3.0 -D_FILE_OFFSET_BITS=64 -DwxDEBUG_LEVEL=0 -D__WXMAC__ -D__WXOSX__ -D__WXOSX_COCOA__
#CXXFLAGS := -std=c++0x $(WXINCLUDES) $(INCLUDES) -MMD
CXXFLAGS := -std=c++0x -DDEBUG -g $(WXINCLUDES) $(INCLUDES) -MMD

all: wxraytracer

SRCS := $(wildcard raytracer/*/*.c*)
SRCS += $(wildcard raytracer/*/*/*.c*)
SRCS := $(filter-out raytracer/build/BuildShadedObjects.cpp, $(SRCS))
OBJS=$(subst .cpp,.o,$(SRCS))

DEPS = $(SRCS:.cpp=.d)

#-include $(DEPS)




wxraytracer: $(OBJS)
	g++ $(CXXFLAGS) $(LIBRARIES) -o wxraytracer $(OBJS)

clean:
	$(RM) $(OBJS) $(DEP)

dist-clean: clean
	$(RM) texture


