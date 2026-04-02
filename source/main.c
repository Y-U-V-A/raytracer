#include "sandbox.h"

int main(const int argc, const char** argv) {

    const char* file_name = "scene";
    if (argc >= 2) {
        file_name = argv[1];
    }

    zmemory_init();

    random_seed();
    render_scene(file_name);

    zmemory_destroy();

    return 0;
}