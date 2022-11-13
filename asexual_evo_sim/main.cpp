#include "pch.h"

#include "sim_app.h"

#include <iostream>
#include <cstdlib>
#include <stdexcept>



int main()
{
	evo::Logger::init();

	evo::Application app{ };

	try
	{
		app.run();
	} catch (const std::exception& e)
	{
		std::cerr << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
