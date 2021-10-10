#include "GuiArray.h"

GuiArray::ComponentCluster::draw(Shader &shader)
{
	if (needRedraw)
		for (auto &&component : components)
			component->draw();
	else
	{
		recorder.use(0, shader, "texture1");
		Mesh::rectangle().draw(shader);
	}
}
void GuiArray::draw(Shader &shader)
{
	mutex.lock();
	reorder();
	for (auto &&cluster : clusters)
	{
		cluster.draw(shader);
	}
	mutex.unlock();
}

void GuiArray::add(GuiComponent *component, size_t position)
{
	mutex.lock();
	component->setNeedRedraw(true);
	//insert it inbetween some other components
	size_t currentPosition = 0;
	for (size_t i = 0; i < clusters.size(); i++)
	{

		auto &cluster = clusters[i];
		if (currentPosition <= position || position < currentPosition + cluster.size())
		{
			//insert into the cluster and finish
			size_t offset = position - currentPosition;
			cluster.components.insert(component, position);
			mutex.unlock();
			return;
		}
		currentPosition + cluster.size();
	}

	// insert it at the end
	if (!clusters.size())
	{
		ComponentCluster componentCluster;
		clusters.push_back(componentCluster);
	}
	ComponentCluster &componentCluster = clusters.at(clusters.size());
	componentCluster.components.push_back(component);
	mutex.unlock();
	return;
}
void GuiArray::remove(GuiComponent *guiComponent)
{
	mutex.lock();
	for (auto &&cluster : clusters)
	{
		for (size_t i = 0; i < cluster.size(); i++)
		{
			if (cluster.components[i] == guiComponent)
			{
				cluster.components.erase(cluster.components.begin() + i);
				i--;
			}
		}
	}
	mutex.unlock();
}
void remove(size_t position)
{
	mutex.lock();
	size_t currentPosition = 0;
	for (size_t i = 0; i < clusters.size(); i++)
	{

		auto &cluster = clusters[i];
		if (currentPosition <= position || position < currentPosition + cluster.size())
		{
			//insert into the cluster and finish
			size_t offset = position - currentPosition;
			cluster.components.erase(cluster.components.begin() + position);
			mutex.unlock();
			return;
		}
		currentPosition + cluster.size();
	}
	mutex.unlock();
}
void GuiArray::reorder(){
	mutex.lock();
	//breaking clusters up
	for (size_t i = 0; i < clusters.size(); i++){
		 auto& cluster = clusters[i];
		 bool needRedraw = cluster.needRedraw;

		for (size_t e = 0; e < cluster.components.size(); e++)
		{
			auto& component = cluster.components[e];
		
			//missmatch
			if(needRedraw!=component->getNeedRedraw()){
				//just change cluster need if its the first
				if(e==0){
					cluster.needRedraw = component->getNeedRedraw();
					continue;
				}

				//create cluster
				ComponentCluster componentCluster;
				componentCluster.needRedraw = component->getNeedRedraw();
				componentCluster.missmatch = true;
				clusters.insert(componentCluster,clusters.begin()+i);
				//move to new cluster
				while(cluster.components.size()>e){
					clusters.at(i+1).components.push_back(cluster.components[e]);
					cluster.components.pop_back();
				}
				cluster.missmatch = true;
			}
		}
		
	}
	//merging clusters together?
	for (size_t i = 0; i < clusters.size(); i++){
		 auto& cluster = clusters[i];
		 bool needRedraw = cluster.needRedraw;

		 if(cluster.size()==0){
		 	clusters.erase(clusters.begin()+i);
		 }else if(clusters.size()>i+1){
			auto& nextCluster = clusters[i+1];
			if(needDraw == nextCluster.needRedraw)
			{
				while(nextCluster.components.size())
					cluster.components.push_back(nextCluster.components[0]);
				clusters.erase(clusters.begin()+i+1);
			}
			cluster.missmatch = true;
		 }
	}
	mutex.unlock();
}