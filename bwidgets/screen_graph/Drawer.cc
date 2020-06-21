#include "bwPaintEngine.h"
#include "bwPainter.h"
#include "bwStyle.h"

#include "Node.h"
#include "ScreenGraph.h"

#include "Drawer.h"

namespace bWidgets {
namespace bwScreenGraph {

Drawer::Drawer(bwStyle& _style) : style(_style)
{
}

void Drawer::draw(bwScreenGraph::ScreenGraph& screen_graph, bwStyle& style)
{
  Drawer drawer{style};
  drawer.drawSubtreeRecursive(screen_graph.Root());
}

void Drawer::drawSubtree(Node& subtree_root, bwStyle& style)
{
  Drawer drawer{style};
  drawer.drawSubtreeRecursive(subtree_root);
}

void Drawer::drawSubtreeRecursive(bwScreenGraph::Node& subtree_root)
{
  const bool has_maskrect = subtree_root.MaskRectangle().has_value();

  drawNode(subtree_root);

  if (has_maskrect) {
    pushMask(subtree_root);
  }

  if (subtree_root.childrenVisible() && subtree_root.Children()) {
    for (auto& child_node : *subtree_root.Children()) {
      drawSubtreeRecursive(*child_node);
    }
  }

  if (has_maskrect) {
    popMask();
  }
}

void Drawer::drawNode(bwScreenGraph::Node& node)
{
  bwWidget* widget = node.Widget();
  if (!widget || !node.isVisible() || node.Rectangle().isEmpty()) {
    return;
  }

  style.setWidgetStyle(*widget);
  widget->draw(style);
}

void Drawer::pushMask(const Node& node)
{
  bwRectanglePixel final_maskrect = *node.MaskRectangle();

  if (!maskrect_stack.empty()) {
    final_maskrect.clamp(maskrect_stack.top());
  }
  maskrect_stack.push(final_maskrect);
  bwPainter::s_paint_engine->enableMask(maskrect_stack.top());
}

void Drawer::popMask()
{
  maskrect_stack.pop();
  if (!maskrect_stack.empty()) {
    bwPainter::s_paint_engine->enableMask(maskrect_stack.top());
  }
}

}  // namespace bwScreenGraph
}  // namespace bWidgets
