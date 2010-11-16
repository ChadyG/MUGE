Library Versions
==========
Gosu  0.7.19
Box2D 2.1.2

Engine Changelog
==========
Nov 15 2010
-------------------
angle to TransformComponent and RenderComponent reads now
filter data into shape defs in level file (categoryBits, maskBits, groupIndex)
box2D attributes all properly cased as they are in box2D

older
-------------------
moved to Box2D 2.1.2
updated input manager to include reset fields function
	called from Core::update in stack dirty
	
Starting from Scratch
==========
The engine should build just fine using VS 2010 or 2009.
On your first build, you will need to copy some files into the build folder (if you know a way to do this from within VS, please tell me!)
Copy the Data/, Images/, and Sound/ folders and fmod.dll from lib/ into your Debug/ and Release/ folders.  Now you can run the builds
without problems.