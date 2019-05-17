#include "al2o3_platform/platform.h"
#include "al2o3_os/filesystem.hpp"
#include "utils_simple_logmanager/logmanager.h"

void RunTestFunc(Os_DirectoryEnumeratorHandle handle, void* userData, char const* filename) {
	if (strncmp(filename, "test_", 5) == 0) {
#if AL2O3_PLATFORM == AL2O3_PLATFORM_WINDOWS
		size_t namePos;
		size_t extPos;
		Os_SplitPath(filename, &namePos, &extPos);
		if (extPos == FS_npos) return;
		if (strncmp(filename + extPos, "exe", 4) != 0) return;
#endif
		if (strncmp(filename, "test_data", 9) == 0) return;

		LOGINFOF("Running %s test", filename);
		Os_SystemRun(filename, 0, nullptr);
	}
}
int main(int argv, char* argc[]) {
	// yes loggin and some filsystem (directory and system run) need to be
	// tested andf working before this works. Classic bootstrap issue :)

	// in case logger is broken at least show something
	printf("Starting run_all_al2o3_tests_app\n");

	auto logger = SimpleLogManager_Alloc();

	int count = 0;
	LOGINFO("Parsing folder for test_*");
	Os_DirectoryEnumeratorHandle handle = Os_DirectoryEnumeratorAlloc("./", &RunTestFunc, nullptr);
	Os_DirectoryEnumeratorSyncStart(handle);
	while (Os_DirectoryEnumeratorSyncNext(handle)) {
		count++;
	}
	Os_DirectoryEnumeratorFree(handle);

	LOGINFOF("%i tests ran", count);

	SimpleLogManager_Free(logger);

	return 0;
}
