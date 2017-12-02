Future Plans
------------

As mentioned in the [main page](\ref index), it's planned to replace big parts of Blender's widget pipeline by bWidgets. This is a major undertaking and has to be done with care. So the idea is to focus on one part of the pipeline at a time, and step by step add features to bWidgets that Blender can use.

First, that would be the making Blender use bWidgets for the widget drawing. Basically replace Blender's [`interface_widgets.c`](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface_widgets.c).

Following that, some basic interaction handling for widgets can be done in bWidgets. That is, code that controls how push-buttons change their state on mouse clicks, how scrollbars react to dragging, etc. At this point we might want to add some text-editing API to bWidgets.<br/>
Eventually, all code in Blender's [`interface_handlers.c`](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface_handlers.c) that does not depend on Blender data should be replaced by bWidgets. Code using Blender data (or whatever application specific data) would be executed using functors that Blender can assign to widgets.<br/>

Next up, bWidgets should get a widget layout system. Qt shows an interesting approach to this that bWidgets could follow: Instead of a linear list of widgets, there is a widget hierarchy where widgets can contain child-widgets. Each parent widget is then responsible for the layout of its childs. For some cases this layout may be 'hardcoded', although usually a layout from a layout-engine would be activated (like `widget.setLayout(horizontal_layout)`).<br/>
For Blender, this would replace the need for the `uiBlock` struct which only allows linear widget storage. It would further integrate the layout system much better with the widget storage. The layout system usage via the BPY should stay compatible.
