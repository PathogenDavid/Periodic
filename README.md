Periodic
========

Periodic is an educational game for helping K12 students learn chemistry based on the Sifteo platform.

## Building
The project expects to be in the same directory as the Sifteo SDK. (So `periodic` should be immediately in the same folder as `bin`, `examples`, `include`, etc.)

Building can be accomplished using the Sifteo SDK Shell or by building within Visual Studio using the included Visual Studio 2013 solution.

Running in the Siftulator can be accomplished be running `make run` or by starting a debug session in Visual Studio. (Note that Visual Studio debugging features will not actually be available.)

Deploying to hardware currently is only possible by running `make install`, and cannot be accomplished through Visual Studio.

## Standalone app
In addition to running the code in the Siftulator or on the actual Sifteo cubes. There is a proof of concept standalone app named `PeriodicAppWinForms`. It's a little rough around the edges, but it demonstrates that the codebase is portable enough to run in other places.

## Third Party Licenses
Included in the repository is a modified copy of the font [Coder's Crux](http://fontstruct.com/fontstructions/show/619715) by NAL, which is licensed under [CC BY 3.0](http://creativecommons.org/licenses/by/3.0/).
It has been modified from its original state into a reduced character set bitmap font for use on Sifteo Cubes.
