#include "DropDownSelector.hpp"
#include "Button.hpp"
#include "containers/OverlayContainer.hpp"
#include "TextWidget.hpp"
#include "labels/Label.hpp"

using namespace morda;



namespace{

const char* DLayout = R"qwertyuiop(
		Label{
			name{morda_dropdown_selection}
			layout{
				dimX{0} dimY{min}
				weight{1}
			}
			text{stuff}
		}
		PushButton{
			name{morda_dropdown_button}
			Label{
				text{\/}
			}
		}
	)qwertyuiop";

}


DropDownSelector::DropDownSelector(const stob::Node* chain) :
		Widget(chain),
		HorizontalContainer(stob::parse(DLayout).get())
{
	auto b = this->findChildByNameAs<PushButton>("morda_dropdown_button");
	b->clicked = [this](PushButton& b){
		auto p = this->findAncestor<OverlayContainer>();
		if(!p){
			TRACE(<< "DropDownSelector: no OverlayContainer parent found" << std::endl)
			return;
		}
		
		auto w = utki::makeShared<Label>();
		w->setText("context menu");
		
		p->showContextMenu(w, this->calcPosInParent(Vec2r(0), p));
	};
}
