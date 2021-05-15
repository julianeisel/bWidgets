bWidgets
========

bWidgets is a cross-platform core widget toolkit, designed specifically for
[Blender](https://www.blender.org/), but reusable as a standalone library.

__NOTE:__ This is not an official Blender project!

Unlike other toolkits, bWidgets does _not_ contain window management, event
management, graphics libraries, font management and the like. It is solely
focused on core widget features: Widget types, layouts, styling, interaction.

Mission & Goals
===============

__Mission:__ Explore new design ideas & technology for a new era of Blender UI
development; start with a blank canvas.

The fundamental goals in relation to Blender are:
* Solve long standing design issues & technical debt. Apply lessons learnt from
  the current design.
  * Keep good working designs! Enrich the UI design paradigms, don’t fight them.
* Extend UI design vocabulary & functionality: Animations, drag & drop, greatly
  improved theming, multi-line widgets, etc.
* Better tooling for UI development: Clear specifications, comprehensive
  documentation, hugely increased testability, Blender UI builder app (later
  on).
* Standalone widget toolkit, adaptable by other Blender projects for a unified
  look & feel.
* Bring fun back to Blender UI coding!

Further Documentation
=====================
* [Documentation landing page](https://julianeisel.github.io/bWidgets/index.html)
  (subpages contain source-code documentation).
* [High level code design documentation](https://julianeisel.github.io/bWidgets/md_docs_bWidgets_about.html).
* [Build instructions](https://julianeisel.github.io/bWidgets/md_docs_build_instructions.html).


Licensing
========

bWidgets itself is licensed under the GNU General Public License (GPL) 2 or
later. This is a temporary solution because it contains snippets from GPL
licensed Blender code.  
Further contributions will be dual-licensed under GPL2 (or later) and the MIT
license (see LICENSE.txt) until further notice. It is planned to switch to
license that allows usage in non-GPL projects too, e.g. an MIT-like license.

The demo application is permanently licensed under GPL2 or later. Everything
else (namely documentation and unit tests) uses the MIT license, except for
bundled third party libraries for which license files can be found together
with the library code.


# Supported Platforms

bWidgets itself only requires a compiler supporting the C++11 ABI.<br/>
The demo application however requires Freetype2 and OpenGL 3.3 to compile/run.

bWidgets and its demo application have successfully been built on following
platforms:
* Linux Debian 9, GCC 6.3
* Windows 7, MSVC 2017 (only compiled, couldn't launch due to OpenGL driver
issues)
* Windows 10, MSVC 2017
* macOS Big Sur 11.2

More platforms/compilers should be tested.
