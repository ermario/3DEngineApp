# MADAEngine

This is a Game Engine under construction. At the current stage we are able to load a model .fbx with its own texture using Assimp for the meshes and DevIL for textures. The Engine runs on OpenGL3 and uses SDL and glew.

GitHub: https://github.com/ermario/3DEngineApp

# HOW THE ENGINE WORK

Besides from the model that is loaded by default, we can move around with the camera:

 - Left mouse click for camera look around rotation + WASD for moving
 - Q and E for absolute height of the camera
 - F for focusing the object 
 - Left ALT + Left Click for orbiting the object.
 - Arrows for camera rotation
 - Mouse wheel for Zoom in/out

 We also have some menu options:
  - Camera information
  - Hardware information
  - On File option from menu you can QUIT the Engine. by doing this you get a popup module asking to quit or not.


#SOMETHING TO KNOW ABOUT THE CODE:

I've used Visual Studio 2019 Community edition. The code is not refactored, time was short in my personal case and I barely implemented a few features. Is not an excuse is a fact. I know I can do it a lot better with a bit of time and organization. For sure there are a lot of mistakes and room for improvement.
	 
# LICENSE

MIT License

Copyright (c) 2021 ermario

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.