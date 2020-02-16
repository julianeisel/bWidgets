#pragma once

#include <string>

#include "bwAbstractButton.h"

namespace bWidgets {

/**
 * \brief A radio-button like widget retaining a pressed state once pressed.
 *
 * \note: The way this works right now is not that nice. It's however made so it can work nicely
 * with Blender.
 *
 * Normally, there's a group of radio buttons, once one of them is pressed, the others get
 * un-pressed. It's an exclusive behavior: There should only ever be one pressed radio button in
 * the group. bWidgets doesn't ensure this behavior at all as of now. The host application needs to
 * do this. In future, there should be a group-widget that ensures the exclusive behavior within
 * bWidgets. We need to make Blender ready for it first, though (or make it optional).
 */
class bwRadioButton : public bwAbstractButton {
 public:
  bwRadioButton(const std::string& text,
                unsigned int width_hint = 0,
                unsigned int height_hint = 0);

  bool canAlign() const override;

  bwPtr<bwScreenGraph::EventHandler> createHandler() override;
};

}  // namespace bWidgets
