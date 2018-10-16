# OBJ Reader
OBJ files reader, built with OpenGL.

## Build and Run
MinGW's G++ specific libraries were compiled and used on this project. In order to build the runnable .exe, use the following command:
	
	g++ -Wall ./Source/*.cpp -I. -g -lglew32 -lglfw3 -lsoil2 -lopengl32 -lglu32 -lgdi32


After building the .exe, call it by using the following command syntax:

	a.exe -obj <OBJ file name>


Since the OBJ file may contain MTL file(s) reference(s), it is preferrable to keep all scene files on the application's root directory.

## Made with
* C++
* OpenGL
* GLFW
* GLEW
* SOIL

## Development Environment
* [MinGW](http://mingw.org/)
* [VS Code](https://code.visualstudio.com/)

## Authors
* [brunovieira97](https://www.github.com/brunovieira97) - Bruno Vieira
* [christianmlima](https://www.github.com/christianmlima) - Christian Lima