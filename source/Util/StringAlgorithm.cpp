#include "StringAlgorithm.h"

const sf::String StringAlgorithm::whitespace = " \t\n";

unsigned int StringAlgorithm::skip (sf::String& string, sf::String const& characterGroup,unsigned int start){
	for (size_t i = start; i < string.getSize(); i++)
	{
		if(characterGroup.find(string[i])==sf::String::InvalidPos)
			return i;
	}
	return string.getSize();
}
unsigned int StringAlgorithm::rskip(sf::String& string, sf::String const& characterGroup,unsigned int start){
	if(start==0)
		return 0;
	for (int i = start; i >= 1; i--)
	{
		if(characterGroup.find(string[i-1])==sf::String::InvalidPos)
			return i;
	}
	return 0;
}

unsigned int StringAlgorithm::find(sf::String& string, sf::String const& characterGroup,unsigned int start){
	for (size_t i = start; i < string.getSize(); i++)
	{
		if(characterGroup.find(string[i])!=sf::String::InvalidPos)
			return i;
	}
	return sf::String::InvalidPos;
}
unsigned int StringAlgorithm::rfind(sf::String& string, sf::String const& characterGroup,unsigned int start){
	if(start==0)
		return sf::String::InvalidPos;
	for (int i = start; i >= 1; i--)
	{
		if(characterGroup.find(string[i-1])!=sf::String::InvalidPos)
			return i;
	}
	return sf::String::InvalidPos;
}