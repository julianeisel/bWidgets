#pragma once

#include <string>

#include "bwAbstractButton.h"


namespace bWidgets {

class bwPushButton : public bwAbstractButton
{
public:
	bwPushButton(
	        const std::string& text,
	        unsigned int width_hint = 0, unsigned int height_hint = 0);

	const class bwIconInterface* getIcon() const override;
	bwPushButton& setIcon(const class bwIconInterface&);

private:
	const class bwIconInterface* icon{nullptr};
};

} // namespace bWidgets
