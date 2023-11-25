# Common Variables

CURRENT_DIRECTORY	= $(shell pwd)
SOURCE_DIRECTORY	= $(CURRENT_DIRECTORY)
BINARY_PATH			= $(shell dirname $(SOURCE_DIRECTORY))/binaries/biq
CXX					= clang++
CXX_FLAGS			= -O2 -std=gnu++11 -fno-rtti -fno-exceptions -Wno-sign-compare -Wno-format-security -Wno-narrowing -D_FILE_OFFSET_BITS=64
DEBUG_FLAGS			= -g3 -DBIQ_DEBUG=1
INCLUDES			= -I$(SOURCE_DIRECTORY)/
LIBS				=
STRIP				= @true
STRIP_EXE			= strip
ARCH				= x86_64

# Common Objects

OBJECTS	=	$(SOURCE_DIRECTORY)/Main.o \
			$(SOURCE_DIRECTORY)/Engine/Engine.o \
			$(SOURCE_DIRECTORY)/Engine/Renderer.o \
			$(SOURCE_DIRECTORY)/Engine/Sound.o \
			$(SOURCE_DIRECTORY)/Engine/World.o \
			$(SOURCE_DIRECTORY)/Game/Splash.o \
			$(SOURCE_DIRECTORY)/Game/InGame.o

# Linux Variables

LINUX_CXX		= clang++
LINUX_CXX_FLAGS	=
LINUX_INCLUDES	=
LINUX_LIBS		= -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LINUX_OBJECTS	=

# Windows Variables

WINDOWS_CXX			= x86_64-w64-mingw32-g++
WINDOWS_CXX_FLAGS	= -static-libgcc -static-libstdc++
WINDOWS_INCLUDES	=
WINDOWS_LIBS		= -lm -lmingw32 -lwinmm -lpthread -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
WINDOWS_STRIP		= x86_64-w64-mingw32-strip
WINDOWS_OBJECTS		=

# "Final" Variables

ifndef TARGET
	TARGET = linux
endif

ifndef TYPE
	TYPE = debug
endif

ifeq ($(TARGET), linux)
	CXX			= $(LINUX_CXX)
	CXX_FLAGS	+= $(LINUX_CXX_FLAGS)
	INCLUDES	+= $(LINUX_INCLUDES)
	LIBS		+= $(LINUX_LIBS)
	OBJECTS		+= $(LINUX_OBJECTS)
endif

ifeq ($(TARGET), windows)
	ARCH		:= $(ARCH).exe
	CXX_FLAGS	+= $(WINDOWS_CXX_FLAGS)
	INCLUDES	+= $(WINDOWS_INCLUDES)
	LIBS		+= $(WINDOWS_LIBS)
	OBJECTS		+= $(WINDOWS_OBJECTS)
	CXX			= $(WINDOWS_CXX)
	STRIP_EXE	= $(WINDOWS_STRIP)
endif


ifeq ($(TYPE), debug)
	CXX_FLAGS	+= $(DEBUG_FLAGS)
	ARCH		:= debug.$(ARCH)
endif

ifeq ($(TYPE), release)
	STRIP = $(STRIP_EXE) -s
endif

# Targets

%.o: %.cxx
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $< -o $@

all: $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(OBJECTS) $(LIBS) -o $(BINARY_PATH).$(ARCH)
	$(STRIP) $(BINARY_PATH).$(ARCH)

clean:
	find $(SOURCE_DIRECTORY)/ -type f -iname "*.o" -exec rm -v {} \;

help:
	@echo ""
	@echo "Usage: make TARGET=<target name> TYPE=<debug|release>"
	@echo ""
	@echo "Available targets:"
	@echo " - linux"
	@echo " - windows"
	@echo ""
	@echo "Defaults to: linux, debug"