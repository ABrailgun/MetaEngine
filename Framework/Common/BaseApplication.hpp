#pragma once
#include "IApplication.hpp"

namespace Meta {

class BaseApplication : public IApplication {
 public:
  virtual int Initialize();
  virtual void Finalize();
  virtual void Tick();
  virtual bool IsQuit();

 protected:
   bool quit_;
};

}




