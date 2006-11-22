#include "YabInterface.h"

int main(int argc, char** argv)
{
	int ret;
	YabInterface *yabInterface = new YabInterface(argc, argv, "");
	yabInterface->Run();
//	ret = yabInterface->GetErrorCode();
	delete yabInterface;
	return ret;
}
