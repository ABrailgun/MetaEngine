//
// Created by Shuji Li on 2022/8/18.
//

#include "BaseApplication.hpp"

int Meta::BaseApplication::Initialize() {
  quit_ = false;
  return InitialStatus::Success;
}

void Meta::BaseApplication::Finalize() {
}

void Meta::BaseApplication::Tick() {
}

bool Meta::BaseApplication::IsQuit() {
  return quit_;
}