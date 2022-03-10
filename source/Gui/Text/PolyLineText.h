#pragma once
#include "MonoLineText.h"

 class PolyLineText
 {
	 std::vector<MonoLineText> lines;
 public:
	PolyLineText();
	~PolyLineText();

	void draw(Shader& shader,TextSettings ts);

	virtual int getHeight(const TextSettings& ts);
	virtual int getWidth(const TextSettings& ts);
 };
 
 