
#include "DiffWorkerCallback.hpp"

extern "C" {
  #include "c/bsdiff/bsdiff.h"
}

// Public
DiffWorkerCallback::DiffWorkerCallback(const Napi::Function& callback, const std::string& oldfile, const std::string& newfile, const std::string& patchfile)
    : Napi::AsyncProgressWorker<int>(callback)
{
  _oldfile = oldfile;
  _newfile = newfile;
  _patchfile = patchfile;
}

DiffWorkerCallback::~DiffWorkerCallback()
{   }

void DiffWorkerCallback::Execute(const Napi::AsyncProgressWorker<int>::ExecutionProgress & progress)
{
  char error[1024];
  memset(error, 0, sizeof error);

  DiffWorkerData data;
  data.percentage = 0;
  data.progressWorker = &progress;

  bsdiff(error, _oldfile.c_str(), _newfile.c_str(), _patchfile.c_str(), &data, &DiffWorkerCallback::CCallback);
  _error = error;
}

void DiffWorkerCallback::OnProgress(const int* data, size_t count)
{
  if (data != nullptr)
  {
    Callback().Call(Receiver().Value(), std::initializer_list<napi_value>{
      Napi::Value::From(Env(), *data),
        Napi::String::From(Env(), _error)
    });
  }
}

void DiffWorkerCallback::OnOK()
{
  Callback().Call(Receiver().Value(), std::initializer_list<napi_value>{
    Napi::Value::From(Env(), 100),
      Napi::String::From(Env(), _error)
  });
}
