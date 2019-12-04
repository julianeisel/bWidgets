#include "gtest/gtest.h"

#include "bwPoint.h"
#include "bwPolygon.h"

using namespace bWidgets;

static unsigned int getRandomVertexCount(unsigned int min, unsigned int max)
{
  srand(time(nullptr));
  return (rand() % (max - min)) + min;
}

static bwPolygon createRandomPolygon(unsigned int vert_count)
{
  bwPolygon poly;

  for (int i = 0; i < vert_count; i++) {
    poly.addVertex(i, i);
  }

  return poly;
}

TEST(bwPolygon, Constructor_empty)
{
  bwPolygon poly;
  bwPointVec vertices = poly.getVertices();

  EXPECT_EQ(vertices.size(), 0);
}

TEST(bwPolygon, Constructor_copy)
{
  unsigned int vert_count = getRandomVertexCount(0, 100);
  bwPolygon poly_original = createRandomPolygon(vert_count);
  bwPolygon poly_copy = poly_original;
  bwPointVec vertices = poly_copy.getVertices();

  EXPECT_EQ(vertices.size(), vert_count);
}

TEST(bwPolygon, Constructor_vertex_list)
{
  bwPointVec verts_original = {bwPoint(5, 5), bwPoint(-1, -2), bwPoint(42, 42)};
  bwPolygon poly(verts_original);

  // Compares items individually
  EXPECT_EQ(poly.getVertices(), verts_original);
}

TEST(bwPolygon, addVertex)
{
  unsigned int vert_count = getRandomVertexCount(0, 100);
  bwPolygon poly = createRandomPolygon(vert_count);
  bwPointVec vertices = poly.getVertices();

  EXPECT_EQ(vertices.size(), vert_count);
}

TEST(bwPolygon, vertex_random_access)
{
  unsigned int vert_count = getRandomVertexCount(0, 100);
  bwPolygon poly;

  // Does what createRandomPolygon does, but we want to know how
  // vertex coordinates are set exactly. So better not rely on it.
  for (int i = 0; i < vert_count; i++) {
    poly.addVertex(i, i);
  }

  bwPointVec vertices = poly.getVertices();
  for (int i = 0; i < vert_count; i++) {
    EXPECT_EQ(vertices[i], bwPoint(i, i));
  }
}

TEST(bwPolygon, isDrawable)
{
  unsigned int vert_count = getRandomVertexCount(1, 100);
  bwPolygon poly;

  EXPECT_FALSE(poly.isDrawable());

  poly = createRandomPolygon(vert_count);
  EXPECT_TRUE(poly.isDrawable());
}
