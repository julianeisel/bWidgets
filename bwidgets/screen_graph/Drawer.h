#pragma once

#include <stack>

#include "bwRectangle.h"

namespace bWidgets {

class bwStyle;

namespace bwScreenGraph {
class ScreenGraph;
class Node;

class Drawer {
 public:
  static void draw(ScreenGraph& screen_graph, bwStyle& style);
  static void drawSubtree(Node& subtree_root, bwStyle& style);

 private:
  Drawer(bwStyle& style);

  void drawSubtreeRecursive(Node& subtree_root);
  void drawNode(Node& node);
  void pushMask(const Node& node);
  void popMask();

  bwStyle& style;
  std::stack<bwRectanglePixel> maskrect_stack;
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
