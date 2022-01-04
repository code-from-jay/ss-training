# Assignment 3 Response

The command line option you'd pass to clang++ would be "-g", this flag will make the compiler emit debugging information like file names, source lines, and names of functions which would otherwise be garbled unless exported. This allows you to use debuggers like `gdb` to easily step through your program and add break points to specific function names or even specific lines in a source file.

To test this, you can build the example program with make and run the program with gdb, if you do `break fib` you can set a breakpoint inside the fib function, and gdb will even tell you where in main.cpp that breakpoint is!
