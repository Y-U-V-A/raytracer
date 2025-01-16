#include "sandbox.h"

int main(const int argc, const char** argv) {

    const char* file_name = "scene";
    if (argc >= 2) {
        file_name = argv[1];
    }

    zmemory_init();
    zmemory_log();
    random_seed();

    scene_render(file_name);

    zmemory_log();
    zmemory_destroy();
}