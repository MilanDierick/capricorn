// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "capricorn/base/application.hpp"

int main()
{
	auto* application = new cc::application();

	application->initialize();
	application->execute();
	application->shutdown();

	delete application;

	return 0;
}
