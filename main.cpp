#include "al2o3_platform/platform.h"
#include "al2o3_os/filesystem.hpp"
#include "utils_simple_logmanager/logmanager.h"



void RunTestFunc(Os_DirectoryEnumeratorHandle handle, void* userData, char const* filename) {
}
int main(int argv, char* argc[]) {
	// yes loggin and some filsystem (directory and system run) need to be
	// tested andf working before this works. Classic bootstrap issue :)

	// in case logger is broken at least show something
	printf("Starting run_all_al2o3_tests_app\n");

	auto logger = SimpleLogManager_Alloc();

	int testsRan = 0;

	LOGINFO("Searching folder for test_*");

	Os_DirectoryEnumeratorHandle handle = Os_DirectoryEnumeratorCreate("./");
	while (Os_DirectoryEnumeratorItem const* entry = Os_DirectoryEnumeratorSyncNext(handle)) {
		if (strncmp(entry->filename, "test_", 5) == 0) {
#if AL2O3_PLATFORM == AL2O3_PLATFORM_WINDOWS
			size_t namePos;
			size_t extPos;
			Os_SplitPath(entry->filename, &namePos, &extPos);
			if (extPos == FS_npos)
				continue;
			if (strncmp(entry->filename + extPos, "exe", 4) != 0)
				continue;
#endif
			if(entry->directory)
				continue;

			LOGINFOF("Running %s test", entry->filename);
			Os_SystemRun(entry->filename, 0, nullptr);

			testsRan++;
		}
	}
	Os_DirectoryEnumeratorDestroy(handle);

	LOGINFOF("%i test programs ran", testsRan);

	SimpleLogManager_Free(logger);

	return 0;
}
