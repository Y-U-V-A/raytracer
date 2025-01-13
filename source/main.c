#include "sandbox.h"

int main(const int argc, const char** argv) {

    if (argc < 2 || argc > 2) {
        LOGE("format -> .exe image_name");
        return 0;
    }

    zmemory_init();
    zmemory_log();
    random_seed();

    scene_render(argv[1]);

    zmemory_log();
    zmemory_destroy();
}