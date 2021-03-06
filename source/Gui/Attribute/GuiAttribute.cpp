#include "GuiAttribute.h"
#include "GuiEquation.h"
#include "../General/GuiComponent.h"
#include "../../Util/Log.h"

GuiAttribute::GuiAttribute(GuiComponent *compo, bool xAxis,std::string name)
	: mutex(compo->mutex)
{
	this->component = compo;
	this->xAxis = xAxis;
	this->name = name;
}

GuiAttribute::~GuiAttribute()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(locked)
		unlock(locked);
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
double GuiAttribute::evaluate(GuiEquation& eq){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return eq.evaluate(*component,cachedValue,xAxis);
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
void GuiAttribute::overrideCachedValue(double d,GuiAttributeKey* key)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(locked==key){
		if(cachedValue!=d){
			cachedValue = d;
			propagateChange();
		}
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
		if (dependency == origin)
			return true;
		if (dependency->checkCyclicDependency(origin))
			return true;
	}
	return false;
}

void GuiAttribute::propagateChange()
{
	component->triggerEvent(GuiEventAttributeChange(*this));
	for (auto &depender : dependers)
	{
		depender->updateValue();
	}
}

bool GuiAttribute::lock(GuiAttributeKey* key){
	mutex.lock();
	if(!locked){
		locked = key;	
		key->registerGuiAttribute(this);
		mutex.unlock();
		return true;
	}else {
		mutex.unlock();
		return false;
	}
}
bool GuiAttribute::unlock(GuiAttributeKey* key){
	mutex.lock();
	if(locked==key){
		locked = nullptr;
		key->unregisterGuiAttribute(this);

		mutex.unlock();
		return true;
	}else {
		mutex.unlock();
		return false;
	}
}
std::string GuiAttribute::debugInformation(std::string tabs){
	std::string rv;
	rv+=tabs+"# Dependencies of "+std::to_string((int)this)+" "+name+" of "+std::to_string((int)component)+"\n";
	if(equation)
		rv+=equation->debugInformation(tabs+"\t");
	auto dpcs = this->getDependencies();
	for(auto& dpc:dpcs){
		rv+=dpc->debugInformation(tabs+"\t");
	}
	return rv;
}