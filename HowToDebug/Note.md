
Basicly, we have two mainly ways to check the errors in OpenGL:
1. `glGetError`: It is compatible with all versions. Once we call OpenGL functions and there are errors occurred, a sign
   will be set in OpenGL cache, which indicates what kind of the error is. When we call `glGetError`, we will get the code
   of the error.  
   This approach is a polling strategy that lets us know if and what kind of errors have occurred at any time.
2. `glDebugMessageCallback`, it's added in OpenGL 4.3, and it allows us to specify a function pointer to OpenGL and OpenGL 
   will callback when an error occurs in this function. This approach is much better. The other advantage of it is that
   it offers you more detailed messages instead of error codes.