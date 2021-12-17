## Learn Engine Dev

![Test Game Screenshot](https://github.com/Chukobyte/learn-engine-dev/blob/main/assets/images/screenshots/led_3_1.PNG?raw=true)

A work in progress tutorial series for developing a cross platform game engine from scratch.  The source code for all tutorials will be maintained in this repo and can be accessed [here](https://chukobyte.github.io/learn-engine-dev/).

### Chapters

- [1.0 Embedding Python](https://chukobyte.github.io/learn-engine-dev/1.foundation/1.embedding_python/hello_python/)
- [2.0 Game Loop & SDL](https://chukobyte.github.io/learn-engine-dev/1.foundation/2.game_loop_and_sdl/game_loop_introduction/)
- [3.0 OpenGL Rendering](https://chukobyte.github.io/learn-engine-dev/1.foundation/3.opengl_rendering/asset_management/)
- [4.0 Implementing Audio](https://chukobyte.github.io/learn-engine-dev/1.foundation/4.implementing_audio/using_sdl_mixer/)
- [5.0 Input Management](https://chukobyte.github.io/learn-engine-dev/1.foundation/5.input_management/input_events/)
- [6.0 Entity Component System](https://chukobyte.github.io/learn-engine-dev/1.foundation/6.entity_component_system/building_an_ecs/)
- [7.0 Serializing With JSON](https://chukobyte.github.io/learn-engine-dev/1.foundation/7.serializing_with_json/creating_scene_json_files/)

*Note: Chapters are a work in progress and subject to change*

### Features

- [x] Sprite Rendering
- [x] Font Rendering
- [x] Keyboard, mouse, and gamepad input
- [x] Entity Component System
- [x] Scene Management
- [x] Json Serialization of project properties and scene data
- [x] Python Scripting
- [ ] Animation
- [ ] Python Engine API
- [ ] Collisions
- [ ] Camera System
- [ ] Example Games

### Building

[Make](https://www.gnu.org/software/make/) is used to build the source code examples.  To build the code for a section, change directory to that folder's section (which contains a Makefile) and run the `build` target with `make build`.  Below is more information about targets defined in the Makefiles.

| Name        | Description                                  |
|:-----------:|:--------------------------------------------:|
| build       | Compile and link the application.            |
| run         | Will run the application.                    |
| clean       | Deletes the executable and .o files.         |
| format      | Will format the source code via astyle.      |
| debug-build | Same as `build` but adds debug symbols.      |

There is also a top level Makefile that can be used to run make targets for each section's source.  To run, define a `SECTION` parameter when executing make with the following syntax: `SECTION=[PART].[CHAPTER].[SECTION]`.

#### Example

```shell
# Build Chapter 1 Section 0 (Hello Python)
make build SECTION=1.1.0
# Run Chapter 1 Section 1 (Hello Python)
make run SECTION=1.1.0
# Format Chapter 5 Section 1 (Input Management)
make format SECTION=1.5.0
```

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
