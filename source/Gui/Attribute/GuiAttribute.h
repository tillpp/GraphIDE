#pragma once
class GuiComponent;
class GuiEquation;
class GuiAttributeKey;
#include <mutex>
#include <vector>
#include <set>
#include <functional> 
#include "GuiAttributeKey.h"

class GuiAttribute
{
private:
	friend GuiComponent;
	std::recursive_mutex& mutex;
	
	std::string name;

	GuiEquation* equation = nullptr;
	std::set<GuiAttribute*> dependers;
	GuiComponent* component; 
	
	double cachedValue = 0;
	bool xAxis;

	GuiAttributeKey* locked = nullptr; //If set, cachedoverride doesnt work anymore
protected:
	void deregisterDepender(GuiAttribute*);
	void registerDepender(GuiAttribute*); //no checking for cyclic dependencies
	
	//Assuming that the other attributes without this attribute are acyclicly dependend : This function will terminate. 
	bool checkCyclicDependency(GuiAttribute* origin);
public:
	operator double();

	GuiAttribute(GuiComponent*,bool xAxis,std::string name);
	GuiAttribute(GuiAttribute&)=delete;

	~GuiAttribute();

	std::vector<GuiAttribute*> getDependencies();

	//only evaluate no changes tho
	double evaluate(GuiEquation& eq);

	void updateValue();
	//this is a [SLOW FUNCTION] 
	void setEquation(const GuiEquation& eq);
	void overrideCachedValue(double,GuiAttributeKey* key = nullptr);


	//Lock functions
	bool lock(GuiAttributeKey*);
	bool unlock(GuiAttributeKey*);


	//Use this before moving the GuiComponent
	void deconnect();
	//Use this after moving after the GuiComponent has been moved [SLOW FUNCTION]
	void connect();
	

	std::string debugInformation(std::string tabs="");
protected:
	/*
		updates the depender of the dependencies.
		checking for cyclic dependencies.
	*/
	void setDependerFromDependencies();
	/*
		remove all dependers from the dependencies.
	*/
	void removeAllDependerFromDependency();
	/*
		propage change of cachedValue to all dependers 
	*/
	void propagateChange();
};
