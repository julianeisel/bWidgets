Current Blender widget code vs. bWidgets
========================================

\brief Which problems does the current Blender widget code have and how does
bWidgets try to solve them?

[TOC]

With bWidgets we try to solve a number of issues the current Blender UI code
has. The following sections describe some of these issues and how bWidgets tries
to address them.

---
**Note**

_At this point bWidgets is limited to drawing and some very basic
handling. Hence, much of the issues mentioned here are not addressed yet, but
bWidgets is designed to do so from the ground up._

---

# General Design {#general_design}

## There is none (almost)

Aside from a general MVC approach (which is broken in many places), the Blender
interface code as a whole doesn't really follow any design ideas. "Just make it
work" is the route it takes. Granted, this may not necessarily be bad, and after
all this code worked for many years; however, it became a big mass of spaghetti
code that is hard to maintain, extend and understand.

bWidgets does have a far more defined design. It sticks to using MVC and it
is much more strict in applying it. Further, it incorporates useful abstractions
to clearly separate different parts of the code. That alone adds another level
of flexibility.
<br/>
For more information, check the [design overview]
(\ref md_docs_bWidgets_design_overview).

## "Wannabe-generic" data-structures

The central widget related data-structures (e.g. `uiBlock`, `uiBut`,
`uiHandleButtonData`) try to be one-size-fits-all designs. All widgets use
exactly these structures, even though they need to store completely different
data. Hacks are used to get variables to work for multiple widget-types, even if
the needed data-type differs. Examples are the
[`uiBut.a1` and `uiBut.a2` fields](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface_intern.h;1dafe759edc3bc9a89f99a894d1c1fb3a14f44a9$224-243),
or
[`uiBut.poin`](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/include/UI_interface.h;1dafe759edc3bc9a89f99a894d1c1fb3a14f44a9$229-245).
Additionally, the data-structures contain many members they only need in certain
situations (e.g. for a certain button type).<br/>
These points make the structures pretty confusing to work with and ask for
trouble (and add unnecessary bytes of memory).

In the bWidgets design, every widget-type has its own data-structure,
specifically crafted for its own needs. Data-duplication across multiple
widget-types can be avoided using inheritance. The controller (speaking in MVC
terminology) can implement interfaces which store custom data as needed, like
RNA or operator data. Although not implemented yet, imagine it to work like
this: the controller registers a handler object for widgets (e.g.
`uiRNAStringHandler` or `uiOperatorHandler`), that stores its own data and gets
notified on state changes.

## Tight coupling of data and modules

The Blender UI code is built really tightly around the Blender data systems,
much closer than healthy. There are calls to DNA and RNA everywhere. Different
modules are also mixed, the UI code does many Blender-kernel or window-manager
calls. Such tight coupling of data and modules provokes problems:
* Changes in one module might affect the functioning of the others.
* Refactoring or even replacing single modules becomes really hard.
* Code becomes a lot more confusing.

Making the code more modular and adding reasonable abstractions should help
here.

bWidgets would not know anything about Blender's internal data systems. In its
design, widgets are more like blackboxes, manufactured to represent more complex
data with primitive tools. They only know about strings, numeric-values, color
values, etc. The controller (in MVC terms) manages the application specific data
manipulations (e.g. updating DNA through RNA).

## Unconventional/unfitting "design"

It is general consensus that widget/GUI toolkits benefit from object-oriented
designs. There are many different kinds of GUI elements and there is lots of
overlap between them. Such overlap can be managed nicely using
object-oriented-programming (OOP) inheritance in code. Encapsulation and
abstraction are useful to ensure valid states (often through _manager_-objects
and the like), maintainability and great flexibility. Blender doesn't follow any
of these conventions, even though they seem to be very reasonable.
<br/>
For new programmers, this adds another hurdle, since they are used to entirely
different designs.

_Note that this point is not about programming languages: GTK+ (a widely used
widget toolkit) is written in C, but tries to emulate OOP features as much as
possible. However an OOP-based language should still be better suited._

The design of bWidgets is based on these conventions and should be able to gain
from their benefits. It also uses C++ which has OOP features builtin.

# Limitations {#limitations}

## Unit-/Regression-testing

No part of the Blender UI code is unit-tested as of now. Because of the tight
coupling of modules and data, it's just not easy to do. A unit-test can't simply
create a widget to test some of its functionality, a big part of the widget
pipeline would have to be involved. Or at least, only very few units can be
tested in a useful manner.
<br/>
Neither is there a way to render the GUI or parts of it (individual widgets)
into an image. Such images would be useful for regression-tests, for example in
the layout- or theme-system.

bWidgets addresses this as follows:
* Unit-tests are part of the bWidgets feature set.
* Widgets are independent from much of the widget pipeline, so individual
  aspects can be tested easily.
* The paint-engine design allows to easily render individual widgets or the
  entire GUI into images which can be used for regression-testing.

## Missing hierarchy

Widgets are grouped together into blocks (`uiBlock`) in Blender. This system is
not very flexible in that there is a lack of further grouping capabilities.
There's no nesting of blocks.
<br/>
A hierarchical structure could bring some benefits:
* Integrate panels into widget pipeline (they act like blocks, but have drawing
  capabilities like widgets).
* Better control over handling with multiple widgets involved (e.g. modifier
  drag & drop or
  [_multi-number button editing_](https://wiki.blender.org/index.php/Dev:Ref/Release_Notes/2.70/UI#Multi-Number_Button_Editing)).
* Add a real relation between buttons and popups (e.g. the button to invoke a
  menu would be the parent of the menu). Currently this relation is barely
  there.
* Limit redraws (e.g. only redraw one node of the tree).
* Faster lookups (e.g. use bounding-box of grouped widgets when trying to find
  hovered widget).
* Better and easier debugging (debugging can be limited to a specific part of
  the tree, plus the tree can be visualized or printed to stdout).
* Replace `uiBlock` with case specific data-structures.

On the downside, hierarchical or tree structures are harder to work with than
linear lists, thus it's easier to make mistakes. We can unit-test hierarchies
nicely though.

In bWidgets we could do what Qt does: Widgets can have child widgets, whereby
each parent widget is responsible for managing the layout of its children. In
most cases, the parent widget would ask the layout system to do needed
calculations.

# Widget Handling (User Input Code) {#widget_handling}

## Welcome to hell - `interface_handlers.c`

Most GUI interaction code is placed in
[`interface_handerls.c`](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface_handlers.c),
a 10,000+ lines beast. It's really hard to navigate in such a file, though the
file size by far isn't the only problem (that alone would be easy to solve):
* Different levels of code are mixed everywhere.
* Mixed handling of entire areas, widgets, popups, text-editing, tooltips, etc.
* Contains lots of functions that don't belong there (e.g. context-menu
  population, see [`ui_but_menu`]
  (https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface_handlers.c;00d2dfa93ae8a84b163930246709c50b8ee1e148$6763-7077)).
* Thousands of exceptions for countless special cases.
* Data management is mixed with low-level event handling everywhere.
* State changes are hard to follow and predict.
* ...

## Confusing Popup Handling

Popup handling is part of `interface_handlers.c` and suffers from the same
issues. It still deserves special mention though:
* The popup state is especially hard to predict and follow. It's based on return
  values which get passed through multiple levels of recursion (and some
  temporary data-structures)
* Context gets temporarily changed.
* Code jumps from special menu handling to regular button handling multiple
  times.
* ...

If you want to get a taste of it, have a look at
[`ui_handle_menus_recursive`](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface_handlers.c;a3409d3f53f1decb3cbe5c04fd804062dcf2cf49$9877-9974),
or the gorgeous
[`ui_handle_menu_event`](https://developer.blender.org/diffusion/B/browse/master/source/blender/editors/interface/interface_handlers.c;a3409d3f53f1decb3cbe5c04fd804062dcf2cf49$9057-9492).

For bWidgets we plan to have something like a popup-manager to take over
high-level popup handling. Using the hierarchical widget storage described
earlier, proper relations can be established that make the execution easier to
follow. We may also add some internal UI-context to bWidgets for managing
states. All these changes should split the popup handling into smaller, more
manageable parts.

# Theme System {#theme_system}

## Limited Theme Options

The current theme-system only allows limited tweaks to the appearance of GUI
elements. Why shouldn't it have full control over how the GUI looks? What if it
had control over header sizes, widget sizes, layout margins, text alignments,
radii of widget corners, the used icon set, icon sizes, widget drop-shadows, ...
With the current approach to theming in Blender that's not possible, because it
mainly concerns colors and every option is exposed in the UI. [We should not
expose any more theme options](https://developer.blender.org/T45352).

Let's rethink theming completely.
<br/>
bWidgets has _Styles_ builtin, which should eventually give full control over
how widgets look. These styles can be CSS based (WIP), meaning users can edit
them even if not exposed in the UI. And, they should be able to share styles
online. The way they are designed, styles (and thus CSS) can affect properties
outside of bWidgets too, for example header sizes.

Imagine a theming-system, which lets you choose from a flat, depth-based design
(like [Google Material Design](https://material.io/)), a more classic design, or
even an OS-native design in Blender. And that fully customizable and with
support for sharing.
<br/>
[GTK+ is a great example of such flexibility]
(https://www.gnome-look.org/browse/cat/135/)
<br/>
Eventually, styles should be able to completely override the drawing (and
handling) of widgets. For example a theme/style could make Blender's
unconventional number-widgets use a conventional design for those who want it.

## Themes can't inherit nicely

Depending on where widgets are drawn, they may need to use different theme
colors. Blender's push-buttons (to execute an operator) are drawn completely
different when placed in a menu - and different again when placed in a pie-menu.
Similarly, text colors may need adjustments when used in front of special
backgrounds (see [T48241] (https://developer.blender.org/T48241)). Currently,
Blender solves this with hardcoded checks for these special cases. These become
hard to keep track of and maintain. Fixing problems with the hard-coded checks
is no rocket-science, but we can actually get rid of them entirely.

A widget theming system that allows overrides based on where widgets are placed
in a hierarchy would be preferable. CSS has that builtin: The `bwMenu >
bwPushButton` CSS selector would apply its style properties only to push-buttons
that are placed directly in a menu. bWidgets could support that.

## XML vs. CSS for Themes

Blender already supports storing and loading themes using XML. However it is a
markup language, not a style-sheet language. In theory we could add all the
features we need to it by adding more custom tags and properties. But all that
is just emulating something that CSS is built to do. Just [look at one of the
theme files](https://developer.blender.org/diffusion/B/browse/master/release/scripts/presets/interface_theme/flatty_light.xml),
and it's obvious how it tries to mimic CSS. Designers should also be familiar
with CSS, unlike some Blender specific XML usage.
<br/>

The CSS standards define many features we'd need for an advanced theme-system:
* Property values based on state (e.g. different color on mouse hover)
* Property values based on hierarchy position (see previous section)
* Margins and paddings
* Text alignment
* Linear gradients (no more `shadetop` and `shadedown` for widget themes)
* Drop-shadows
* ...

So we would be able to support many of the CSS standard properties, and the
selectors suit our needs as well. There are CSS parsing libraries that we can
use. Although we may have to write our own code to interpret the parsing result
according to the standards. We can start with a small supported subset of the
standards and add more features as needed.
<br/>
Work on CSS support is ongoing in the [css_styles]
(https://github.com/julianeisel/bWidgets/tree/css_styles) branch.
