SOURCES = src/TitleState.cpp src/main.cpp src/Engine/animation.cpp	\
src/Engine/camera.cpp src/Engine/contactListener.cpp			\
src/Engine/environment.cpp src/Engine/InputFilter.cpp			\
src/Engine/MUGE.cpp

LINUX_CXXFLAGS += `gosu-config --cxxflags` -Iinclude/Box2D/Include -Iinclude -I/usr/local/include -I/opt/local/include
LINUX_LIBS = -lgosu `gosu-config --libs`

OSX_CXXFLAGS += -Iinclude/Box2D/Include -Iinclude -I/usr/local/include	\
-I/opt/local/include -Ilibs/Gosu.framework -framework Gosu
OSX_LIBS =

default:
	@echo "You must choose either target linux or osx: \`make linux\` or \`make osx\`"

.PHONY: linux osx clean

linux:
	g++ ${SOURCES} $(LINUX_LIBS) $(LINUX_CXXFLAGS) -o destructable


osx:
	g++ ${SOURCES} $(OSX_LIBS) $(OSX_CXXFLAGS) -o destructable

clean:
	@rm -f destructable


