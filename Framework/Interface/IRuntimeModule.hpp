#pragma once

namespace Meta {

enum InitialStatus {
    Success = 0,
    Failed = -1
};

class IRuntimeModule {
 public:
  virtual ~IRuntimeModule() {};

  virtual int Initialize() = 0;
  virtual void Finalize() = 0;

  virtual void Tick() = 0;
};

}