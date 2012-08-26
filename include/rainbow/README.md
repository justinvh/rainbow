# Rainbow Headers #

## Where to begin? ##
Each folder corresponds to a module of the engine. So, if you're interested
in working on the renderer, then I would advise you to go check out the
renderer folder, etc. Read the folder breakdown for more details.

## Folder Breakdowns ##
Any file that has a corresponding folder name will just be a top-level
include for library-users. The libraries internal to Rainbow will call below
the folders for the most part.

### `display` ###
The windowing functions. This module provides access to the window and
provides an interface for initializing common drawing routines indirect of
the actual engine.

### `map` ###
Supported map formats and integration. These are formats that will typically
provide ways of getting map data into the renderer in the most painless
fashion.

### `math` ###
Math functionality. This module is a collection of math-like functions such
as random number generation, glm-wrappers, and so on.

### `model` ###
Model interaction and loading. These are formats that will typically provide
ways of getting model data into the renderer.
