# Added README

## Install

### Windows - MSYS2
Perhaps most straightforward compilation is using [MSYS2](https://www.msys2.org/). Other packages such as `make` or `g++` can be installed using `pacman` from MSYS2 console.
[SFML-2.5.1](https://www.sfml-dev.org/) library should be put on on project-root directory.
As a workaround `make` should be called from PowerShell! (LSHIFT-right click...)
Make sure `obj` directory exists; it gets deleted by make clean.
```
c:\path\to\make\make.exe
```
Copy .dll files from `SFML-2.5.1/bin` to project-root folder.
Done.


## TODO
Cross platform makefile

## Debate
### Cross platform compilation
https://stackoverflow.com/questions/9791127/using-make-for-cross-platform-compilation

