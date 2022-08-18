//
// Created by Shuji Li on 2022/8/18.
//

#include "BaseApplication.hpp"

namespace Meta {
    BaseApplication global_app_;
    IApplication* global_app_pointer_ = &global_app_;
}

