NAME = destructible
CC = g++
SOURCES = $(wildcard src/*.cpp) $(wildcard src/Engine/*.cpp) 
BOX2D_SOURCES = $(wildcard include/Box2D/Source/Collision/*.cpp) $(wildcard include/Box2D/Source/Collision/Shapes/*.cpp) $(wildcard include/Box2D/Source/Common/*.cpp) $(wildcard include/Box2D/Source/Dynamics/*.cpp) $(wildcard include/Box2D/Source/Dynamics/Joints/*.cpp) $(wildcard include/Box2D/Source/Dynamics/Contacts/*.cpp)
HEADERS = $(wildcard src/*.h) $(wildcard src/*.hpp) $(wildcard src/Engine/*.h) $(wildcard src/Engine/*.hpp)
OBJ := $(patsubst %.cpp, %.o, $(SOURCES))

LINUX_CXXFLAGS += `gosu-config --cxxflags` -Iinclude/Box2D/Include -Iinclude -I/usr/local/include -I/opt/local/include
LINUX_LIBS = -lgosu `gosu-config --libs` -lboost_signals

OSX_CXXFLAGS += -Iinclude/Box2D/Include -Iinclude -I/usr/local/include/boost-1_37	\
-I/opt/local/include -Ilibs/Gosu.framework
OSX_LIBS = -lboost_signals-mt -L/opt/local/lib/ -framework Gosu -Llibs/ -lBox2D

CXXFLAGS =
LIBS = 

default:
	@echo "You must choose either target linux or osx: \`make linux\` or \`make osx\`"

.PHONY: linux osx gosu clean

# targets for Linux
linux: CXXFLAGS = $(LINUX_CXXFLAGS) 
linux: LIBS = $(LINUX_LIBS)
linux: SOURCES += $(BOX2D_SOURCES)
linux: OBJ += $(patsubst %.cpp, %.o, $(BOX2D_SOURCES)) # update for the addition to SOURCES
linux: $(NAME) 

# targets for osx
gosu: 
	cp -rf libs/libBox2D.dylib ./
	cp -rf libs/Gosu.framework /Library/Frameworks/	
osx: CXXFLAGS = $(OSX_CXXFLAGS) 
osx: LIBS = $(OSX_LIBS)
osx: gosu $(NAME)

# general targets
$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LIBS)

%.o: %.cpp
	$(CC) -c $< $(CXXFLAGS) -o $@

%.h:
%.hpp:

clean:
	@rm -f destructible $(OBJ)

