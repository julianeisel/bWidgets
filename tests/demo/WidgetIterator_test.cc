#if 0
#include "gtest/gtest.h"

#include "bwLabel.h"

#include "Layout.h"

#include "WidgetIterator.h"

using namespace bWidgets;
using namespace bWidgetsDemo;


class WidgetIteratorTest : public ::testing::Test
{
protected:
	bwPtr<RootLayout> layout;
	const std::array<std::string, 15> labels = {
		"Lorem", "ipsum", "dolor", "sit", "amet,", "consectetuer", "adipiscing", "elit.",
		"Aenean", "commodo", "ligula", "eget", "dolor.", "Aenean", "massa."
	};

public:
	WidgetIteratorTest() :
	    layout(bwPtr_new<RootLayout>(100, 100))
	{
		layout->padding = 7;
		layout->item_margin = 5;
	}


	void expectWidgetCountAndLabels(LayoutItem& layout_item, uint expected_count, bool with_hidden = false)
	{
		uint counter = 0;

		assert(labels.size() >= expected_count);
		if (with_hidden) {
			for (bwWidget& widget : WidgetIterator::withHidden(layout_item)) {
				EXPECT_EQ(*static_cast<const bwLabel&>(widget).getLabel(), labels[counter]);
				counter++;
			}
		}
		else {
			for (bwWidget& widget : layout_item) {
				EXPECT_EQ(*static_cast<const bwLabel&>(widget).getLabel(), labels[counter]);
				counter++;
			}
		}
		EXPECT_EQ(counter, expected_count);
	}

	void expectWidgetCountAndLabels(uint expected_count, bool with_hidden = false)
	{
		expectWidgetCountAndLabels(*layout, expected_count, with_hidden);
	}
};


TEST_F(WidgetIteratorTest, empty)
{
	expectWidgetCountAndLabels(0);
}

TEST_F(WidgetIteratorTest, subtree_empty)
{
	auto& col = ColumnLayout::create(*layout);
	auto& row = RowLayout::create(col);
	expectWidgetCountAndLabels(row, 0);

}

TEST_F(WidgetIteratorTest, subtree_empty_parent_not)
{
	auto& col = ColumnLayout::create(*layout);
	// Subtree empty, root tree not.
	col.addWidget(bwPtr_new<bwLabel>(labels[0], 10, 20));
	auto& row = RowLayout::create(col);
	expectWidgetCountAndLabels(row, 0);
}

TEST_F(WidgetIteratorTest, only_non_widget_items)
{
	RowLayout::create(*layout);
	expectWidgetCountAndLabels(0);
	ColumnLayout::create(*layout);
	expectWidgetCountAndLabels(0);
	ColumnLayout::create(*layout);
	expectWidgetCountAndLabels(0);
}

TEST_F(WidgetIteratorTest, subtree_only_non_widget_items)
{
	auto& row = RowLayout::create(*layout);
	RowLayout::create(row);
	expectWidgetCountAndLabels(row, 0);
	ColumnLayout::create(row);
	expectWidgetCountAndLabels(row, 0);
	ColumnLayout::create(row);
	expectWidgetCountAndLabels(row, 0);
}

TEST_F(WidgetIteratorTest, only_non_widget_and_hidden_items)
{
	RowLayout::create(*layout);
	expectWidgetCountAndLabels(0);
	ColumnLayout::create(*layout);
	expectWidgetCountAndLabels(0);

	auto widget = bwPtr_new<bwLabel>(labels[0], 30, 40);
	widget->hidden = true;
	layout->addWidget(std::move(widget));
	ColumnLayout::create(*layout);
	expectWidgetCountAndLabels(0);
}

TEST_F(WidgetIteratorTest, include_hidden_only_non_widget_and_hidden_items)
{
	RowLayout::create(*layout);
	expectWidgetCountAndLabels(0);
	ColumnLayout::create(*layout);
	expectWidgetCountAndLabels(0);

	auto widget = bwPtr_new<bwLabel>(labels[0], 30, 40);
	widget->hidden = true;
	layout->addWidget(std::move(widget));
	ColumnLayout::create(*layout);
	expectWidgetCountAndLabels(1, true);
}

TEST_F(WidgetIteratorTest, single_item)
{
	layout->addWidget(bwPtr_new<bwLabel>(labels[0], 10, 20));
	expectWidgetCountAndLabels(1);
}

TEST_F(WidgetIteratorTest, subtree_single_item)
{
	auto& col = ColumnLayout::create(*layout);
	col.addWidget(bwPtr_new<bwLabel>(labels[0], 10, 20));
	expectWidgetCountAndLabels(col, 1);
}

TEST_F(WidgetIteratorTest, hidden_item)
{
	auto widget = bwPtr_new<bwLabel>(labels[0], 30, 40);
	widget->hidden = true;
	layout->addWidget(std::move(widget));
	expectWidgetCountAndLabels(0);
}

TEST_F(WidgetIteratorTest, include_hidden_item)
{
	auto widget = bwPtr_new<bwLabel>(labels[0], 30, 40);
	widget->hidden = true;
	layout->addWidget(std::move(widget));
	expectWidgetCountAndLabels(1, true);
}

TEST_F(WidgetIteratorTest, multiple_child_widgets)
{
	layout->addWidget(bwPtr_new<bwLabel>(labels[0], 30, 40));
	layout->addWidget(bwPtr_new<bwLabel>(labels[1], 31, 42));
	layout->addWidget(bwPtr_new<bwLabel>(labels[2], 42, 53));

	expectWidgetCountAndLabels(3);

	layout->addWidget(bwPtr_new<bwLabel>(labels[3], 1, 0));
	expectWidgetCountAndLabels(4);
}

TEST_F(WidgetIteratorTest, subtree_multiple_child_widgets)
{
	auto& col = ColumnLayout::create(*layout);

	col.addWidget(bwPtr_new<bwLabel>(labels[0], 30, 40));
	col.addWidget(bwPtr_new<bwLabel>(labels[1], 31, 42));
	col.addWidget(bwPtr_new<bwLabel>(labels[2], 42, 53));

	expectWidgetCountAndLabels(col, 3);

	col.addWidget(bwPtr_new<bwLabel>(labels[3], 1, 0));
	expectWidgetCountAndLabels(col, 4);
}

TEST_F(WidgetIteratorTest, multiple_childs_only_some_widgets)
{
	RowLayout::create(*layout);
	layout->addWidget(bwPtr_new<bwLabel>(labels[0], 30, 40));
	ColumnLayout::create(*layout);
	layout->addWidget(bwPtr_new<bwLabel>(labels[1], 31, 42));
	RowLayout::create(*layout);
	layout->addWidget(bwPtr_new<bwLabel>(labels[2], 42, 53));
	ColumnLayout::create(*layout);

	expectWidgetCountAndLabels(3);

	layout->addWidget(bwPtr_new<bwLabel>(labels[3], 50, 30));
	expectWidgetCountAndLabels(4);
}

TEST_F(WidgetIteratorTest, subtree_multiple_childs_only_some_widgets)
{
	auto& row = RowLayout::create(*layout);

	RowLayout::create(row);
	row.addWidget(bwPtr_new<bwLabel>(labels[0], 30, 40));
	ColumnLayout::create(row);
	row.addWidget(bwPtr_new<bwLabel>(labels[1], 31, 42));
	RowLayout::create(row);
	row.addWidget(bwPtr_new<bwLabel>(labels[2], 42, 53));
	ColumnLayout::create(row);

	expectWidgetCountAndLabels(row, 3);

	row.addWidget(bwPtr_new<bwLabel>(labels[3], 50, 30));
	expectWidgetCountAndLabels(row, 4);
}

TEST_F(WidgetIteratorTest, grand_child_widgets)
{
	RowLayout& row = RowLayout::create(*layout);
	row.addWidget(bwPtr_new<bwLabel>(labels[0], 30, 40));

	expectWidgetCountAndLabels(1);

	row.addWidget(bwPtr_new<bwLabel>(labels[1], 30, 40));
	expectWidgetCountAndLabels(2);
	row.addWidget(bwPtr_new<bwLabel>(labels[2], 30, 40));
	expectWidgetCountAndLabels(3);
}

TEST_F(WidgetIteratorTest, subtree_grand_child_widgets)
{
	auto& col = ColumnLayout::create(*layout);
	auto& row = RowLayout::create(col);

	RowLayout& subrow = RowLayout::create(row);
	subrow.addWidget(bwPtr_new<bwLabel>(labels[0], 30, 40));

	expectWidgetCountAndLabels(row, 1);

	subrow.addWidget(bwPtr_new<bwLabel>(labels[1], 30, 40));
	expectWidgetCountAndLabels(row, 2);
	subrow.addWidget(bwPtr_new<bwLabel>(labels[2], 30, 40));
	expectWidgetCountAndLabels(row, 3);
}

TEST_F(WidgetIteratorTest, grand_child_widgets_with_hidden)
{
	RowLayout& row = RowLayout::create(*layout);

	auto widget = bwPtr_new<bwLabel>(labels[0], 10, 20);
	widget->hidden = true;
	row.addWidget(std::move(widget));
	expectWidgetCountAndLabels(0);

	row.addWidget(bwPtr_new<bwLabel>(labels[0], 30, 40));
	expectWidgetCountAndLabels(1);

	widget = bwPtr_new<bwLabel>(labels[1], 10, 20);
	widget->hidden = true;
	row.addWidget(std::move(widget));
	expectWidgetCountAndLabels(1);

	row.addWidget(bwPtr_new<bwLabel>(labels[1], 30, 40));
	expectWidgetCountAndLabels(2);
	row.addWidget(bwPtr_new<bwLabel>(labels[2], 30, 40));
	expectWidgetCountAndLabels(3);

	widget = bwPtr_new<bwLabel>(labels[0], 10, 20);
	widget->hidden = true;
	row.addWidget(std::move(widget));
	expectWidgetCountAndLabels(3);
}

TEST_F(WidgetIteratorTest, include_hidden_grand_child_widgets_with_hidden)
{
	RowLayout& row = RowLayout::create(*layout);

	auto widget = bwPtr_new<bwLabel>(labels[0], 10, 20);
	widget->hidden = true;
	row.addWidget(std::move(widget));
	expectWidgetCountAndLabels(1, true);

	row.addWidget(bwPtr_new<bwLabel>(labels[1], 30, 40));
	expectWidgetCountAndLabels(2, true);

	widget = bwPtr_new<bwLabel>(labels[2], 10, 20);
	widget->hidden = true;
	row.addWidget(std::move(widget));
	expectWidgetCountAndLabels(3, true);

	row.addWidget(bwPtr_new<bwLabel>(labels[3], 30, 40));
	expectWidgetCountAndLabels(4, true);
	row.addWidget(bwPtr_new<bwLabel>(labels[4], 30, 40));
	expectWidgetCountAndLabels(5, true);

	widget = bwPtr_new<bwLabel>(labels[5], 10, 20);
	widget->hidden = true;
	row.addWidget(std::move(widget));
	expectWidgetCountAndLabels(6, true);
}

TEST_F(WidgetIteratorTest, anchestor_widgets)
{
	RowLayout& row = RowLayout::create(*layout);
	expectWidgetCountAndLabels(0);

	/* nested layout */

	ColumnLayout& col = ColumnLayout::create(row);
	expectWidgetCountAndLabels(0);

	RowLayout& nested_row = RowLayout::create(col);
	nested_row.addWidget(bwPtr_new<bwLabel>(labels[0], 30, 40));
	expectWidgetCountAndLabels(1);

	ColumnLayout& nested_col = ColumnLayout::create(col);
	nested_col.addWidget(bwPtr_new<bwLabel>(labels[1], 30, 40));
	expectWidgetCountAndLabels(2);
}

TEST_F(WidgetIteratorTest, anchestor_widgets_with_hidden)
{
	RowLayout& row = RowLayout::create(*layout);
	expectWidgetCountAndLabels(0);

	/* nested layout */

	ColumnLayout& col = ColumnLayout::create(row);
	expectWidgetCountAndLabels(0);

	RowLayout& nested_row = RowLayout::create(col);
	nested_row.addWidget(bwPtr_new<bwLabel>(labels[0], 30, 40));
	expectWidgetCountAndLabels(1);

	auto widget = bwPtr_new<bwLabel>(labels[0], 10, 20);
	widget->hidden = true;
	nested_row.addWidget(std::move(widget));
	expectWidgetCountAndLabels(1);

	ColumnLayout& nested_col = ColumnLayout::create(col);
	nested_col.addWidget(bwPtr_new<bwLabel>(labels[1], 30, 40));
	expectWidgetCountAndLabels(2);
}

TEST_F(WidgetIteratorTest, widget_with_anchestor_widgets_with_hidden)
{
	RowLayout& row = RowLayout::create(*layout);
	expectWidgetCountAndLabels(0);

	row.addWidget(bwPtr_new<bwLabel>(labels[0], 30, 40));
	expectWidgetCountAndLabels(1);

	/* nested layout */

	PanelLayout& panel = PanelLayout::create(labels[1], 10, row);
	expectWidgetCountAndLabels(2);

	RowLayout& nested_row = RowLayout::create(panel);
	nested_row.addWidget(bwPtr_new<bwLabel>(labels[2], 30, 40));
	expectWidgetCountAndLabels(3);

	auto widget = bwPtr_new<bwLabel>(labels[0], 10, 20);
	widget->hidden = true;
	nested_row.addWidget(std::move(widget));
	expectWidgetCountAndLabels(3);

	ColumnLayout& nested_col = ColumnLayout::create(panel);
	nested_col.addWidget(bwPtr_new<bwLabel>(labels[3], 30, 40));
	expectWidgetCountAndLabels(4);
}

TEST_F(WidgetIteratorTest, include_hidden_anchestor_widgets_with_hidden)
{
	RowLayout& row = RowLayout::create(*layout);
	expectWidgetCountAndLabels(0);

	/* nested layout */

	ColumnLayout& col = ColumnLayout::create(row);
	expectWidgetCountAndLabels(0, true);

	RowLayout& nested_row = RowLayout::create(col);
	nested_row.addWidget(bwPtr_new<bwLabel>(labels[0], 30, 40));
	expectWidgetCountAndLabels(1, true);

	auto widget = bwPtr_new<bwLabel>(labels[1], 10, 20);
	widget->hidden = true;
	nested_row.addWidget(std::move(widget));
	expectWidgetCountAndLabels(2, true);

	ColumnLayout& nested_col = ColumnLayout::create(col);
	nested_col.addWidget(bwPtr_new<bwLabel>(labels[2], 30, 40));
	expectWidgetCountAndLabels(3, true);
}

TEST_F(WidgetIteratorTest, subtree_filled)
{
	const std::string label_never = "label never";

	RowLayout* row = &RowLayout::create(*layout);

	row->addWidget(bwPtr_new<bwLabel>(label_never, 10, 20));

	ColumnLayout& col = ColumnLayout::create(*row);
	col.addWidget(bwPtr_new<bwLabel>(labels[0], 10, 20));

	row = &RowLayout::create(col);
	row->addWidget(bwPtr_new<bwLabel>(labels[1], 10, 20));

	// Expect only subtree be traversed, we should traverse higher than col.
	expectWidgetCountAndLabels(col, 2);
	bool found_label0 = false;
	bool found_label1 = false;
	for (bwWidget& iter_widget : col) {
		if (*iter_widget.getLabel() == labels[0]) {
			found_label0 = true;
		}
		else if (*iter_widget.getLabel() == labels[1]) {
			found_label1 = true;
		}
		EXPECT_NE(*iter_widget.getLabel(), label_never);
	}
	EXPECT_TRUE(found_label0);
	EXPECT_TRUE(found_label1);
}

TEST_F(WidgetIteratorTest, widget_with_childs)
{
	layout->addWidget(bwPtr_new<bwLabel>(labels[0], 10, 20));

	PanelLayout& panel = PanelLayout::create(labels[1], 30, *layout);
	panel.addWidget(bwPtr_new<bwLabel>(labels[2], 10, 20));
	panel.addWidget(bwPtr_new<bwLabel>(labels[3], 10, 20));
	expectWidgetCountAndLabels(*layout, 4);
}

TEST_F(WidgetIteratorTest, subtree_widget_with_childs)
{
	layout->addWidget(bwPtr_new<bwLabel>(labels[0], 10, 20));

	PanelLayout& panel = PanelLayout::create(labels[0], 30, *layout);
	panel.addWidget(bwPtr_new<bwLabel>(labels[1], 10, 20));

	panel.addWidget(bwPtr_new<bwLabel>(labels[2], 10, 20));
	expectWidgetCountAndLabels(panel, 3);
}

#endif
