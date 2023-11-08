bWidgets
========

_bWidgets is a cross-platform widget framework with a narrow scope, designed
specifically for [Blender](https://www.blender.org/)._

__Note:__ This is not an official Blender project!

Unlike common UI toolkits, bWidgets does _not_ contain window management, event
management, graphics libraries, font management and the like. It is solely
focused on core widget features: Widget types, layouts, styling, interaction.

Do <u>not</u> attempt to use bWidgets at this point. A lot of the core designs
are still in progress.

<br/>
<div align="center">
<img width="318" alt="bwidgets_classic_style" src="https://user-images.githubusercontent.com/7873998/147843328-a97c0841-9a71-41bb-8b8b-41c6af5c55ee.png"> <img width="318" alt="bwidgets_flat_light_style" src="https://user-images.githubusercontent.com/7873998/147843329-fb01ebc0-62b9-4f3a-a1cb-cf61f7d0c84a.png"> <img width="318" alt="bwidgets_flat_dark_style" src="https://user-images.githubusercontent.com/7873998/147843330-a249ab1f-a2b3-4653-b69d-92ae9dbc2b99.png">

<br/>
<i>Demo application in different Styles</i>
</div>

Mission & Goals
===============

__Mission:__ Explore new design ideas & technology for the next decade of
Blender UI development; start with a blank canvas.

The fundamental goals in relation to Blender are:
* Address long standing design issues & technical debt. Apply lessons learnt
  from the current design.
  * Keep good working designs!
  * Enrich the UI design paradigms, don’t fight them.
* Extend UI design vocabulary & functionality: Animations, drag & drop, much
  improved theming, multi-line widgets, custom widgets, etc.
* Professional grounds for UI development: Clear specifications, comprehensive
  documentation, hugely increased testability, Blender UI builder app, debugging
  tools, ...
* A reusable widget toolkit for the Blender design, adoptable by other Blender
  services for a unified look & feel (standalone).
* Support exposing the entire toolkit to the Blender Python API.
* Bring fun back to Blender UI coding!

Further Documentation
=====================
* [Documentation landing page](https://julianeisel.github.io/bWidgets/index.html)
  (subpages contain source-code documentation).
* [High level code design documentation](https://julianeisel.github.io/bWidgets/md_docs_bWidgets_about.html).
* [Build instructions](https://julianeisel.github.io/bWidgets/md_docs_build_instructions.html).


Licensing
========

Components of this repository have different licenses:
* bWidgets itself is licensed under the GNU General Public License (GPL) 2 or
  later.
  * This is a temporary solution because it contains snippets from GPL
    licensed Blender code.
  * Further contributions will be dual-licensed under GPL2 (or later) and the MIT
    license (see LICENSE.txt) until further notice. It is planned to switch to
    license that allows usage in non-GPL projects too, e.g. an MIT-like license.
* The demo application is permanently licensed under GPL2 or later.
* The demo application further includes source files of third party libraries
  together with their respective license files.
* Everything else (namely documentation and unit tests) is licenced under the MIT
  license.


# Supported Platforms

bWidgets itself only requires a compiler supporting the C++11 ABI.<br/>
The demo application however requires Freetype2 and OpenGL 3.3 to compile/run.

It has been succesfully build on a number of macOS, Linux and Windows systems.
