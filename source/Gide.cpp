#include "Gide.h"
#include <iostream>
#include <unistd.h>

Gide::Gide(/* args */)
{
	char tmp[256];
	getcwd(tmp,256);
	std::cout << tmp << std::endl;
}

Gide::~Gide()
{
}
