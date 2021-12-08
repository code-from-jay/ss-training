# Assignment 4 Response

The C++ compiler option you would use is the "-c" flag, this tells the compiler to still compile the files, but not to link them into an executable. You can later link these objects into an executable by calling clang++ with the resulting object files instead of the source files.

You can test this functionality by using the provided Makefile, just running `make` will compile the source files using the "-c" flag, leaving you with object files in the `build/src` directory. Then you can link the object files into an executable by using running `make link`!
