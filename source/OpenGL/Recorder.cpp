#include "Recorder.h"

void Recorder::create(int width,int height){
	mutex.lock();
	renderTexture.create(width,height,sf::ContextSettings(32));
	mutex.unlock();
}

void Recorder::record(){
	mutex.lock();
	renderTexture.setActive();
	renderTexture.clear();	
	mutex.unlock();
}
void Recorder::save(){
	mutex.lock();
	renderTexture.display();
	
	TextureSettings settings;
	settings.flipVertically=true;
	
	//DEBUGLINE DELETE ME!
	renderTexture.getTexture().copyToImage().saveToFile("test.png");

	texture.LoadFromTexture(renderTexture.getTexture(),settings);
	mutex.unlock();
}
void Recorder::use(short inActivate, Shader& inShaderProgram, std::string inTextureNameInShader){
	mutex.lock();
	texture.use(inActivate,inShaderProgram,inTextureNameInShader);
	mutex.unlock();
}