Design Overview
===============

\brief High level overview on the bWidgets design.

[TOC]

# The Pipeline {#pipeline}

To get a functional, interactive widget onto the screen, it has to go through various stages of processing. On the highest level, we've established six distinct ones for bWidgets. Given that there's a specific order in which they have to be executed, we call this the _pipeline_:

* __Construction__

  Build a _screen-graph_, which defines a hierarchy and principal layout for any
  widget that may appear on screen.
* __Styling__

  Compute and assign the final styling data (colors, fonts, border-widths, etc.) for each
  item in the screen-graph.
* __Layout__

  Compute the final position and dimensions of each widget in the screen-graph.
* __Drawing__

  Draw the laid out and styled widgets of the screen-graph to an output buffer
  (typically the screen-buffer).
* __Handling__

  Pass events (typically triggered by user interaction) down the screen-graph
  to allow widgets to respond with state changes.
* __Write Back__

  Inform the host application (i.e. Blender) about any state changes to let it
  update its data accordingly.

Not all stages have to be executed on each redraw. If nothing changed, the
styling stage can be skipped for example.
<!--
The different stages don’t have to be executed strictly sequentially. In fact,
it is planned to let bWidgets performs styling and layout in an overlapping
manner: as soon as sufficient style information on a screen-graph item is
gathered, it is put into a layout queue that may be worked on in parallel to
remaining style calculations.<br/>
Neither do all stages have to be executed on every redraw. For example if the
used style and the underlying screen-graph haven’t changed, the styling stage
can be skipped.
-->

A lot can be said about each stage, we’ll look at them one by one in the
following sections.
<br/>
<br/>


> <a href="javascript:void(0)" onClick="toggleCollapsed($('#pipeline_rationale'), 'Design Rationale')">
>   <b id="pipeline_rationale-header"></b>
> </a>
<div id="pipeline_rationale">
  <div id="pipeline_rationale-summary"></div>
  <div id="pipeline_rationale-content">
> A core design principle of bWidgets is to have clearly defined and minimized
> responsibilities for all code units; from the high level modules to the low
> level functions. Both the data flow and the code architecture convey this. Such
> a principle increases the separation of concerns – an important quality in
> software engineering.<br/>
> Unfortunately, each of the above mentioned stages addresses a high-level
> responsibility that depends on its predecessors. E.g. the drawing code must not
> execute before the layout engine has done its job.<br/>
> Rather than fighting these (unavoidable) dependencies, the design is built
> around them. So the pipeline pictures the dependency flow into the bWidgets
> architecture.
> 
> Note how the described pipeline nicely implements the Model-View-Controller
> pattern:
> [TODO graphic]
  </div>
  <div id="pipeline_rationale-trigger" src="-"></div>
</div>

\htmlonly
<script type="text/javascript">
  $("#pipeline_rationale").ready(function() {
  		  toggleCollapsed($("#pipeline_rationale"), "Design Rationale");
  });
</script>
\endhtmlonly


# Construction {#construction}

The construction stage is responsible for building the screen-graph. The
screen-graph is a vital data-structure, in that most other stages base their
operations on it. It is a tree structure, with each node representing:
 * a widget, or
 * a layout item (a node that supports child nodes aligned in some layout),
 * a container widget (a widget which may have child nodes aligned in some layout; i.e. panels)

[TODO graphic]

To construct a screen-graph, a helper API called screen-graph _builder_ is used.
It hides complexity and ensures only valid screen-graphs are constructed. For
example it makes sure there’s no way to construct a label widget with children
attached to its node – a label is not a container (nor a layout).<br/>
It's actually possible to have multiple builders, allowing domain specific
specializations. Like an own builder for Blender RNA widgets, which also allows
setting RNA data in a convenient way (see example below).

Creating/expanding a screen-graph could look like this:

~~~~~~~~~~~~~~~~~~~{.cpp}
// Create a builder which will add children to the root.
bwScreenGraph::Builder builder(screen_graph_root);

// Further items will be placed in row, until different layout is activated/added.
builder.addLayout<RowLayout>();

// Parameters passed to addWidget() will be forwarded to the widget constructor (C++ perfect forwarding).
builder.addWidget<bwLabel>(“Option:”, 0, BUTTON_HEIGHT);
builder.addWidget<bwPushButton>(“Push Me!”, ICON_ACTION, 0, BUTTON_HEIGHT);

// --------------------------------------------------------------
// Example of how a custom builder for RNA widgets could be used:

ScreenGraphRNABuilder rna_builder(screen_graph_root);
PointerRNA& ptr = get_some_ptr();
PropertyRNA& prop = get_some_prop();

rna_builder.addWidget<bwCheckbox>(prop, ptr, BUTTON_HEIGHT);
~~~~~~~~~~~~~~~~~~~
<br/>


> <a href="javascript:void(0)" onClick="toggleCollapsed($('#construction_rationale'), 'Design Rationale')">
>   <b id="construction_rationale-header"></b>
> </a>
<div id="construction_rationale">
  <div id="construction_rationale-summary"></div>
  <div id="construction_rationale-content">
> One weakness of the Blender UI code is that it doesn't support nesting widgets.
> I.e. It’s not possible to have a panel widget that is the parent of its
> contained widgets. Panels, modifier-boxes and thelike get special treatment to
> at least visually appear connected to their content. But if a logical
> connection is needed (say to support dragging a modifier box with its
> contents), further tricks are needed.<br/>
> So bWidgets was supposed to support widget nesting from the get-go.
>
> Three general approaches to storing widgets in a hierarchy come to mind:
>  1. Widgets themselves store child widgets
> 
>     Widgets could have a list of children, whereby the parent controls their
>     layout (i.e. `parent_widget.setLayout(row_layout)`). A number of grouping,
>     or container widgets would likely be needed (e.g. panel, radio-button
>     group, tab-bar, …). Qt uses such a design.
> 
>  2. Widgets are owned by a layout tree
> 
>     Layouts are hierarchical by nature: Columns can contain rows, rows can
>     contain columns, panels can contain rows or columns, etc. So the layout
>     hierarchy, aka the layout tree, can be used to store widgets hierarchically
>     too.
>
>  3. A separate data-structure defines the hierarchy and manages layouts and
>     widgets within
> 
>     If neither the widgets themselves, nor the layout tree are used to store
>     the hierarchy, a separate data-structure could be used. Further, it could
>     combine the widget hierarchy and the layout tree. So the idea is to have a
>     tree structure, where each node can be a widget, a layout, or a combination
>     of both (a widget with a layout for child nodes).
>
> You may realize that we went with the third approach, but to understand why,
> we'll have to look at the topic of responsibilies first. One way to archieve
> previously mentioned separation of concerns is by minimizing the
> responsibilities of components. Components should do few things (one ideally)
> and do them well (see the single responsibility principle). Existing toolkits
> fail at that. They typically give way too many responsibilities to widgets.
> Widgets are typically responsible for:
> 
> - Drawing themselves
>
>   <br/>
> - Managing their own data
>
>   Text and icons to display, geometry, numeric values, text selection, tooltip
>   text, handling callbacks and data, widget specific options, …
>
> - Handling events
>
>   Listen to fixed events, pass on events to children if needed, listen to
>   changeable shortcuts, ...
>
> * Managing states
>
>   Correctly enter text edit state if needed (i.e. store the string to be reset
>   on cancel), forbid exiting text edit state when invalid value is entered, don’t
>   listen to events when different menu is in focus, ...
>
> * Providing hints for the layout engine
>
>   Minimum dimensions, flow direction of the widget, alignment (can this widget
>   be “stitched” to others?), …
>
> * Providing APIs for data access and manipulation
>
>   Direct state changes, trigger updates, query information (e.g. does the
>   widget have visible children?), ...
>
> * etc.
>
>   <br/>
>
> So even if widgets seem like simple little objects, they tend to grow quite
> complex due to their many responsibilities. This is something that should be
> addressed.<br/>
> Looking at our options again, the first one adds yet another responsibility for
> widgets. That is, correctly managing children. The second one doesn't, but it
> turns the layout tree into a vastly more important data-structure than it
> should be. To the point where the layout tree isn't a layout tree really, but a
> separate structure which happens to be used by the layout engine. So basically
> like option number three.<br/>
> Interestingly, the third option allows us to redistribute responsibilities via
> the intermediate data-structure it introduces. Responsibilities can be taken
> out of widgets and moved into own classes/objects, which are then connected to
> the widget via the node representing it in the hierarchy. The resulting
> classes/objects are smaller and more focused.
>
> ![Principal redistribution of responsibilities via screen-graph nodes]
> (screen_graph_responsibilities.png)
>
> The cost is another abstraction to care for. It's more intuitive to deal with
> widgets directly, without an intermediate and generic "node" object inbetween.
> But this atypical approach promisses a much greater separation of concerns. So
> it's an interesting experiment.
>
> A huge benefit of this design is that a flexible, yet simple and clear event
> dispatch path is defined: Rather than hardcoding which widgets (or containers
> like Blender’s `ARegion`s) listen to an event, the event is passed down the
> screen-graph towards a target widget (that may then bubble the event up to its
> parent again).<br/>
>
> Another interesting quality of the screen-graph design: The screen-graph has
> many similarities with the document object model (DOM). So it's quite similar
> to the core data-structure of the world-wide-web. While it's certainly fair to
> critique many aspects of web technologies, the basic DOM design seems to have
> provided a good working base.
>
> Lastly, object oriented code tends to suffer a lot from data-structures that so
> much other code depends on. We don't want to just move the complexity of
> widgets to screen-graph nodes. So the nodes should be mere data-structures
> without any (non-trivial) functions. Behavior is then implemented by helper
> classes, like the Builder class for building the screen-graph.
> es, i.e. panels).
  </div>
  <div id="construction_rationale-trigger" src="-"></div>
</div>

\htmlonly
<script type="text/javascript">
  $("#construction_rationale").ready(function() {
  		  toggleCollapsed($("#construction_rationale"), "Design Rationale");
  });
</script>
\endhtmlonly


# Styling {#styling}

TODO

# Layout {#layout}

In the layout stage, items from the screen-graph are positioned on screen. For
this, the screen-graph already contains all needed information

Note that although bWidgets’ styling is (going to be) based on the CSS
specification, we don’t use any layout features of it except of the box-model.
The CSS layout system just has too many issues in practise and doesn't suite
our needs.

# Drawing {#drawing}

TODO

# Handling {#handling}

TODO

# Write Back {#writeback}
Should the host application apply any changes to the data displayed on screen, it should also trigger an update of the view. This implies that the bWidgets pipeline is triggered to apply the necessary updates.

