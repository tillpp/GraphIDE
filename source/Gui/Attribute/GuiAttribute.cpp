#include "GuiAttribute.h"
#include "GuiEquation.h"
#include "../GuiComponent.h"
#include "../../Util/Log.h"

GuiAttribute::GuiAttribute(GuiComponent *compo, bool xAxis)
	: mutex(compo->mutex)
{
	this->component = compo;
	this->xAxis = xAxis;
}

GuiAttribute::~GuiAttribute()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	deconnect();
	if (equation)
		delete equation;
	equation = nullptr;
}
GuiAttribute::operator double()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return cachedValue;
}
void GuiAttribute::updateValue()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	double oldCachedValue = cachedValue;
	if (equation)
		cachedValue = equation->evaluate(*component,cachedValue,xAxis);
	if (cachedValue != oldCachedValue)
		propagateChange();
}
std::vector<GuiAttribute *> GuiAttribute::getDependencies()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if (equation)
	{
		auto v = equation->getDependencies(*component, xAxis);
		return v;
	}
	return {};
}
void GuiAttribute::setEquation(const GuiEquation &eq)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);

	//remove all dependencies
	removeAllDependerFromDependency();

	if (equation)
		delete equation;
	equation = eq.clone();

	setDependerFromDependencies();

	updateValue();
}
void GuiAttribute::overrideCachedValue(double d)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(cachedValue!=d){
		cachedValue = d;
		propagateChange();
	}
}

void GuiAttribute::removeAllDependerFromDependency()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	auto dependencies = getDependencies();
	for (auto &&dependency : dependencies)
		dependency->deregisterDepender(this);
}
void GuiAttribute::setDependerFromDependencies()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	auto dependencies = getDependencies();
	for (auto &&dependency : dependencies)
		dependency->registerDepender(this);
	if (checkCyclicDependency(this))
	{
		deconnect();
		Log::error("GUI ERROR: Cyclic dependencies in Scene found");
		while (1)
			;
	}
}
void GuiAttribute::deconnect()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	removeAllDependerFromDependency();
	dependers.clear();
}
void GuiAttribute::connect()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	setDependerFromDependencies();

	//update for parent
	auto parent = component->getParent();
	std::vector<GuiAttribute *> attributes;
	if (parent)
	{
		attributes = parent->getGuiAttributes();
		for (auto x : attributes)
			x->setDependerFromDependencies();

		//update for siblings
		for (auto &&sibling : parent->children)
		{
			attributes = sibling->getGuiAttributes();
			for (auto x : attributes)
				x->setDependerFromDependencies();
		}
	}
	//update for children
	for (auto &&child : component->children)
	{
		attributes = child->getGuiAttributes();
		for (auto x : attributes)
			x->setDependerFromDependencies();
	}
	updateValue();
}
void GuiAttribute::deregisterDepender(GuiAttribute *x)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	dependers.erase(x);
}
void GuiAttribute::registerDepender(GuiAttribute *x)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	dependers.insert(x);
}
bool GuiAttribute::checkCyclicDependency(GuiAttribute *origin)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	auto dependencies = getDependencies();
	for (auto &&dependency : dependencies)
	{
		if (dependency == this)
			return true;
		if (dependency->checkCyclicDependency(origin))
			return true;
	}
	return false;
}

void GuiAttribute::propagateChange()
{
	for (auto &depender : dependers)
	{
		depender->updateValue();
	}
}