# Welcome To Learn Engine Dev

## About

Welcome to a tutorial series for creating a simple game engine, named ***Red Engine***, from scratch!  Making a game engine is no easy task and each system we create could have multiple tutorials of its own.  But fear not, we will walk through building an engine each system at a time.  This tutorial series currently supports windows and linux, but may expand to other platforms in the future.

## Purpose

The purpose of this tutorial series is to provide a step-by-step approach to building a game engine.  The end goal is to have a fully functional game engine that can be reused for multiple projects.  There's a popular saying floating around that goes "Make games, not engines".  To not disappoint the folks that usually mention this quote, we will be making 3 games with ***Red Engine*** to test it out!

## Prerequisites

Programming and technical skills of all levels are welcome, but to get the most out of this tutorial series you should have the following:

- A basic understanding of the C programming language.  This includes compiling and writing code.
- Proficiency with writing code in Python.
- A high level understanding of OpenGL.

## Structure

There are a total of three parts to this tutorial series which are split in multiple chapters.  Each chapter is further broken up into sections which can be navigated to via the menu on the left.  Each chapter and section has source code attributed to it which can be found within the [github repo](https://github.com/Chukobyte/learn-engine-dev).  Code will need to be shared between each project folder and a game library will be created in `include/re`.  Typically, code that is created within a chapter is moved into the game library once it's implementation is completed within the sections.

## Tech Stack

***Red Engine*** will be built with the following technologies:

- **C 11** - Core engine
- **OpenGL 3.3** - Graphics API
- **SDL3** - Windowing and input
- **Miniaudio** - Audio
- **Pocketpy** - Library used to embed python for scripting

## Build

Cmake is used for cross platforming building.  Each chapter and section is a project that can be built separately.

## Work In Progress

This tutorial series is not yet complete so expect incomplete chapters and sections.  The source code for each chapter will come first followed by more detailed descriptions and information for each section.  Also builds for windows are typically done first before linux, so expect delays in updating the Makefiles for linux in later chapters.

## Feedback

Feedback is greatly appreciated!  One way to submit feedback is to open up an [issue](https://github.com/Chukobyte/learn-engine-dev/issues) in github and apply an appropriate label.  If you like what this project is doing consider starring the Learn Engine Dev [github repo](https://github.com/Chukobyte/learn-engine-dev).
