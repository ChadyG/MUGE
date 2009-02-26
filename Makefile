NAME = destructible
CC = g++
SOURCES = $(wildcard src/*.cpp) $(wildcard src/Engine/*.cpp)
OBJ := $(patsubst %.cpp, %.o, $(wildcard src/*.cpp)) $(patsubst %.cpp, %.o, $(wildcard src/Engine/*.cpp))

LINUX_CXXFLAGS += `gosu-config --cxxflags` -Iinclude/Box2D/Include -Iinclude -I/usr/local/include -I/opt/local/include
LINUX_LIBS = -lgosu `gosu-config --libs` -lboost_signals

OSX_CXXFLAGS += -Iinclude/Box2D/Include -Iinclude -I/usr/local/include/boost-1_37	\
-I/opt/local/include -Ilibs/Gosu.framework/Headers -framework Gosu -L/opt/local/lib/
OSX_LIBS = -lboost_signals-mt

CXXFLAGS =
LIBS = 

default:
	@echo "You must choose either target linux or osx: \`make linux\` or \`make osx\`"

.PHONY: linux osx gosu clean

# targets for Linux
linux: CXXFLAGS = $(LINUX_CXXFLAGS) 
linux: LIBS = $(LINUX_LIBS)
linux: $(NAME) 

# targets for osx
gosu: 
	cp -rf libs/Gosu.framework /Library/Frameworks/
osx: CXXFLAGS = $(OSX_CXXFLAGS) 
osx: LIBS = $(OSX_LIBS)
osx: gosu $(NAME)

# general targets
$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LIBS)

%.o: %.cpp	
	$(CC) -c $< $(CXXFLAGS) -o $@

clean:
	@rm -f destructible $(OBJ)

