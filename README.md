## Learn Engine Dev

A work in progress tutorial series for developing a cross platform game engine from scratch.  The source code for all turtorials will be maintained in this repo and tutorials can be accessed [here](https://chukobyte.github.io/learn-engine-dev/).

### Building

[Make](https://www.gnu.org/software/make/) is used to build the source code examples.  To build the code for a section, change directory to that folder's section (which contains a Makefile) and run the `build` target with `make build`.  Below is more information about targets defined in the Makefiles.

| Name   | Description                                  |
|:------:|:--------------------------------------------:|
| build  | Compile and link the application.            |
| run    | Will run the application.                    |
| clean  | Deletes the executable and .o files.         |
| format | Will format the source code via astyle.      |

#### Environment Variables

The makefiles for each section used the following environment variables for building:

| Name           | Description                                |
|:--------------:|:------------------------------------------:|
| PYTHON_INCLUDE | Include folder of the python installation. |
| PYTHON_LIBS    | Python lib folder containing .a files.     |
| SDL2_INCLUDE   | Include folder for SDL2.                   |
| SDL2_LIBS      | SDL2 lib folder containing .a files.       |
| FREETYPE_LIBS  | Freetype lib folder containing .a files.   |
