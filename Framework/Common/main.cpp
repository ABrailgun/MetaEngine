#include <iostream>
#include "IApplication.hpp"

using namespace Meta;

namespace Meta {

extern IApplication* global_app_pointer_;

}

int main() {
    int ret = 0;
    if ((ret = global_app_pointer_->Initialize()) != InitialStatus::Success) {
        printf("App Initialize failed, will exit now.");
        return ret;
    }

    while (!global_app_pointer_->IsQuit()) {
        global_app_pointer_->Tick();
    }

    global_app_pointer_->Finalize();

    return 0;
}
