#define NOBDEF static inline
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#define NOB_WARN_DEPRECATED
#include "nob.h"


#define cc nob_cc
#define cc_flags nob_cc_flags
#define cc_output nob_cc_output
#define cc_inputs nob_cc_inputs



#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"
#define TMP_FOLDER   "tmp/"

#define GAME_TARGET_LINUX 'l' 
#define GAME_TARGET_WEB 'w'
bool build_linux();
bool build_web();
Cmd cmd = {0};  

int main (int argc, char **argv)
{
  NOB_GO_REBUILD_URSELF(argc, argv);
 if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
	int target = GAME_TARGET_LINUX;
	if (argc > 1) { 
		target = argv[1][0]; ///LMAO
		}
	switch (target) {
		case GAME_TARGET_LINUX: build_linux();
														break;
		case GAME_TARGET_WEB: build_web();
													break;
		default: build_linux();
	}

	return 0;

}

bool build_linux() {
	cc(&cmd);
  cc_flags(&cmd);
  cc_output(&cmd, BUILD_FOLDER "down_to_a_coin");
  cc_inputs(&cmd, SRC_FOLDER "main.c");
	cmd_append(&cmd, "-lraylib");
  if (!cmd_run(&cmd)) return 1;
	printf("good!");	
	cmd_append(&cmd, "./build/down_to_a_coin");
  if (!cmd_run(&cmd)) return 1;
  return 0;	
}
bool build_web() { //idk what im doing btw
  if (!nob_mkdir_if_not_exists(BUILD_FOLDER "web")) return 1;
cmd_append(&cmd, "emcc");
cc_output(&cmd, BUILD_FOLDER "web/index.html");
cmd_append(&cmd, "src/main.c");
cmd_append(&cmd, "-Wall");
cmd_append(&cmd, "-std=c23");
cmd_append(&cmd, "-D_DEFAULT_SOURCE");
cmd_append(&cmd, "-Os");
cmd_append(&cmd, "-I.");
cmd_append(&cmd, "-I", "../raylib/src");
cmd_append(&cmd, "-I", "../raylib/src/external");
cmd_append(&cmd, "-L.");
cmd_append(&cmd, "-L", "../raylib/src");
cmd_append(&cmd, "-s", "USE_GLFW=3");
cmd_append(&cmd, "-s", "ASYNCIFY");
cmd_append(&cmd, "-s", "TOTAL_MEMORY=67108864");
cmd_append(&cmd, "-s", "FORCE_FILESYSTEM=1");
cmd_append(&cmd, "../raylib/src/libraylib.web.a");
cmd_append(&cmd, "-DPLATFORM_WEB",  "-s", "EXPORTED_RUNTIME_METHODS=ccall");
cmd_append(&cmd, "--preload-file", "./assets");
cmd_append(&cmd, "--shell-file", "src/shell.html");
//cmd_append(&cmd, "-s", "'EXPORTED_FUNCTIONS=[\"_free\",\"_malloc\",\"_main\"]'");

if (!cmd_run(&cmd)) return 1;

cmd_append(&cmd, "rm", BUILD_FOLDER "web/web.zip");
cmd_run(&cmd);
cmd_append(&cmd, "zip", "-j", BUILD_FOLDER "web/web.zip", BUILD_FOLDER "web/index.html",BUILD_FOLDER "web/index.data",BUILD_FOLDER "web/index.js",BUILD_FOLDER "web/index.wasm", );
if (!cmd_run(&cmd)) return 1;
	printf("web!");	
  return 0;	
}


/*

cmd_append(&cmd, "emcc \")
cmd_append(&cmd, "-o build/web/index.html \")
cmd_append(&cmd, "src/main.c \")
cmd_append(&cmd, "-Wall \")
cmd_append(&cmd, "-std=c23 \")
cmd_append(&cmd, "-D_DEFAULT_SOURCE \")
cmd_append(&cmd, "-Os \")
cmd_append(&cmd, "-I. \")
cmd_append(&cmd, "-I ../raylib/src \")
cmd_append(&cmd, "-I ../raylib/src/external \")
cmd_append(&cmd, "-L. \")
cmd_append(&cmd, "-L ../raylib/src \")
cmd_append(&cmd, "-s USE_GLFW=3 \")
cmd_append(&cmd, "-s ASYNCIFY \")
cmd_append(&cmd, "-s TOTAL_MEMORY=67108864 \")
cmd_append(&cmd, "-s FORCE_FILESYSTEM=1 \")
cmd_append(&cmd, "../raylib/src/libraylib.web.a \")
cmd_append(&cmd, "-DPLATFORM_WEB -s 'EXPORTED_FUNCTIONS=["_free","_malloc","_main"]' -s EXPORTED_RUNTIME_METHODS=ccall \")
cmd_append(&cmd, "--preload-file ./resources \")
cmd_append(&cmd, "--shell-file src/shell.html \")
cmd_append(&cmd, "#--preload-file Sounds \")



*/

