bWidgets {#mainpage}
========

bWidgets is a cross-platform widget toolkit, designed specifically for
[Blender](https://www.blender.org/), but reusable as a standalone library.

On the long run, big parts of Blender's widget code should be replaced by
bWidgets. In short, this will greatly improve the Blender interface code and
allow us to address long standing usability issues.

The bWidgets source code [can be found on GitHub](https://github.com/julianeisel/bWidgets).

[TOC]

----

_A "b"-prefix is a common convention in Blender's source code to mark internal
Blender C-structs. The name "bWidgets" refers to this. It's fine to pronounce
bWidgets as "Blender-widgets" too._

----

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
  considering the CSS support. Now, the idea is, to only have these styles, plus
  a single color-palette to control the entire theme.
  <br/>
  For the majority of users, this should be more than
  enough control. For advanced cases, the CSS files give maximum control.
  
* __Major Rewrite of Blender's widget code__

  A rather major refactor of Blender's widget code is really needed. It is a
  prime time example of spaghetti code produced over decades.
  
  bWidgets would be the base of such a major refactor. It was carefully designed
  with current needs and [long-term plans](\ref md_docs_future_plans) in mind.
  <br/>
  The
  _[Current Blender widget code vs. bWidgets](\ref md_docs_current_ui_code_vs_bwidgets)_
  page discusses this in length.
  
* __Unit- and Regression-Testing__

  Because of various reasons, Blender currently doesn't have any unit- or
  regression-tests for its UI code. bWidgets comes with a design that should
  allow extensive test coverage. A testing framework is builtin too.
  
* __Sharing Blender's widget code with other apps__

  The look and feel of an application is an important part of its branding and
  user-experience. bWidgets allows us to share the widget code across
  applications, creating a consistent experience over more parts of the Blender
  infrastructure. For example the [Blender Animation
  Player](https://developer.blender.org/T37764), the
  [Cycles standalone](https://wiki.blender.org/index.php/Dev:Source/Render/Cycles/Standalone)
  and the Blender .msi installer could get a unified look and feel.
  
  But, even better, we could build a
  [WYSIWYG](https://en.wikipedia.org/wiki/WYSIWYG) UI design application for
  Blender. Allowing us to build prototypes and mockups with graphical tools
  (like [JavaFX Scene Builder](http://gluonhq.com/products/scene-builder/) or
  [Qt Quick Designer](http://doc.qt.io/qtcreator/creator-using-qt-quick-designer.html)).
  
* __Getting closer to Qt (and similar toolkits)__

  Qt is a (the most?) popular widget toolkit. It is incredibly powerful and
  flexible, developers are familiar with it. While there are no concrete plans
  to switch to Qt, bWidgets helps finding out if such a switch would make sense
  and how feasible it is.

  bWidgets is in fact designed similar to Qt. Partially because they try to
  address similar requirements, partially because Qt is purposely being used as
  reference here and there.

# Role of bWidgets {#role_of_bwidgets}

bWidgets is purely about widgets. Everything that is not directly related to
widgets is out of scope and not part of bWidgets. For example it does __not__
contain a window-manager, a drawing-backend (like OpenGL) or an event-system.

As of now, bWidgets actually only does widget drawing with some proof-of-concept
state changing on user input.


# Current Features {#current_features}

As of now, bWidgets basically contains a few widget definitions with simple
drawing and handling mimicking Blender. It further supports:
* A primitive style preset system (only supporting the current built-in styles)
* A drawing system via a graphics framework independent drawing interface

  Basically a _painter_ object defines geometry as _polygons_ and draws them
  through a _paint-engine_. The paint-engine is created by the application that
  uses bWidgets. It can be a paint-engine for OpenGL drawing, Vulcan drawing,
  rendering to an image, HTML/CSS export, etc.
* Scaling
* Primitive gradient drawing
* Anti-aliasing for widget outlines

The demo application has basic features like a primitive layout system, font
drawing using FreeType2, GLSL shader support, scrolling and DPI scaling. Most
buttons are dummies showing off the bWidgets feature set.

![](bwidgets_demo_screenshot.png)

Further, there is a documentation and unit testing framework for both bWidgets
and its demo application.
