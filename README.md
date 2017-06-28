# bWidgets

bWidgets is a cross-platform widget toolkit, designed specifically for the Blender project, but reusable as a standalone library.

A "b"-prefix is a common convention in Blender's source code to mark internal Blender C-structs. The name "bWidgets" refers to this.
It's fine to pronounce bWidgets as "Blender-widgets".

## Motivation

Plan is to replace big parts of Blender's widget code by the bWidgets library. There are several reasons why this could bring benefits for Blender, although the world outside of Blender could benefit too.

### Refactor of Blender's widget code

A refactor of Blender's widget code is long overdue. Especially the widget interaction handling, which basically still uses the same code from when Blender was born in the nineties.
While the widget drawing code has been refactored during the 2.5 project, it could be improved further and could work better with the interaction handling code.

Using a standalone widget toolkit would cause the Blender widget code to be modular and well designed. It would add useful layers of abstraction.

### Introduction of Draw-Styles to Blender

It is planned to make bWidgets support draw-styles. Such draw-styles would be presets that can greatly change look and feel of widgets.

In Blender we planned to support such a thing anyway (see [T45025](https://developer.blender.org/T45025)). However, bWidgets could go even further and support custom styles using style sheet or scripting languages (CSS, Python, JSON). Blender users would get huge amounts of control on how the application looks, and they could easily share the looks online.

### Reusing Blenders widget code for other apps

Various applications could use bWidgets to get the look and feel of Blender's widgets (without having to mimic it manually). This would make sense to give the Blender Animation Player or the Cycles standalone a proper UI. In the past there were also people asking about which widget toolkit Blender uses so they could use the same, simply because they liked it.

One more possible use-case would be building a UI protoyping app for Blender. Users, script-authors and Blender developers could use it for testing and communicating ideas. This could be a big deal.

### Working towards a possible move to Qt widgets

Qt is an (the?) industry standard widget library. It is incredibly powerful and flexible. Developers are familiar with it.
bWidgets could act as an intermediate step for Blender towards using Qt. Allowing Blender developers to see if a switch could work, how tangible it is and if it would make sense at all.

bWidgets is in fact designed similar to Qt. Partially because they try to address similar requirements, partially because Qt is purposely being used as reference. This should make later switch to Qt *much* easier (even if just for testing purposes).

## Supported Platforms

Although bWidgets aims to be cross-platform, it has only been built and tested on Linux (Debian 8) so far. Making sure this works on other platforms (Windows, Mac) should be done soon.

bWidgets has only been compiled on GCC 6.3 so far, but should also compile on GCC 5.1 and newer, as well as all compilers with C++11 ABI support.
