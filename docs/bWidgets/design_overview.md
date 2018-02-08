# Design Overview

\brief High level overview on the bWidgets design.

## Model-View-Controller (MVC)

**Note:** _This section describes the planned long-term design and is not fully
valid yet._

bWidgets is designed to take over the widgets related _view_ component in an
[MVC](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller)
based application. MVC has gained great popularity and Blender's code follows
this pattern too (rather loosely though).

![](bwidgets_mvc.png)

Communication between bWidgets and any other component is supposed to happen
through the controller. If the application receives an external event (e.g mouse
clicks or timer ticks), the controller is responsible for converting them to the
higher-level events that bWidgets uses (_onKeyPress_, _onMousePress_, etc.) and
passing them on to bWidgets. Depending on the context (e.g. which widget is
hovered or which widget currently is in text editing mode), bWidgets can then
decide on how to react to the event, change the context if needed and notify the
controller about any changes. bWidgets will also inform the controller if the
event was "consumed" or if it's allowed to pass it on to other handlers.

The most important part of the MVC _view_ component is the representation of
data. bWidgets does this by defining geometry and text to be drawn on screen (or
other media if wanted). The look of geometry and text can be controlled with
styles.<br/>
We also consider any layout management to be a part of the _view_ component.
However, bWidgets doesn't handle that part yet, so it's still job of the
application.

**Note:**
_There may be versions of the MVC pattern that better resemble the design of
bWidgets. Given the early state of the project, we keep it general though._

## Defining Widget Types

C++ object-oriented programming and inheritance plays an important role for
widgets. All widgets derive from the abstract
[bwWidget](\ref bWidgets::bwWidget) class and implement its abstract functions.
They also typically override virtual functions of
[bwWidget](\ref bWidgets::bwWidget) to customize their behavior.<br/>
Since many widgets have some similar behavior and properties, in-between classes
can be useful, such as [bwAbstractButton](\ref bWidgets::bwAbstractButton).
Instead of inheriting from [bwWidget](\ref bWidgets::bwWidget) directly, widgets
can inherit from those to get a more practical starting point.

## Widget Drawing

Drawing widgets happens through multiple abstractions. A painter manages all
drawing, defined using [bwPainter](\ref bWidgets::bwPainter). You tell the
painter to change to a certain color, activate a gradient or eventually to draw
certain geometry. No drawing calls are issued directly. Instead polygons are
created ([bwPolygon](\ref bWidgets::bwPolygon)) and handed over to the painter
to draw them. Or in the case of text, strings with some drawing information are
passed to it.<br/>
The painter also has functions to draw typical shapes, such as round-boxes,
rectangles or check-marks. In most cases those should be enough to draw
widgets.<br/>
Once the painter gets the instruction to draw some geometry, it passes it on to
the next important abstaction: the paint-engine
([bwPaintEngine](\ref bWidgets::bwPaintEngine)). This engine is the interface
between bWidgets, and the application defined drawing implementation. That way
the application can implement paint-engines using different backends (e.g.
OpenGL, Vulcan, DirectX, PNG exporter, HTML & CSS exporter).

## Widget Handling

TODO

## Styles

TODO

## Generics

General low-level data-types are bundled into an own "generics" module. Examples
are [bwColor](\ref bWidgets::bwColor), [bwGradient](\ref bWidgets::bwGradient),
[bwPolygon](\ref bWidgets::bwPolygon) and
[bwRectangle](\ref bWidgets::bwRectangle).<br/>
They can be accessed from everywhere inside of bWidgets, but also outside of it.
The public bWidgets API uses them too.
