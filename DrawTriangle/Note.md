## Vertex Buffer

Vertex Buffer is the buffer stored in GPUs VRAM. Then we can call `draw` command to draw a picture with datas in the buffer.  

A shader is a program on the GPU, it support us a method to code on GPU.

We can select the specific buffer and shader to draw the picture.

OpenGL work as s state machine, which means that everything you generate is contextual. That means that each buffer has 
its flows. However, OpenGL assigns an unique unsigned integer identify to each object, like vertex buffers, vertex 
arrays, textures, shaders and so on.

Vertex can contain many stats besides coordinates. It can contain color, normal, speed, texture coordinates and
many other things. Vertex is a whole set of data that makes up a vertex. That's why we need to describe it to GPU.

## Shader

We need to tell the GPU what to do with the data we've sent it and that is what a shader fundamentally is.

### Vertex Shader

Vertex shader is called for each vertex that we're trying to render. A vertex shaders get called for 3 times, for example,
to get the three vertices. The primary purpose of a vertex shader is to tell OpenGL where you want that vertex to be in 
your screen space.

### Fragment/Pixel Shader

Fragment shader runs once for each pixel that needs to be rasterized. The principal purpose of fragment shader is to 
determine what the color of each pixel should be.  
Notice that fragment shader will be executed for many times, but vertex shader only need to be executed for few times.
Thus we'd like to process some critical operations in vertex shader rather than fragment shader.