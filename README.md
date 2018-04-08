bWidgets
========

bWidgets is a cross-platform widget toolkit, designed specifically for
[Blender](https://www.blender.org/), but reusable as a standalone library.

Further documentation:
* [Documentation landing page](https://julianeisel.github.io/bWidgets/index.html)
  (subpages contain source-code documentation).
* [High level code design documentation](https://julianeisel.github.io/bWidgets/md_docs_bWidgets_about.html).
* [Build instructions](https://julianeisel.github.io/bWidgets/md_docs_build_instructions.html).


Licensing
========

As a temporary solution, bWidgets uses the GPL2 or later. This is to avoid
licensing issues when using GPL licensed code, specifically from Blender.
Further contributions will be dual-licensed under GPL2 (or later) and the MIT
until further notice.
Later we can then switch back to either MIT, or a different FOSS license, like
Apache2 or LGPL. Note that this only applies to bWidgets itself.

The demo application is licensed under GPL2 or later. Everything else (namely
documentation and unit tests) uses the MIT license (see LICENSE.txt), except
for third party libraries for which a license file is placed inside each root
directory.


# Supported Platforms

bWidgets itself only requires a compiler supporting the C++11 ABI.<br/>
The demo application however requires Freetype2 and OpenGL 3.3 to compile/run.

bWidgets and its demo application have successfully been built on following
platforms:
* Linux Debian 9, GCC 6.3
* Windows 7, MSVC 2017 (only compiled, couldn't launch due to OpenGL driver
issues)
* Windows 10, MSVC 2017

More platforms/compilers should be tested.
