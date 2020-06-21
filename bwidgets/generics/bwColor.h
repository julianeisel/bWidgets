#pragma once

namespace bWidgets {

class bwColor {
 public:
  bwColor(float red, float green, float blue, float alpha = 1.0f);
  bwColor(float rgb, float alpha = 1.0f);
  bwColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha = 255);
  bwColor(unsigned int rgb, unsigned int alpha = 255);
  bwColor() = default;
  ~bwColor() = default;
  bwColor(const bwColor&);

  auto shade(float rgb_shade, float alpha_shade = 0.0f) -> bwColor&;
  auto shade(unsigned int rgb_shade, unsigned int alpha_shade = 0.0f) -> bwColor&;

  void setColor(float red, float green, float blue, float alpha = 1.0f);
  void setColor(float rgb, float alpha = 1.0f);
  void setColor(const float rgba[4]);
  auto getColor() const -> const float*;
  auto operator=(const float* rgb) -> bwColor&;
  auto operator=(const bwColor& other_color) -> bwColor&;

  auto operator==(const bwColor& compare_color) const -> bool;
  auto operator[](const int index) -> float&;
  // Implicit conversion to float*
  operator const float*() const;

 private:
  enum class Component {
    RED = 0,
    GREEN = 1,
    BLUE = 2,
    ALPHA = 3,
  };

  auto operator[](const Component component) -> float&
  {
    return rgba[static_cast<int>(component)];
  }

  void clamp(const Component component);

  float rgba[4]{0, 0, 0, 1};
};

}  // namespace bWidgets
