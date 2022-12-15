#include <iostream>
#include <DatabaseController.h>

int main()
{
	DatabaseHandle Handle;
	InitDatabaseHandle(Handle);
	LoadDatabase(Handle, "Test.json");
	SaveDatabaseTo(Handle, "T.json");
	InitDatabaseHandle(Handle);
	return 0;
}