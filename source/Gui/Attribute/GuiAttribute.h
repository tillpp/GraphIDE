#pragma once
class GuiComponent;
class GuiEquation;
#include <mutex>
#include <vector>
#include <set>
#include <functional> 
class GuiAttribute
{
private:
	std::recursive_mutex& mutex;
	
	GuiEquation* equation = nullptr;
	std::set<GuiAttribute*> dependers;
	GuiComponent* component; 
	
	double cachedValue = 0;
	bool xAxis;
protected:
	void deregisterDepender(GuiAttribute*);
	void registerDepender(GuiAttribute*); //no checking for cyclic dependencies
	
	//Assuming that the other attributes without this attribute are acyclicly dependend : This function will terminate. 
	bool checkCyclicDependency(GuiAttribute* origin);
public:
	operator double();

	GuiAttribute(GuiComponent*,bool xAxis);
	GuiAttribute(GuiAttribute&)=delete;

	~GuiAttribute();

	std::vector<GuiAttribute*> getDependencies();

	void updateValue();
	//this is a [SLOW FUNCTION] 
	void setEquation(GuiEquation&& eq);
 
	void adjustEquation(std::function<void(GuiEquation*)> fun);
	

	//Use this before moving the GuiComponent
	void deconnect();
	//Use this after moving after the GuiComponent has been moved [SLOW FUNCTION]
	void connect();
	
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
	
	
};
