# Design Principles

\brief Basic design principles bWidgets tries to follow.


bWidgets uses a mostly object-oriented architecture. This suits the design
pretty well.<br/>
That said, if it's reasonable to follow a different paradigm - say procedural -
just go for it! C++ allows us to vary quite a bit. So use whatever makes sense.

As a whole, bWidgets was designed to have certain qualities:

## Self-contained

bWidgets does not have any dependencies. If it has to do things that do not fit
into the scope of a rather minimal widget toolkit, interfaces are provided for
the application to implement.<br/>
For example, bWidgets doesn't contain a drawing backend (like OpenGL). Instead,
the application has to implement and register a paint-engine to do the actual
drawing on a backend it supports.

Neither does any widget know or care about the application specific type of data
it represents. Widgets only contain some primitive data like strings or numeric
values. To access and manipulate the application specific data widgets represent
(say RNA properties for Blender), the application can register its own classes
that get notified on value changes.

## Maintainability, Flexibility

We try to keep bWidgets highly modular to increase maintainability and
flexibility.
Widgets share low level types for their data (e.g. 
[bwRectangle](\ref bWidgets::bwRectangle)), and high-level abstractions to
act uppon the data (e.g. [bwPainter](\ref bWidgets::bwPainter)). That way,
different levels are handled in separate locations. Communication between them
is established through their public APIs.

APIs try to not expose implementation details. Avoid pseudo encapsulation by
making class members private and adding mutators and accessors to them. Once
again, be reasonable!

Widgets don't know of each other (although they do know about their base
classes) For example a [bwPushButton](\ref bWidgets::bwPushButton) doesn't know
about [bwNumberSlider](\ref bWidgets::bwNumberSlider) and vice versa. Changes to
one widget usually don't affect other widgets that way.

## Interoperability

Because of its flexibility and self-contained design, bWidgets is pretty
interoperable. Widgets can be created "on the fly", draw themselves and be
destroyed immediately after; Or be some permanent interface element that exists
throughout the entire lifetime of the application. Maybe an application uses all
of bWidgets' features, maybe just the widget drawing capabilities.<br/>
It's up to the application to decide how it uses bWidgets, which just
unobtrusively offers all of its functionality.

That said, the main purpose of bWidgets is still using it in Blender. Design and
development may reflect this.

## Platform Compatibility

Since bWidgets is supposed to be used in Blender, it has to support all
platforms Blender supports. Mainly Windows, Linux and macOS.

## Understandability

Last but not least! Understandability - or more broadly speaking "clean code".

For any code base, "clean code" is a crucial quality for its future development.
It can be argued that this applies even more to open source software, where
occasional contributiors are important and volunteers need to be attracted.

For bWidgets that means: names are chosen with care, aliases are used to make
general data-types more domain specific, unique-pointers and references are used
to communicate ownership clearly, ...<br/>
We also take documentation very serious.

------

## Neglected Qualities

So far, there hasn't been a need to care much about performance/efficiency in
bWidgets. Mainly due to the small layer bWidgets currently is. We don't expect
to see bWidgets in a performance critical territory anytime soon either. So
common sense should be enough to keep performance/efficiency high.

Another area that has been neglected so far is exception safety. Still something
to look into though.
