# Code split across several files

In this coursework you will be working on writing C++ code that is split across several files, as would be the case in a larger software project.

This coursework is worth 10 marks, and is due in *Wednesday the 14th of December at 11pm*.  There is no advanced part this week.

# Compiling - the story so far

So far, you have written your code in header files, with a .h extension.  These have been `#include`d by a test harness containing a main function.  At an abstract level:

`#include "YourCode.h"`  
` `  
`int main() {`  
` `  
`   ... call some functions from YourCode.h ...`  
` `  
`}`  

This would then be compiled into an executable using

`g++ -std=c++11 -o TestYourCode TestYourCode.cpp`

A `#include` statement is a copy-and-paste: when the compiler runs, whatever code is in that file is copy-pasted into the place it was included.  This allows `main()` to see and call the functions defined in that file, or to use any classes that were defined, and so on.

You've seen `#include` statements in your own code, to use the `vector` class, or `list` class, etc.  Again, this copy-pastes the class definition from the relevant file (provided with the compiler, and installed on your machine) so you can go ahead and use it.  To distinguish header files that are provided with the compiler, from those you have written yourself, note the slight difference:

`#include "YourCode.h" // double quotes: looks for a file YourCode.h that you have written`  
`#include <SystemCode> // < and > : looks for code installed on your machine`  

# Declaration and Definition

In lecture 1 I mentioned function *prototypes* -- we can declare that a function exists, before saying what it does:

`#include <iostream>`  
` `  
`void someFunction(); // declare the function`  
` `  
`int main() {`  
`  someFunction();`  
`}`  
` `  
`void someFunction() { // define the function`  
`  std::cout << "Hello world\n";`  
`}`


All that is needed to call a function is its declaration.  The definition needs to appear eventually, but the declaration is a promise that it exists.

# Splitting code across files

Open up `main.cpp`, `SomeFunction.h` and `SomeFunction.cpp` in your favourite text editor.

`SomeFunction.h` declares the function `someFunction()`.  It doesn't define it though -- it only has a prototype.  This is enough for `main.cpp` though, which `#include`s this file, so can call `someFunction()`.

The following will give an error though:

`g++ -std=c++11 -o main main.cpp`  

...something like:

`/tmp/cc19qCb7.o: In function 'main':`  
`main.cpp:(.text+0x5): undefined reference to 'someFunction()'`  
`collect2: error: ld returned 1 exit status`  

This happens because the compiler is trying to make an executable file `main`, but it's never seen the definition of `someFunction()` so the program cannot possibly work: `main()` is relying on a call to a function that was never defined.

This is where `SomeFunction.cpp` comes in.  It contains the *definition* of the function.  For our final executable to work, we need to combine the sources from `main.cpp` *and* `SomeFunction.cpp`.  There are two ways of doing this:

`g++ -std=c++11 -o main main.cpp SomeFunction.cpp`  

This compiles both of the C++ files, and then produces `main`.  Alternatively:

`g++ -std=c++11 -c main.cpp`  
`g++ -std=c++11 -c SomeFunction.cpp`  
`g++ -std=c++11 -o main main.o SomeFunction.o`  

This splits the compilation into a number of stages:

- The `-c` flag instructs it to compile `main.cpp` to an object file `main.o`, but not to make an executable.  Object files contain the compiled definitions of functions, but we can't actually run them.  `main.o` then contains the definition of `main()`.
- The same thing happens with `SomeFunction`: it produces `SomeFunction.o`, containing the definition of `someFunction()`
- Then, the two object files are combined together, producing the final `main` executable.  This then works, as it has all the function definitions it needs -- it combines both files.

As this is a lot to type in each time, we can use a Makefile to automate this.  If you open the file Makefile it defines:

- How to compile main: first make the two .o files, then do the final stage to merge them together
- A rule that says to turn a .cpp file into a .o file of the same name, run g++ substituting in the appropriate filenames

Running make will then compile the program.

I don't expect you to be able to write Makefiles.  I don't write them either.  One of the reasons is that unless we edit the Makefile to record some additional rules, and the dependencies between files (e.g. main.cpp uses SomeFunction.h) it won't recompile the program.

# [CMake](https://cmake.org/)

CMake is a tool for making Makefiles automatically.  It does all the hard bits for us.  Here is the content of the CMakeLists.txt file that we use for the example above:

`add_definitions(-std=c++11)`  
`add_executable(main main.cpp SomeFunction.cpp)`  

That's it: we want to use C++11, and make an executable called `main`, based on the two cpp files.

To actually turn this into a Makefile, we run CMake itself.  At the command line, in the directory containing your git repository:

`mkdir build`  
`cd build`  
`cmake ..`

(`..` is a shorthand for 'the parent directory' -- i.e. the directory containing the CMakeLists.txt file.)

If you look, there's now a file `Makefile` in the build directory.  Now you just run, in that directory:

`make`

...and it will compile the program.  If you edit any of the source files (try it!) just run make again, in the build directory, and it will rebuild it.  Similarly, if you edit CMakeLists.txt to e.g. add new source files, just run make again inside the build directory: this will build the new sources, too.

One thing you might have noticed is that all the compiled code has gone in the build directory.  *Don't add this to github*.  Separating your source code and compiled code is a good idea for all sorts of reasons:

### Different machine have different compilers

We may need to recompile the code on different machines: Linux, windows, mac, ...  The source code can be the same, and saved in github, but the compiled code?  Just save the CMakeLists.txt file in github, to allow you to easily compile it on another machine.

### git + compiled code = :(

Git keeps the full history of all the commits you make.  If you commit compiled code, this blows up the size of your repository.  git is pretty good at dealing with source files, as it only keeps the differences between successive versions, but it can't do so well with compiled code.  Thus, roughly, anyone who wants to clone your repository will have to download every version of the compiled code you've ever committed.

### Different build directories

Try this inside your repository;

`mkdir debug`  
`cd debug`  
`cmake .. -DCMAKE_BUILD_TYPE=DEBUG`  
`make`

This sets up a build directory that will compile the code with '-g', producing an executable that is slower, but can be used with valgrind or gdb to help you find bugs.

Then, if you want your code to run quickly, back in your repository:

`mkdir release`  
`cd release`  
`cmake .. -DCMAKE_BUILD_TYPE=RELEASE`  
`make`

This compiles with `-O3` code, producing an executable that is fast but not as useful for debugging.

You can now choose which version you want to compile, depending on what you need it for.

# (a) Completing a small project (3 marks)

In the `parta` directory, you'll find a header file `MyCode.h` and a cpp source file `MyCode.cpp` that declare and define a simple function `myCode` that takes two ints, performs an operation on them, and returns an answer.

Complete `main.cpp`, implementing a `main()` function that calls `myCode`, passing it the numbers 10 and 2, and prints out the answer.  As well as implementing `main()`, you will need to add some `#include` statements for this to work.

To compile your code, make a `CMakeLists.txt` file that makes the executable `main` from `main.cpp` and `MyCode.cpp`.  Then, make a build directory, run cmake, and run make:

`mkdir build`  
`cd build`  
`cmake ..`  
`make`

Note: you only need to run cmake once.  You can later just do:

`cd build`  
`make`

`git add` your CMakeLists.txt file to your git repository, then commit and push your work.  I will test your work for this question by trying to compile and run it, using CMake.

NB: Do *not* add your build directory or compiled files to your git repository.  I will deduct marks for this.

# (b) Roman numerals (5 marks)

In the `partb` directory, you'll find a header file `Roman.h` that declares a function `romanToInteger` that converts a string containing a number written in upper-case [Roman numerals](https://en.wikipedia.org/wiki/Roman_numerals) into an integer.

In Roman.cpp, define (i.e. implement) the `romanToInteger` function.

In main.cpp, implement a main function that calls `romanToInteger` with "MCMXIV", and prints out the integer it returns (should be 1914).

To compile your code, make a `CMakeLists.txt` file that makes the executable `main` from `main.cpp` and `Roman.cpp`.  As earlier, then make a build directory, run cmake, and run make.  Don't forget to `git add CMakeLists.txt`

NB: Do *not* add your build directory or compiled files to your git repository.  I will deduct marks for this.

# (c) Putting class functions in a separate file

In the `partc` directory, you'll find a header file `SomeClass.h` that defines a class.  You'll note that it has a function `getX()` that is declared, but not defined.  The definition is in `SomeClass.cpp`:

`int SomeClass::getX() const {`  
`    return x;`  
`}`  

The double-colon (::) notation shows that we are defining `getX()` from inside the class `SomeClass`.

Add a const function `printItOut` to the class `SomeClass`.  The declaration should go in the header file; the definition should go in the cpp source file.  The function should `cout` the value of x, followed by a newline.

In the file `main.cpp` add a main function that makes a SomeClass object, passing 42 to its constructor; then calls `printItOut()` on the object.

As with parts (a) and (b), make a CMakeLists.txt file for compiling and running your code.  Don't forget to `git add CMakeLists.txt`.

NB: Do *not* add your build directory or compiled files to your git repository.  I will deduct marks for this.
