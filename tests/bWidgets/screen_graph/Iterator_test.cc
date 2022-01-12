
#include "gtest/gtest.h"

#define private public  // XXX Oh the evilness!
#include "screen_graph/Iterators.h"
#undef private
#include "screen_graph/Builder.h"

using namespace bWidgets;

class DummyNodeLayout : public bwLayoutInterface {
 public:
  DummyNodeLayout(std::string label) : label(label)
  {
  }
  bwRectanglePixel getRectangle() override
  {
    return {};
  }
  std::string label;
};

class IteratorTest : public ::testing::Test {
 protected:
  bwScreenGraph::LayoutNode screen_graph;
  const std::array<std::string, 15> labels = {"Lorem",
                                              "ipsum",
                                              "dolor",
                                              "sit",
                                              "amet,",
                                              "consectetuer",
                                              "adipiscing",
                                              "elit.",
                                              "Aenean",
                                              "commodo",
                                              "ligula",
                                              "eget",
                                              "dolor.",
                                              "Aenean",
                                              "massa."};

  IteratorTest()
  {
    screen_graph.layout = bwPtr_new<DummyNodeLayout>(labels[0]);
  }

 public:
  /**
   * Count how many nodes are iterated over starting from \a root_node and
   * check if it matches \a expected_count. If not, the calling test fails.
   * It also checks if the label of the iterated node matches the item of
   * above's labels array with the current iteration index.
   *
   * \note The iteration includes \a root_node, so make sure it uses
   *       labels[0] as label.
   */
  void expectNodeCountAndMappedLabels(bwScreenGraph::Node& root_node, uint expected_count)
  {
    uint counter = 0;

    assert(labels.size() >= expected_count);
    for (bwScreenGraph::Node& node : root_node) {
      assert(node.Layout());
      EXPECT_EQ(static_cast<const DummyNodeLayout&>(*node.Layout()).label, labels[counter]);
      counter++;
    }
    EXPECT_EQ(counter, expected_count);
  }

  void expectNodeCountAndMappedLabels(uint expected_count)
  {
    expectNodeCountAndMappedLabels(screen_graph, expected_count);
  }
};

/**
 * Convenience function.
 */
static bwScreenGraph::LayoutNode& addChildNode(bwScreenGraph::LayoutNode& node, std::string label)
{
  bwScreenGraph::Builder builder(node);
  return builder.addLayout<DummyNodeLayout>(label);
}

TEST_F(IteratorTest, no_children)
{
  // Only screen_graph node.
  expectNodeCountAndMappedLabels(1);
}

TEST_F(IteratorTest, single_child)
{
  addChildNode(screen_graph, labels[1]);
  expectNodeCountAndMappedLabels(2);
}

TEST_F(IteratorTest, multiple_children)
{
  /* screen_graph
   *     /|\
   *    1 2 3
   * Iteration order: screen_graph, 1, 2, 3.
   */
  addChildNode(screen_graph, labels[1]);
  addChildNode(screen_graph, labels[2]);
  addChildNode(screen_graph, labels[3]);
  expectNodeCountAndMappedLabels(4);
}

TEST_F(IteratorTest, subtree)
{
  /* screen_graph
   *      |
   *      7
   *     /
   *    0
   *   /|\
   *  1 2 3
   * Starts iterating at label node with label 0, so: 0, 1, 2, 3.
   */
  bwScreenGraph::LayoutNode& node = addChildNode(screen_graph,
                                                 labels[7]);  // Should not be visitied.
  bwScreenGraph::LayoutNode& other_node = addChildNode(node, labels[0]);
  expectNodeCountAndMappedLabels(other_node, 1);  // Should only visit subtree of other_node.
  addChildNode(other_node, labels[1]);
  addChildNode(other_node, labels[2]);
  expectNodeCountAndMappedLabels(other_node, 3);  // Should only visit subtree of other_node.
}

TEST_F(IteratorTest, multiple_descendants)
{
  /*  screen_graph
   *      / \
   *     1   4
   *    /     \
   *   2       5
   *  /
   * 3
   * Starts iterating at screen_graph node so: screen_graph, 1, 2, 3, 4, 5.
   */
  bwScreenGraph::LayoutNode& node1 = addChildNode(screen_graph, labels[1]);
  bwScreenGraph::LayoutNode& node2 = addChildNode(node1, labels[2]);
  addChildNode(node2, labels[3]);

  bwScreenGraph::LayoutNode& node4 = addChildNode(screen_graph, labels[4]);
  addChildNode(node4, labels[5]);

  expectNodeCountAndMappedLabels(6);
}

TEST_F(IteratorTest, multiple_descendants_mirrored)
{
  /*  screen_graph
   *      / \
   *     4   1
   *    /     \
   *   5       2
   *            \
   *             3
   * Starts iterating at screen_graph node so: screen_graph, 4, 5, 1, 2, 3.
   * Note that node names don't match label indices here.
   */
  bwScreenGraph::LayoutNode& node4 = addChildNode(screen_graph, labels[1]);
  /* node5 = */ addChildNode(node4, labels[2]);

  bwScreenGraph::LayoutNode& node1 = addChildNode(screen_graph, labels[3]);
  bwScreenGraph::LayoutNode& node2 = addChildNode(node1, labels[4]);
  /* node3 = */ addChildNode(node2, labels[5]);

  expectNodeCountAndMappedLabels(6);
}

TEST_F(IteratorTest, multiple_descendants_complex)
{
  /* Variation with more complex subtrees, also heavier on the right.
   *  screen_graph
   *      /|\
   *     / | \
   *    /  |  \
   *   /   |   \
   *  /    |    \
   * 1     3     7
   * |    / \   / \
   * 2   4   6 8   9
   *    /         / \
   *   5         10  11
   *                  \
   *                   12
   * Starts iterating at screen_graph node so: screen_graph, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12.
   */
  bwScreenGraph::LayoutNode& node1 = addChildNode(screen_graph, labels[1]);
  /* node2 = */ addChildNode(node1, labels[2]);

  bwScreenGraph::LayoutNode& node3 = addChildNode(screen_graph, labels[3]);
  bwScreenGraph::LayoutNode& node4 = addChildNode(node3, labels[4]);
  /* node5 = */ addChildNode(node4, labels[5]);
  /* node6 = */ addChildNode(node3, labels[6]);

  bwScreenGraph::LayoutNode& node7 = addChildNode(screen_graph, labels[7]);
  /* node8 = */ addChildNode(node7, labels[8]);
  bwScreenGraph::LayoutNode& node9 = addChildNode(node7, labels[9]);
  /* node10 = */ addChildNode(node9, labels[10]);
  bwScreenGraph::LayoutNode& node11 = addChildNode(node9, labels[11]);
  /* node12 = */ addChildNode(node11, labels[12]);

  expectNodeCountAndMappedLabels(13);
}

TEST_F(IteratorTest, subtree_multiple_descendants)
{
  /* screen_graph
   *      |
   *      7
   *     / \
   *    0   8
   *   / \
   *  1   2
   *     / \
   *    3   4
   * Starts iterating at label node with label 0, so: 0, 1, 2, 3, 4.
   */
  bwScreenGraph::LayoutNode& node = addChildNode(screen_graph,
                                                 labels[7]);  // Should not be visitied.
  bwScreenGraph::LayoutNode& root_node = addChildNode(node, labels[0]);
  addChildNode(root_node, labels[1]);
  bwScreenGraph::LayoutNode& child_node = addChildNode(root_node, labels[2]);
  addChildNode(child_node, labels[3]);
  addChildNode(child_node, labels[4]);

  addChildNode(node, labels[8]);  // Should not be visited.
  expectNodeCountAndMappedLabels(root_node, 5);
}

TEST_F(IteratorTest, subtree_multiple_descendants_variation)
{
  /* Variation to iterate over subtree that is heavier on the right.
   *  screen_graph
   *      |
   *      7
   *     / \
   *    8   0
   *       / \
   *      1   2
   *         / \
   *        3   4
   * Starts iterating at label node with label 0, so: 0, 1, 2, 3, 4.
   */
  bwScreenGraph::LayoutNode& node7 = addChildNode(screen_graph,
                                                  labels[7]);  // Should not be visitied.
  addChildNode(node7, labels[8]);                              // Should not be visited.

  bwScreenGraph::LayoutNode& node0 = addChildNode(node7, labels[0]);
  addChildNode(node0, labels[1]);
  bwScreenGraph::LayoutNode& node2 = addChildNode(node0, labels[2]);
  addChildNode(node2, labels[3]);
  addChildNode(node2, labels[4]);

  expectNodeCountAndMappedLabels(node0, 5);
}
