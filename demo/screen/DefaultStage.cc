#include <cassert>

#include "Layout.h"

#include "bwCheckbox.h"
#include "bwLabel.h"
#include "bwNumberSlider.h"
#include "bwPushButton.h"
#include "bwStyleCSS.h"
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

} // namespace bWidgetsDemo


// --------------------------------------------------------------------

DefaultStage::DefaultStage(unsigned int mask_width, unsigned int mask_height) :
    Stage(mask_width, mask_height)
{
	addStyleSelector(*layout);

	bwNumberSlider* slider = new bwNumberSlider(0, BUTTON_HEIGHT);
	slider->apply_functor = bwPointer_new<ScaleSetter>(*slider);
	slider->setText("Interface Scale: ");
	slider->setMinMax(0.5f, 2.0f);
	slider->setValue(1.0f);
	layout->addWidget(slider);

	addFakeSpacer(*layout);


	PanelLayout* panel = new PanelLayout("Some Testing Widgets", PANEL_HEADER_HEIGHT, layout);

	ColumnLayout* col = new ColumnLayout(true, panel);
	col->addWidget(new bwPushButton("Translate", 0, BUTTON_HEIGHT));
	col->addWidget(new bwPushButton("Rotate", 0, BUTTON_HEIGHT));
	col->addWidget(new bwPushButton("Scale", 0, BUTTON_HEIGHT));

	panel->addWidget(new bwPushButton("Mirror", 0, BUTTON_HEIGHT));


	panel = new PanelLayout("More Testing...", PANEL_HEADER_HEIGHT, layout);
	RowLayout* row = new RowLayout(true, panel);
	row->addWidget(new bwCheckbox("Make Awesome", 0, BUTTON_HEIGHT));
	row->addWidget(new bwCheckbox("Wireframes", 0, BUTTON_HEIGHT));

	bwTextBox* text_box = new bwTextBox(0, BUTTON_HEIGHT);
	text_box->setText("Some Text...");
	panel->addWidget(text_box);
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
	RowLayout* row_layout = new RowLayout(true, &parent_layout);

	row_layout->addWidget(new bwLabel("Style:", 0, BUTTON_HEIGHT));

	for (const bwStyle::StyleType& type : bwStyleManager::getStyleManager().getBuiltinStyleTypes()) {
		if (type.type_id == bwStyle::STYLE_CLASSIC_CSS) {
			// We'll add a button for this later.
			continue;
		}
		bwRadioButton* style_button = new bwRadioButton(type.name, 0, BUTTON_HEIGHT);

		style_button->apply_functor = bwPointer_new<StyleSetter>(*this, type);

		if (type.type_id == style->type_id) {
			style_button->state = bwAbstractButton::STATE_SUNKEN;
		}

		row_layout->addWidget(style_button);
	}

	bwCheckbox* checkbox = new bwCheckbox("Use CSS Version", 0, BUTTON_HEIGHT);
	checkbox->hidden = isUseCSSVersionToggleHidden(*style);
	checkbox->apply_functor = bwPointer_new<UseCSSVersionToggleSetter>(*checkbox, *this);
	parent_layout.addWidget(checkbox);
}

void DefaultStage::addFakeSpacer(LayoutItem& layout)
{
	// Just some extra space. No spacer widgets yet.
	layout.addWidget(new bwLabel("", 0, 7));
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
