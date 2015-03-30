/*
 * Copyright 2015, Gerasim Troeglazov, 3dEyes@gmail.com. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include <Application.h>

#include "SVGTranslator.h"
#include "TranslatorWindow.h"

int
main(int argc, char *argv[])
{
	BApplication application("application/x-vnd.Haiku-SVGTranslator");

	status_t result;
	result = LaunchTranslatorWindow(new SVGTranslator, "SVGTranslator Settings",
		BRect(0, 0, 320, 200));
	if (result != B_OK)
		return 1;

	application.Run();

	return 0;
}

