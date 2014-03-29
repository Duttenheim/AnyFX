//------------------------------------------------------------------------------------
/**
	Demo project entry point
*/
#include "demoapp.h"

int
main()
{
	AnyFX::DemoApp app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}

	return 0;
}
