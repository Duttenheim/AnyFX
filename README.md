AnyFX
=====

AnyFX is an effects library which will use a unified programming API for handling shaders implemented in any underlying shading language.

AnyFX is a programming language, but also not that it's not a shading language by itself. AnyFX operates by defining what one could call a padding, around which actual HLSL/GLSL shader code is written. The language specification can be found in the repository and is called 'anyfx spec.pdf'.

AnyFX also uses CMake to setup the project, and is fully implemented in C/C++. 

AnyFX uses the following libraries, all of which follows either the BSD or GNU lesser general license:

mcpp
antlr3c
glew

The AnyFX demo project (which is optional) also uses:

DevIL
glm


AnyFX can be setup in different ways. When running CMake, you will see four variables. These are:

ANYFX_BUILD_COMPILER_LIBRARY
ANYFX_BUILD_DEMO
ANYFX_BUILD_GRAMMAR
ANYFX_TRANSPOSE_MATRICES

The ANYFX_BUILD_COMPILER_LIBRARY will switch if the compiler should build as its own application, or be a library which you can link to your own application and use. When using the compiler as a library, you simply include the contained anyfxcompiler.h in your software.

The ANYFX_BUILD_DEMO will build an included demo project, which features a set of default GLSL shaders as well as an AnyFX example called demo.fx, which utilizes most AnyFX features.

The ANYFX_BUILD_GRAMMAR will detect any changes in the AnyFX.g ANTLR grammar file, and will trigger a rebuild of the entire ANTLR lexer and parser. Switch this to ON if you wish to experiment with the language syntax.

The ANYFX_TRANSPOSE_MATRICES is deprecated and will be removed soon. 

AnyFX is currently platform independent, although it lacks support for DirectX and HLSL at its current state. So note that AnyFX is currently only implemented for OpenGL, and currently is focused on OpenGL 4.0+ support, meaning there is no backwards compatibility with earlier versions of OpenGL or GLSL.
