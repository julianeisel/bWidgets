Design Principles
==========================

bWidgets uses a mostly object-oriented architecture. Not just to go with the flow or to follow some conventions, but because it really makes sense for it.<br/>
That said, if there's ever a reason to follow a different paradigm - say procedural - just go for it! C++ allows us to vary and use pretty much whatever paradigm works best.

As a whole, bWidgets was designed to have certain qualities:

Self-contained
--------------

  bWidgets does not have any dependencies. It provides interfaces for the application to do the parts of the widget pipeline that are out of bWidgets' scope.

  For example it doesn't contain a drawing backend (like OpenGL). Instead, the application has to register a paint-engine to do the actual drawing on a backend it supports.

  Neither does any widget know or care about the application specific type of data it represents. Widgets only contain some primitive data like strings or numeric values. To access and manipulate the data widgets represent (say RNA properties for Blender), the application can register callable objects (function objects) holding references to the data. These objects are then called by bWidgets on value changes.

Modularity, Maintainability, Flexibility
----------------------------------------

  Obviously, these go pretty much hand in hand.<br/>
  While there is some shared low-level code, most objects co-exist without knowing each other. For example a [bwPushButton](\ref bWidgets::bwPushButton) doesn't know about the [bwNumberSlider](\ref bWidgets::bwNumberSlider). Changes to one widget don't affect other widgets that way. The inheritance based design still allows us to make changes affecting multiple widgets though if needed.<br/>
  The self-contained quality adheres to this.

Interoperability
----------------

  Because of its modularity, flexibility and self-contained design, bWidgets is pretty interoperable. Widgets can be created "on the fly", draw themselves and be destroyed immediately after; Or be some permanent interface element that exists throughout the entire lifetime of the application. Maybe an application uses all of bWidgets' features, maybe just the widget drawing capabilities.<br/>
  It's up to the application to decide how it uses bWidgets, which just unobtrusively offers all of its functionality.

  That said, the main purpose of bWidgets is still using it in Blender. Thus design and development of bWidgets is oriented towards Blender.

Understandability
-----------------

Last but not least! Understandability - or more broadly speaking "clean code".

For any code base, "clean code" is a crucial quality for its future development. It can be argued that this applies even more to open source software, where occasional contributiors are important and volunteers need to be attracted.

For bWidgets that means: names are chosen with care, aliases are used to make general data-types more domain specific, unique-pointers and references are used to communicate ownership clearly, ...

------

Neglected Qualities
-------------------

So far, there hasn't been a need to care much about efficiency (performance) in bWidgets. Mainly due to the small layer bWidgets currently is. We don't expect to see bWidgets in a performance critical territory anytime soon either. That said, due to the modular and self-contained design, things like asynchronus drawing should still work.

Another area that has been neglected so far is exception safety. Still something to look into though.
