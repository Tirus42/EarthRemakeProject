# Earth Remake Project

This project was started to recreate a game very similar to the existing Earth 2150 alias "The Moon Project" game.

A first try was made in the game-programming language "BlitzBasic 3D" but stopped due to the limits of the language and the graphics engine. This code is not contained in this repository but was adapted into this project.

After a while, another try was started using the programming language C++ and the "Irrlicht Engine" for model and texture loading and rendering.

In the current state of development some basic features are available:
* Map loading using a hightmap image and waymap generation based on the height difference
* Map generation using noise functions
* Several implementations of path finding algorithms (A*, Bread-first search and Jump point search) including performance tests
* A very simple implementation of unit movement based on the map and the path search result
* Some preperations for building placement
* Some parts of a multiplayer implementation allowing to move units together
* A basic GUI using the Irrlicht Engine

The render engine is completly based on the Irrlicht Engine using OpenGL with several shaders.
Implemented features includes:
* Map rendering using deferred shading to perform light computation
* Point light test code to test the deferred shading
* Overlay rendering to show the waymap and the path a unit is taking
* Very simple day/night circle

For content loading (models, textures, effects, ...) there was the idea to load this data from the base game using a own .wd loader and extractor for the specific files.
There is a working implementation of a texture extractor but none for the .wd files yet.