#include "al2o3_platform/platform.h"
#include "al2o3_os/filesystem.hpp"

void RunTestFunc(Os_DirectoryEnumeratorHandle handle, void* userData, char const* filename) {
	LOGINFOF("Running %s test", filename);
	if (strncmp(filename, "test_", 5) == 0) {
		Os_SystemRun(filename, 0, nullptr);
	}
}
int main(int argv, char* argc[]) {
	LOGWARNING("Parsing folder for test_*");
	Os_DirectoryEnumeratorHandle handle = Os_DirectoryEnumeratorFromPath("./", &RunTestFunc, nullptr);
	Os_DirectoryEnumeratorSyncStart(handle);
	while (Os_DirectoryEnumeratorSyncNext(handle)) {
		// do nothing
	}
	Os_DirectoryEnumeratorClose(handle);
	return 0;
}
