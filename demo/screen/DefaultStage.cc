/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Original work Copyright (c) 2018 Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include <cassert>

#include "blender_icon_defines.h"

#include "IconMap.h"
#include "Layout.h"

#include "bwCheckbox.h"
#include "bwLabel.h"
#include "bwNumberSlider.h"
#include "bwPushButton.h"
#include "bwRadioButton.h"
#include "bwStyleManager.h"

#include "DefaultStage.h"

using namespace bWidgetsDemo;
using namespace bWidgets; // Less verbose


#define BUTTON_HEIGHT       20
#define PANEL_HEADER_HEIGHT 24

namespace bWidgetsDemo {

class ScaleSetter : public bwFunctorInterface
{
public:
	ScaleSetter(const bwNumberSlider&);
	void operator()() override;

private:
	const bwNumberSlider& numslider;
};

class StyleSetter : public bwFunctorInterface
{
public:
	StyleSetter(DefaultStage& stage, const bwStyle::StyleType& style_type);
	static bool StyleButtonsUpdateCb(bwWidget&, void*);
	void operator()() override;

private:
	DefaultStage& stage;
	const bwStyle::StyleType& style_type;
};

class UseCSSVersionToggleSetter : public bwFunctorInterface
{
public:
	UseCSSVersionToggleSetter(const bwCheckbox&, DefaultStage&);
	void operator()() override;

private:
	const bwCheckbox& checkbox;
	DefaultStage& stage;
};

class UseFontHintingToggleSetter : public bwFunctorInterface
{
public:
	UseFontHintingToggleSetter(const bwCheckbox& _checkbox) : checkbox(_checkbox) {}

	void operator()() override
	{
		Stage::setFontHinting(checkbox.isChecked());
	}

private:
	const bwCheckbox& checkbox;
};

class UseFontSubPixelPositioningToggleSetter : public bwFunctorInterface
{
public:
	UseFontSubPixelPositioningToggleSetter(const bwCheckbox& _checkbox) : checkbox(_checkbox) {}

	void operator()() override
	{
		Stage::setFontSubPixelPositioning(checkbox.isChecked());
	}

private:
	const bwCheckbox& checkbox;
};

class UseFontSubPixelsToggleSetter : public bwFunctorInterface
{
public:
	UseFontSubPixelsToggleSetter(const bwCheckbox& _checkbox, const Stage& _stage) :
	    checkbox(_checkbox), stage(_stage)
	{}

	void operator()() override
	{
		Stage::setFontAntiAliasingMode(checkbox.isChecked() ? Font::SUBPIXEL_LCD_RGB_COVERAGE : Font::NORMAL_COVERAGE);
		stage.layout->iterateWidgets([](bwWidget& widget, void* customdata){
			if (auto* iter_checkbox = widget_cast<bwCheckbox*>(&widget)) {
				if (iter_checkbox->apply_functor && dynamic_cast<UseFontSubPixelPositioningToggleSetter*>(iter_checkbox->apply_functor.get())) {
					const auto& checkbox = *static_cast<bwCheckbox*>(customdata);
					iter_checkbox->hidden = !checkbox.isChecked();
				}
			}
			return true;
		}, const_cast<bwCheckbox*>(&checkbox));
	}

private:
	const bwCheckbox& checkbox;
	const Stage& stage;
};

} // namespace bWidgetsDemo


// --------------------------------------------------------------------

DefaultStage::DefaultStage(unsigned int mask_width, unsigned int mask_height) :
    Stage(mask_width, mask_height)
{
	addStyleSelector(*layout);

	auto slider = bwPtr_new<bwNumberSlider>(0, BUTTON_HEIGHT);
	slider->apply_functor = bwPtr_new<ScaleSetter>(*slider);
	slider->setText("Interface Scale: ");
	slider->setMinMax(0.5f, 2.0f);
	slider->setValue(1.0f);
	layout->addWidget(std::move(slider));


	layout->addWidget(bwPtr_new<bwLabel>("Font Rendering:", 0, BUTTON_HEIGHT));
	auto checkbox = bwPtr_new<bwCheckbox>("Hinting", 0, BUTTON_HEIGHT);

	RowLayout* row = &RowLayout::create(*layout, true);
	checkbox->apply_functor = bwPtr_new<UseFontHintingToggleSetter>(*checkbox);
	row->addWidget(std::move(checkbox));

	row = &RowLayout::create(*layout, false);
	checkbox = bwPtr_new<bwCheckbox>("Subpixel Rendering", 0, BUTTON_HEIGHT);
	checkbox->apply_functor = bwPtr_new<UseFontSubPixelsToggleSetter>(*checkbox, *this);
	row->addWidget(std::move(checkbox));
	checkbox = bwPtr_new<bwCheckbox>("Subpixel Positioning", 0, BUTTON_HEIGHT);
	checkbox->apply_functor = bwPtr_new<UseFontSubPixelPositioningToggleSetter>(*checkbox);
	checkbox->hidden = true;
	row->addWidget(std::move(checkbox));

	addFakeSpacer(*layout);


	PanelLayout* panel = &PanelLayout::create("Some Testing Widgets", PANEL_HEADER_HEIGHT, *layout);

	ColumnLayout& col = ColumnLayout::create(*panel, true);
	col.addWidget(bwPtr_new<bwPushButton>("Translate", 0, BUTTON_HEIGHT));
	col.addWidget(bwPtr_new<bwPushButton>("Rotate", 0, BUTTON_HEIGHT));
	col.addWidget(bwPtr_new<bwPushButton>("Scale", 0, BUTTON_HEIGHT));

	panel->addWidget(bwPtr_new<bwPushButton>("Mirror", 0, BUTTON_HEIGHT));


	panel = &PanelLayout::create("More Testing...", PANEL_HEADER_HEIGHT, *layout);
	row = &RowLayout::create(*panel, true);
	row->addWidget(bwPtr_new<bwCheckbox>("Make Awesome", 0, BUTTON_HEIGHT));
	row->addWidget(bwPtr_new<bwCheckbox>("Wireframes", 0, BUTTON_HEIGHT));

	auto text_box = bwPtr_new<bwTextBox>(0, BUTTON_HEIGHT);
	text_box->setText("Some Text...");
	panel->addWidget(std::move(text_box));

	row = &RowLayout::create(*panel, false);
	auto label = bwPtr_new<bwLabel>("Pose Icon", 0, BUTTON_HEIGHT);
	label->setIcon(icon_map->getIcon(ICON_POSE_HLT));
	row->addWidget(std::move(label));

	label = bwPtr_new<bwLabel>("Normalized FCurve Icon", 0, BUTTON_HEIGHT);
	label->setIcon(icon_map->getIcon(ICON_NORMALIZE_FCURVES));
	row->addWidget(std::move(label));

	label = bwPtr_new<bwLabel>("Chroma Scope Icon", 0, BUTTON_HEIGHT);
	label->setIcon(icon_map->getIcon(ICON_SEQ_CHROMA_SCOPE));
	row->addWidget(std::move(label));
}

bool isUseCSSVersionToggleHidden(const bwStyle& style)
{
	return (style.type_id != bwStyle::STYLE_CLASSIC) && (style.type_id != bwStyle::STYLE_CLASSIC_CSS);
}

void DefaultStage::activateStyleID(bwStyle::StyleTypeID type_id)
{
	Stage::activateStyleID(type_id);
	layout->iterateWidgets([](bwWidget& widget, void* /*customdata*/){
		if (auto* checkbox = widget_cast<bwCheckbox*>(&widget)) {
			if (checkbox->apply_functor && dynamic_cast<UseCSSVersionToggleSetter*>(checkbox->apply_functor.get())) {
				widget.hidden = isUseCSSVersionToggleHidden(*Stage::style);
			}
		}
		return true;
	}, nullptr);
}

void DefaultStage::addStyleSelector(LayoutItem& parent_layout)
{
	RowLayout& row_layout = RowLayout::create(parent_layout, true);

	auto label = bwPtr_new<bwLabel>("Style:", 0, BUTTON_HEIGHT);
	label->setIcon(icon_map->getIcon(ICON_BLENDER));
	row_layout.addWidget(std::move(label));

	for (const bwStyle::StyleType& type : bwStyleManager::getStyleManager().getBuiltinStyleTypes()) {
		if (type.type_id == bwStyle::STYLE_CLASSIC_CSS) {
			// We'll add a button for this later.
			continue;
		}
		auto style_button = bwPtr_new<bwRadioButton>(type.name, 0, BUTTON_HEIGHT);

		style_button->apply_functor = bwPtr_new<StyleSetter>(*this, type);

		if (type.type_id == style->type_id) {
			style_button->state = bwAbstractButton::STATE_SUNKEN;
		}

		row_layout.addWidget(std::move(style_button));
	}

	auto checkbox = bwPtr_new<bwCheckbox>("Use CSS Version", 0, BUTTON_HEIGHT);
	checkbox->hidden = isUseCSSVersionToggleHidden(*style);
	checkbox->apply_functor = bwPtr_new<UseCSSVersionToggleSetter>(*checkbox, *this);
	parent_layout.addWidget(std::move(checkbox));
}

void DefaultStage::addFakeSpacer(LayoutItem& layout)
{
	// Just some extra space. No spacer widgets yet.
	layout.addWidget(bwPtr_new<bwLabel>("", 0, 7));
}

void DefaultStage::useStyleCSSVersionSet(
        const bool use_css_version)
{
	bwStyle::StyleTypeID active_type_id = style->type_id;

	assert(active_type_id == bwStyle::STYLE_CLASSIC || active_type_id == bwStyle::STYLE_CLASSIC_CSS);
	if (use_css_version) {
		if (active_type_id != bwStyle::STYLE_CLASSIC_CSS) {
			activateStyleID(bwStyle::STYLE_CLASSIC_CSS);
		}
	}
	else {
		if (active_type_id != bwStyle::STYLE_CLASSIC) {
			activateStyleID(bwStyle::STYLE_CLASSIC);
		}
	}
}

// --------------------------------------------------------------------
// Functor definitions

ScaleSetter::ScaleSetter(const bwNumberSlider& numslider) :
    numslider(numslider)
{
	
}

void ScaleSetter::operator()()
{
	Stage::setInterfaceScale(numslider.getValue());
}

StyleSetter::StyleSetter(
        DefaultStage& stage,
        const bwStyle::StyleType& style_type) :
    stage(stage), style_type(style_type)
{
	
}

bool StyleSetter::StyleButtonsUpdateCb(
        bwWidget& widget_iter,
        void* _stage)
{
	auto& stage = *static_cast<DefaultStage*>(_stage);
	auto* radio_iter = widget_cast<bwRadioButton*>(&widget_iter);
	bwStyle::StyleTypeID active_type_id = stage.style->type_id;

	if (radio_iter && radio_iter->apply_functor) {
		// Using dynamic_cast to check if apply_functor is a StyleSetter. Then we assume it's a style button.
		if (auto iter_style_setter = dynamic_cast<StyleSetter*>(radio_iter->apply_functor.get())) {
			if (iter_style_setter->style_type.type_id == active_type_id) {
				radio_iter->state = bwWidget::STATE_SUNKEN;
			}
			else {
				radio_iter->state = bwWidget::STATE_NORMAL;
			}
		}
	}
	else if (auto* checkbox_iter = widget_cast<bwCheckbox*>(&widget_iter)) {
		if (checkbox_iter->apply_functor &&
		    dynamic_cast<UseCSSVersionToggleSetter*>(checkbox_iter->apply_functor.get()))
		{
			if (active_type_id == bwStyle::STYLE_CLASSIC || active_type_id == bwStyle::STYLE_CLASSIC_CSS) {
				stage.useStyleCSSVersionSet(checkbox_iter->state == bwWidget::STATE_SUNKEN);
			}
		}
	}

	return true;
}

void StyleSetter::operator()()
{
	bwStyle::StyleTypeID style_type_id = style_type.type_id;
	stage.activateStyleID(style_type_id);
	// Deactivate other style radio buttons
	stage.layout->iterateWidgets(StyleButtonsUpdateCb, &stage);
}

UseCSSVersionToggleSetter::UseCSSVersionToggleSetter(
        const bwCheckbox& checkbox,
        DefaultStage& stage) :
    checkbox(checkbox), stage(stage)
{
	
}

void UseCSSVersionToggleSetter::operator()()
{
	stage.useStyleCSSVersionSet(checkbox.state == bwWidget::STATE_SUNKEN);
}
