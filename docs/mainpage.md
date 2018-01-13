bWidgets {#mainpage}
========

bWidgets is a cross-platform widget toolkit, designed specifically for
[Blender](https://www.blender.org/), but reusable as a standalone library.

On the long run, big parts of Blender's widget code should be replaced by
bWidgets. In short, this will greatly improve the Blender interface code while
opening doors for many new features.

The bWidgets source code [can be found on GitHub](https://github.com/julianeisel/bWidgets).

[TOC]

----

_A "b"-prefix is a common convention in Blender's source code to mark internal
Blender C-structs. The name "bWidgets" refers to this. It's fine to pronounce
bWidgets as "Blender-widgets" too._

----


# Role of bWidgets {#role_of_bwidgets}

bWidgets is purely about widgets. Everything that is not directly related to
widgets is out of scope and not part of bWidgets. For example it does __not__
contain a window-manager, a drawing-backend (like OpenGL) or an event-system.

As of now, bWidgets actually only does widget drawing with some proof-of-concept
state changing on user input.

# Motivation {#motivation}

Main motivations for bWidgets come from Blender, namely:

* __Support for "GUI Styles" in Blender__

  Every user has a different tast when it comes to visual style of UI elements.
  While some like a modern "flat" style, others can't stand it and prefer more
  classic ones.<br/>
  The proposal was made to address this by supporting multiple style presets
  that go further than themes, see
  [T45025](https://developer.blender.org/T45025).<br/>
  Support for such style presets is a core feature of bWidgets. It even supports
  CSS defined styles (WIP in
  [css_styles](https://github.com/julianeisel/bWidgets/tree/css_styles)), which
  Blender users could create, customize and share.
  
* __Simplify Theme Options in Blender__

  It is a known issue that Blender has a huge amount of theming options, far
  more than practical (see [T45352](https://developer.blender.org/T45352)).
  There are cases where extreme control is useful, but a much simpler solution
  would work better for most users.<br/>

  Previously mentioned "GUI Styles" already go a long way, especially
  considering the CSS support. For the majority of users, having such a style
  option plus a rather small color-palette for the theming would be more than
  enough. For other cases, the CSS files give maximum control.
  
* __Refactor of Blender's widget code__

  A rather major refactor of Blender's widget code is really needed. It is a
  prime time example of spaghetti code produced over decades.
  
  bWidgets would be the base of such a major refactor. It was carefully designed
  with current needs and [long-term plans](\ref future_plans) in mind.
  
* __Sharing Blender's widget code with other apps__

  Various applications could use bWidgets to get the look and feel of Blender's
  widgets (without mimicing it manually). The Blender Animation Player or the
  Cycles standalone are good examples.

  Another use-case would be a UI protoyping and mockup app for Blender. 
  
* __Getting closer to Qt (and similar toolkits)__

  Qt is a (the most?) popular widget toolkit. It is incredibly powerful and
  flexible, developers are familiar with it. While there are no concrete plans
  to switch to Qt, bWidgets helps finding out if such a switch would make sense
  and how feasible it is.

  bWidgets is in fact designed similar to Qt. Partially because they try to
  address similar requirements, partially because Qt is purposely being used as
  reference here and there.


# Current Features {#current_features}

As of now, bWidgets basically contains a few widget definitions with simple
drawing and handling mimicking Blender. It further supports:
* A primitive style preset system (only supporting the current built-in styles)
* A drawing system via a graphics framework independent drawing interface

  Basically a *painter* object defines geometry as *polygons* and draws them
  through a *paint-engine*. The paint-engine is created by the application that
  uses bWidgets. It can be a paint-engine for OpenGL drawing, Vulcan drawing,
  rendering to an image, HTML/CSS export, etc.
* Scaling
* Primitive gradient drawing
* Anti-aliasing for widget outlines.

The demo application has basic features like a primitive layout system, font
drawing using FreeType2, GLSL shader support, scrolling and DPI scaling. Most
buttons are dummies showing off the bWidgets feature set.

![img def](bwidgets_demo_screenshot.png)

Further, there is a documentation and unit testing framework for both bWidgets
and its demo application.


# Future Plans {#future_plans}

Like mentioned earlier, it's planned to replace big parts of Blender's widget
pipeline by bWidgets. This is a major undertaking and we plan to do it
step-by-step:

1. __Make Blender use bWidgets for the widget drawing__

  Basically replace Blender's
  [`interface_widgets.c`](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface_widgets.c).

2. __Port basic widget interaction handling from Blender to bWidgets__

   That is, code that controls how push-buttons change their state on mouse
   clicks, how scrollbars react to dragging, etc. At this point we might want to
   add some text-editing API to bWidgets.<br/>
   Eventually, all code in Blender's giant
   [`interface_handlers.c`](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface_handlers.c)
   that does not depend on Blender data should be replaced by bWidgets.

3. __Add an initial layout-system to bWidgets__

  With this initial layout-system, Blender's
  [`interface_layout.c`](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface_layout.c)
  would be replaced. It should be compatible with the old system.

  Eventually, we may want to have a layout-system similar to Qt's: Instead of a
  linear list of widgets, they would be in a hierarchy, whereby parent widgets
  manage the layout of their children (optionally via a layout-engine).

4. __Add pop-up management to bWidgets__

  Mainly replacing most of [`interface_regions.c`](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface_regions.c).

5. __Replace Blender's interface definition API__

  That would be parts of
  [`interface.c`](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface.c),
  for example the `uiDefBut` functions. The `uiBlock` struct could be replaced
  by the layout hierarchy by then.

These are ambitious plans for sure, but they are for the long term.

