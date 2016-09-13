# Go

A small framework to prototype different graphics algorithms and in, and things people would label 'engine' code - whatever that means.
Go is not a game, nor is it an attempt to replace any of the big engines out there.

## Pre-compiled demo

I highly recommend downloading the pre-compiled demo instead of trying to recompile everything.


## Libraries

No 3rd-party libraries are included in this repo for licensing reasons.

## Compilation

Little bit tricky. The framework is structured into 4 main projects:

go.engine - This is where most of the code is. What you might call 'engine'.
go.game (DLL) - A layer built on top of go.engine to represent a 'game' - it's not really a game, just quick & dirty usage of the framework.
go.launcher - The launcher uses go.engine to setup an application and fires the go.game DLL callbacks.
go.tools - Different DCC tools (asset conversion, asset importing, etc.).

You will have to manually download and build the 3rd-party libraries.

## Shaders

Effects are written and stitched together in DFX (exttools/dfx).
The DFX project, that is used for the pre-compiled demo, can be found under 'go.engine/source/dfx'.
The project can be loaded by the IDE and setup to generate and copy the final HLSL source files.
The default output path for the engine is the 'assets/shaders' folder.