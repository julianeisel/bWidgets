Design Overview
---------------

Core of the design are the widgets. As of now, these only have functions for simple drawing and simple event based state changing. Drawing attributes of widget types, such as colors, text alignment or corner radii are set through styles, represented by the `bwStyle` class.

C++ object-oriented programming and inheritance plays an important role for widgets. All widgets derive from the abstract [bwWidget](\ref bWidgets::bwWidget) class and implement its abstract functions. They also typically override virtual functions of [bwWidget](\ref bWidgets::bwWidget) to customize their behavior.<br/>
Since many widgets have some similar behavior and properties, in-between classes can be useful, such as [bwAbstractButton](\ref bWidgets::bwAbstractButton). Instead of inheriting from [bwWidget](\ref bWidgets::bwWidget) directly, widgets can inherit from those to get a more practical starting point.

Drawing widgets happens through multiple abstractions. A painter manages all drawing, defined using [bwPainter](\ref bWidgets::bwPainter). You tell the painter to change to a certain color, activate a gradient or eventually to draw certain geometry. No drawing calls are issued directly. Instead polygons are created ([bwPolygon](\ref bWidgets::bwPolygon)) and handed over to the painter to draw them. Or in the case of text, strings with some drawing information are passed to it.<br/>
The painter also has functions to draw typical shapes, such as round-boxes, rectangles or check-marks. In most cases those should be enough to draw widgets.<br/>
Once the painter gets the instruction to draw some geometry, it passes it on to the next important abstaction: the paint-engine ([bwPaintEngine](\ref bWidgets::bwPaintEngine)). This engine is the interface between bWidgets, and the application defined drawing implementation. That way the application can implement paint-engines using different backends (e.g. OpenGL, Vulcan, DirectX, PNG exporter, HTML & CSS exporter).

TODO handling.

General low-level data-types are bundled into an own "generics" module. Examples are [bwColor](\ref bWidgets::bwColor), [bwGradient](\ref bWidgets::bwGradient), [bwPolygon](\ref bWidgets::bwPolygon) and [bwRectangle](\ref bWidgets::bwRectangle).<br/>
They can be accessed from everywhere inside of bWidgets, but also outside of it. The public bWidgets API uses them too.
