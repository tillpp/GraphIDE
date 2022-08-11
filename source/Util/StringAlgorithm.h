#pragma once
#include <SFML/System.hpp>
#include <stddef.h>
#include <string>

struct StringAlgorithm
{
	// ## constants ##
	static const sf::String whitespace; 

	// ## functions ##
	static unsigned int skip (sf::String& string, sf::String const& characterGroup,unsigned int start = 0);
	static unsigned int rskip(sf::String& string, sf::String const& characterGroup,unsigned int start);

	static unsigned int find(sf::String& string, sf::String const& characterGroup,unsigned int start);
	static unsigned int rfind(sf::String& string, sf::String const& characterGroup,unsigned int start);
};
 