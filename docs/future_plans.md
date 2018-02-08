# Future Plans

\brief Overview on long-term plans for bWidgets.

---
**Note**

_This page is incomplete and could be improved._

---

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
