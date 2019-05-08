#include "al2o3_platform/platform.h"
#include "al2o3_os/filesystem.hpp"

void RunTestFunc(Os_DirectoryEnumeratorHandle handle, void* userData, char const* filename) {
	if (strncmp(filename, "test_", 5) == 0) {
#if AL2O3_PLATFORM == AL2O3_PLATFORM_WINDOWS
		size_t namePos;
		size_t extPos;
		Os_SplitPath(filename, &namePos, &extPos);
		if (extPos == FS_npos) return;
		if (strncmp(filename + extPos, "exe", 4) != 0) return;
#endif

		LOGINFOF("Running %s test", filename);
		Os_SystemRun(filename, 0, nullptr);
	}
}
int main(int argv, char* argc[]) {
	// yes loggin and some filsystem (directory and system run) need to be
	// tested andf working before this works. Classic bootstrap issue :)

	LOGWARNING("Parsing folder for test_*");
	Os_DirectoryEnumeratorHandle handle = Os_DirectoryEnumeratorFromPath("./", &RunTestFunc, nullptr);
	Os_DirectoryEnumeratorSyncStart(handle);
	while (Os_DirectoryEnumeratorSyncNext(handle)) {
		// do nothing
	}
	Os_DirectoryEnumeratorClose(handle);
	return 0;
}
