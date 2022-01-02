#pragma once

#include "bwAbstractButton.h"
#include "data_binding/Binding.h"

namespace bWidgets {

class bwCheckbox : public bwAbstractButton {
  Binding<bool> value_binding_;

 public:
  using DataBindingType = bool;

  bwCheckbox(Binding<bool> binding,
             const std::string& text = "",
             std::optional<unsigned int> width_hint = std::nullopt,
             std::optional<unsigned int> height_hint = std::nullopt);

  auto getTypeIdentifier() const -> std::string_view override;

  void draw(class bwStyle& style) override;

  auto matches(const bwWidget&) const -> std::optional<bool> override;
  auto getBinding() const -> std::optional<TypeErasedBinding> override;

  auto createHandler(bwScreenGraph::Node& node) const
      -> std::unique_ptr<bwScreenGraph::EventHandler> override;

  auto setChecked(bool checked = true) -> bwCheckbox&;
  auto isChecked() const -> bool;

 private:
  auto isInsideCheckbox(const bwPoint& point) const -> bool;

  auto getCheckboxRectangle() const -> bwRectanglePixel;
  auto getTextRectangle(const bwRectanglePixel& checkbox_rectangle) const -> bwRectanglePixel;
};

}  // namespace bWidgets
