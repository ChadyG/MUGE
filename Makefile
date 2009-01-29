SOURCES = src/TitleState.cpp src/main.cpp src/Engine/animation.cpp	\
src/Engine/camera.cpp src/Engine/contactListener.cpp			\
src/Engine/environment.cpp src/Engine/InputFilter.cpp			\
src/Engine/MUGE.cpp
CXXFLAGS += `gosu-config --cxxflags` -Iinclude/Box2D/Include -Iinclude
LIBS = -lgosu `gosu-config --libs`

destructable:
	g++ ${SOURCES} $(LIBS) $(CXXFLAGS) -o destructable
