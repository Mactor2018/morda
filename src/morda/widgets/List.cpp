#include "List.hpp"

#include "../App.hpp"


using namespace morda;




namespace{

class StaticProvider : public List::ItemsProvider{
	std::vector<std::shared_ptr<Widget>> widgets;
public:

	size_t count() const NOEXCEPT override{
		return this->widgets.size();
	}
	
	std::shared_ptr<Widget> getWidget(size_t index)override{
//		TRACE(<< "StaticProvider::getWidget(): index = " << index << std::endl)
		//TODO: inflate new widget each time
		return this->widgets[index];
	}
	
	void add(std::shared_ptr<Widget> w){
		this->widgets.push_back(std::move(w));
	}
};

}




List::List(bool isVertical, const stob::Node* chain):
		Widget(chain),
		isVertical(isVertical)
{
	if(!chain){
		return;
	}
	
	const stob::Node* n = chain->ThisOrNextNonProperty().node();
	
	if(!n){
		return;
	}
	
	std::shared_ptr<StaticProvider> p = ting::New<StaticProvider>();
	
	for(; n; n = n->NextNonProperty().node()){
		p->add(morda::App::Inst().inflater.Inflate(*n));
	}
	
	this->setItemsProvider(std::move(p));
}



void List::OnResize() {
	this->numTailItems = 0;//means that it needs to be recomputed
	
	this->updateChildrenList();
}


void List::setItemsProvider(std::shared_ptr<ItemsProvider> provider){
	if(this->provider){
		this->provider->list = nullptr;
	}
	this->provider = std::move(provider);
	if(this->provider){
		this->provider->list = this;
	}
	this->notifyDataSetChanged();
}



real List::scrollFactor()const NOEXCEPT{
	return real(this->posIndex) / real(this->count() - this->visibleCount());
}


void List::setScrollPosAsFactor(real factor){
	if(this->numTailItems == 0){
		this->updateTailItemsInfo();
	}
	
	this->posIndex = factor * real(this->count() - this->numTailItems);
	
	if(this->count() != this->numTailItems){
		real intFactor = real(this->posIndex) / real(this->count() - this->numTailItems);

		if(this->Children().size() != 0){
			real d;
			if(this->isVertical){
				d = this->Children().front()->Rect().d.y;
			}else{
				d = this->Children().front()->Rect().d.x;
			}
			
			this->posOffset = ting::math::Round(d * (factor - intFactor) * real(this->count() - this->numTailItems) + factor * this->firstTailItemOffset);
		}else{
			this->posOffset = 0;
		}
	}else{
		ASSERT(this->posIndex == 0)
		this->posOffset = ting::math::Round(factor * this->firstTailItemOffset);
	}
	
	this->updateChildrenList();
}

void List::updateChildrenList(){
	if(!this->provider){
		this->posIndex = 0;
		this->posOffset = 0;
		
		this->removeAll();
		this->addedIndex = 0;
		return;
	}
	
	if(this->numTailItems == 0){
		this->updateTailItemsInfo();
	}
	
	
	
	
	this->removeAll();
	this->addedIndex = 0;
	
	real pos;
	
	if(this->isVertical){
		pos = this->Rect().d.y + this->posOffset;
	}else{
		pos = -this->posOffset;
	}
	
	for(size_t i = this->posIndex; i < this->count(); ++i){
		auto w = this->provider->getWidget(i);
		
		auto& lp = this->GetLayoutParamsAs<LayoutParams>(*w);
		
		Vec2r dim = this->dimForWidget(*w, lp);
		
		w->Resize(dim);
		
		if(this->isVertical){
			w->MoveTo(Vec2r(0, pos - w->Rect().d.y));
			pos -= w->Rect().d.y;
			
			if(pos < this->Rect().d.y){
				this->Add(w);
			}
			
			if(w->Rect().p.y <= 0){
				break;
			}
		}else{
			w->MoveTo(Vec2r(pos, 0));
			pos += w->Rect().d.x;
			
			if(pos > 0){
				this->Add(w);
			}
			
			if(w->Rect().Right() >= this->Rect().d.x){
				break;
			}
		}
	}
}



void List::updateTailItemsInfo(){
	this->numTailItems = 0;
	
	if(!this->provider || this->provider->count() == 0){
		return;
	}
	
	real dim;
	
	if(this->isVertical){
		dim = this->Rect().d.y;
	}else{
		dim = this->Rect().d.x;
	}
	
	ASSERT(this->provider)
	ASSERT(this->provider->count() > 0)
	
	for(size_t i = this->provider->count(); i != 0 && dim > 0; --i){
		++this->numTailItems;
		
		auto w = this->provider->getWidget(i - 1);
		
		auto& lp = this->GetLayoutParamsAs<LayoutParams>(*w);
		
		Vec2r d = this->dimForWidget(*w, lp);
		
		if(this->isVertical){
			dim -= d.y;
		}else{
			dim -= d.x;
		}
	}
	
	if(dim > 0){
		this->firstTailItemOffset = -1;
	}else{
		this->firstTailItemOffset = -dim;
	}
}
