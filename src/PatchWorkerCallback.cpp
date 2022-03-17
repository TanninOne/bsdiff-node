
#include "PatchWorkerCallback.hpp"

extern "C" {
  #include "c/bspatch/bspatch.h"
}

// Public
PatchWorkerCallback::PatchWorkerCallback(const Napi::Function& callback, const std::string& oldfile, const std::string& newfile, const std::string& patchfile)
    : AsyncProgressWorker(callback)
{
  _oldfile = oldfile;
  _newfile = newfile;
  _patchfile = patchfile;
}

PatchWorkerCallback::~PatchWorkerCallback()
{   }

void PatchWorkerCallback::Execute(const ExecutionProgress& progress)
{
    char error[1024];
    memset(error, 0, sizeof error);

    PatchWorkerData data;
    data.percentage = 0;
    data.progressWorker = &progress;

    bspatch(error, _oldfile.c_str(), _newfile.c_str(), _patchfile.c_str(), &data, &PatchWorkerCallback::CCallback);
    _error = error;
}

void PatchWorkerCallback::OnProgress(const int* data, size_t count)
{
    if(data != nullptr)
    {
      Callback().Call(Receiver().Value(), std::initializer_list<napi_value>{
        Napi::Value::From(Env(), *data),
          Napi::String::From(Env(), _error)
      });
    }
}

void PatchWorkerCallback::OnOK()
{
  Callback().Call(Receiver().Value(), std::initializer_list<napi_value>{
    Napi::Value::From(Env(), 100),
      Napi::String::From(Env(), _error)
  });
}
