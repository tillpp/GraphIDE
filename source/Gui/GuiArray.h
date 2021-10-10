#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "OpenGL/Texture.h"
#include "OpenGL/Recorder.h"
#include "OpenGL/Mesh.h"
#include "GuiComponent.h"


/*
	coordinates efficient drawing of multiple gui components.
*/
class GuiArray
{
	/*
		Cluster of components that dont have to be redrawn.
	*/
	struct ComponentCluster{
		bool 						needRedraw = false;
		std::vector<GuiComponent*> 	components;
		Recorder*					recorder;
		bool 						missmatch = false; // did a missmatch happen?
		
		void draw(Shader& shader);
		size_t size(){
			return components.size();	
		}

		ComponentCluster(){
			recorder = new Recorder;
		}
		~ComponentCluster(){
			delete recorder;
		}
	};
	std::recursive_mutex mutex;
	std::vector<ComponentCluster> clusters;

	void reorder();
public:

	void add(GuiComponent*,size_t position = SIZE_MAX);
	void remove(GuiComponent*);
	void remove(size_t position);
	
	void draw(Shader& shader);
};
