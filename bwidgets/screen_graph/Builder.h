#pragma once

#include <type_traits>

#include "bwWidget.h"
#include "Node.h"
#include "ScreenGraph.h"

namespace bWidgets {
namespace bwScreenGraph {

/**
 * \brief Helper class to construct screen-graphs.
 */
class Builder {
 public:
  Builder() = default;
  Builder(LayoutNode& active_layout_node);
  Builder(ScreenGraph& screen_graph) : Builder(screen_graph.Root())
  {
  }
  ~Builder() = default;

  static void setLayout(LayoutNode& node, bwPtr<bwLayoutInterface> layout);
  static bwWidget& addWidget(LayoutNode& node, bwPtr<bwWidget> widget);

  void setActiveLayout(bwScreenGraph::LayoutNode&);

  /**
   * \brief Add child node for a layout created in-place.
   */
  template<typename _LayoutType, typename... _Args> LayoutNode& addLayout(_Args&&... __args)
  {
    static_assert(std::is_base_of<bwLayoutInterface, _LayoutType>::value,
                  "Should implement bwLayoutInterface");

    LayoutNode& new_node = addChildNode<LayoutNode>(*_active_layout_node);
    new_node.layout = bwPtr_new<_LayoutType>(std::forward<_Args>(__args)...);
    setActiveLayout(new_node);
    return new_node;
  }

  template<typename _WidgetType, typename... _Args> _WidgetType& addWidget(_Args&&... __args)
  {
    static_assert(std::is_base_of<bwWidget, _WidgetType>::value, "Should derrive from bwWidget");

    WidgetNode& new_node = addChildNode<WidgetNode>(*_active_layout_node);
    new_node.widget = bwPtr_new<_WidgetType>(std::forward<_Args>(__args)...);
    new_node.handler = new_node.widget.get();
    return static_cast<_WidgetType&>(*new_node.widget);
  }

  /**
   * Only creates widget in-place, layout still needs to be created by the
   * caller. Otherwise, we'd have to do perfect forwarding of parameters to
   * multiple constructors. This can be done (e.g. see
   * https://cpptruths.blogspot.com/2012/06/perfect-forwarding-of-parameter-groups.html)
   * but in the end, just letting the caller create the layout is less
   * verbose and easier to understand.
   */
  template<typename _WidgetType, typename... _Args>
  ContainerNode& addContainer(bwPtr<bwLayoutInterface> layout, _Args&&... __args)
  {
    static_assert(std::is_base_of<bwWidget, _WidgetType>::value, "Should derrive from bwWidget");

    ContainerNode& new_node = addChildNode<ContainerNode>(*_active_layout_node);
    // TODO compile time check if widget supports being container.
    setLayout(new_node, std::move(layout));
    new_node.widget = bwPtr_new<_WidgetType>(std::forward<_Args>(__args)...);
    new_node.handler = new_node.widget.get();
    setActiveLayout(new_node);
    return new_node;
  }

  /**
   * \brief Add child node for a widget created in-place.
   * Simplifies creating and appending widgets. The arguments \a __args are
   * forwarded to the widget constructor; a new node for it is added as child
   * to \a node.
   *
   * Without `emplaceWidget()`:
   * \code
   * using bWidgets::bwScreenGraph;
   *
   * auto& widget = static_cast<bwLabel&>(Builder::addWidget<bwLabel>(screen_graph,
   * bwPtr_new<bwLabel>("Foo", 0, 10))); widget.foo();
   * // ...
   * \endcode
   *
   * With `emplaceWidget()`:
   * \code
   * using bWidgets::bwScreenGraph;
   *
   * auto& widget = Builder::emplaceWidget<bwLabel>(screen_graph, "Foo", 0, 10);
   * widget.foo();
   * // ...
   * \endcode
   */
  template<typename _WidgetType, typename... _Args>
  static _WidgetType& emplaceWidget(LayoutNode& node, _Args&&... __args)
  {
    static_assert(std::is_base_of<bwWidget, _WidgetType>::value, "Should derrive from bwWidget");

    WidgetNode& new_node = addChildNode<WidgetNode>(node);
    new_node.widget = bwPtr_new<_WidgetType>(std::forward<_Args>(__args)...);
    new_node.handler = new_node.widget.get();
    return static_cast<_WidgetType&>(*new_node.widget);
  }

  static void setWidget(WidgetNode& node, bwPtr<bwWidget> widget);

 private:
  template<typename _NodeType> static _NodeType& addChildNode(LayoutNode& parent_node)
  {
    static_assert(std::is_base_of<Node, _NodeType>::value,
                  "Should derrive from bwScreenGraph::Node");

    parent_node.children.push_back(bwPtr_new<_NodeType>());
    Node& ref = *parent_node.children.back();
    ref.parent = &parent_node;

    return dynamic_cast<_NodeType&>(ref);
  }

 private:
  bwScreenGraph::LayoutNode* _active_layout_node{nullptr};
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
