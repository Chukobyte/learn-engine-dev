## Learn Engine Dev

A work in progress tutorial series for developing a cross platform game engine from scratch.  The source code for all tutorials will be maintained in this repo and can be accessed [here](https://chukobyte.github.io/learn-engine-dev/).  A list of engine features can be found [here](https://chukobyte.github.io/learn-engine-dev/#features).

### Chapters

- [1.0 Embedding Python](https://chukobyte.github.io/learn-engine-dev/embedding_python/hello_python/)
- [2.0 Game Loop & SDL](https://chukobyte.github.io/learn-engine-dev/game_loop_and_sdl/game_loop_introduction/)
- [3.0 OpenGL Rendering](https://chukobyte.github.io/learn-engine-dev/opengl_rendering/asset_management/)
- [4.0 Implementing Audio](https://chukobyte.github.io/learn-engine-dev/implementing_audio/using_sdl_mixer/)
- [5.0 Input Management](https://chukobyte.github.io/learn-engine-dev/input_management/input_events/)
- [6.0 Entity Component System](https://chukobyte.github.io/learn-engine-dev/entity_component_system/building_an_ecs/)

*Note: Chapters are a work in progress and subject to change*

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

| Name             | Description                                |
|:----------------:|:------------------------------------------:|
| PYTHON_INCLUDE   | Include folder of the python installation. |
| PYTHON_LIBS      | Python lib folder containing .a files.     |
| SDL2_INCLUDE     | Include folder for SDL2.                   |
| SDL2_LIBS        | SDL2 lib folder containing .a files.       |
| FREETYPE_INCLUDE | Freetype include folder.                   |
| FREETYPE_LIBS    | Freetype lib folder containing .a files.   |
