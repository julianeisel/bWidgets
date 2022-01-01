Building bWidgets
=================

\brief Instructions on how to compile bWidgets from source code.

## Step 1: Install Dependencies {#dependencies}

__Required:__
- [CMake](https://cmake.org/) 3.1 or later
- Windows-only:
  - [Visual Studio 2019](https://visualstudio.microsoft.com/) or later
  - [Git for Windows](https://gitforwindows.org/)

__Demo Application:__
- [FreeType 2](https://freetype.org/)
- OpenGL?

On Windows it's easiest to use the `winget` command to install libraries.

## Step 2: Clone Sources {#clone}

Navigate to a directory of choice to put the sources and the build under. Then clone the repository, including submodules:
~~~~~~~~~~~~~~~~~~~shell
$ git clone  --recurse-submodules https://github.com/julianeisel/bWidgets.git
$ cd bWidgets
~~~~~~~~~~~~~~~~~~~

This creates a `bWidgets/` directory with the sources and navigates into it.

## Step 3: Configure CMake {#configure}

From the source directory (the `bWidgets/` directory created above):
~~~~~~~~~~~~~~~~~~~shell
$ cmake -B ../build
~~~~~~~~~~~~~~~~~~~

This creates a `build/` directory next to the `bWidgets/` one.

## Step 4: Build {#compile}

You can build bWidgets from your IDE (or `make` directly on Linux). For a cross-platform command, use the following, still in the same source directory:
~~~~~~~~~~~~~~~~~~~shell
$ cmake --build ../build --config Release
~~~~~~~~~~~~~~~~~~~

The demo application executable is located at `build/bin/bWidgetsDemo`.