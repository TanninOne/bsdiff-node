#include <napi.h>
#include "DiffWorkerCallback.hpp"
#include "PatchWorkerCallback.hpp"

extern "C" {
  #include "c/bsdiff/bsdiff.h"
  #include "c/bspatch/bspatch.h"
}

namespace bsdpNode {
  Napi::Value diff(const Napi::CallbackInfo& info)
  {
    if(info.Length() < 4 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString())
    {
      throw Napi::Error::New(info.Env(), "Expected parameters: srcFile, dstFile, patchFile, callback");
    }

    std::string oldfile(info[0].ToString());
    std::string newfile(info[1].ToString());
    std::string patchfile(info[2].ToString());
    Napi::Function callback = info[3].As<Napi::Function>();

    DiffWorkerCallback* wc = new DiffWorkerCallback(callback, oldfile, newfile, patchfile);
    wc->Queue();
    return info.Env().Undefined();
  }

  Napi::Value diffSync(const Napi::CallbackInfo& info)
  {
    if(info.Length() < 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString())
    {
      throw Napi::Error::New(info.Env(), "Expected parameters: srcFile, dstFile, patchFile");
    }

    std::string oldfile(info[0].ToString());
    std::string newfile(info[1].ToString());
    std::string patchfile(info[2].ToString());

    char error[1024];
    memset(error, 0, sizeof error);

    int ret = bsdiff(error, oldfile.c_str(), newfile.c_str(), patchfile.c_str(), nullptr, nullptr);

    if (ret != 0) {
      throw Napi::Error::New(info.Env(), error);
    }
    return info.Env().Undefined();
  }

  Napi::Value patch(const Napi::CallbackInfo& info)
  {
    if(info.Length() < 4 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString())
    {
      throw Napi::Error::New(info.Env(), "Expected parameters: srcFile, dstFile, patchFile, callback");
    }

    std::string oldfile(info[0].ToString());
    std::string newfile(info[1].ToString());
    std::string patchfile(info[2].ToString());
    Napi::Function callback = info[3].As<Napi::Function>();

    PatchWorkerCallback* wc = new PatchWorkerCallback(callback, oldfile, newfile, patchfile);
    wc->Queue();
    return info.Env().Undefined();
  }

  Napi::Value patchSync(const Napi::CallbackInfo& info)
  {
    if(info.Length() < 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString())
    {
      throw Napi::Error::New(info.Env(), "Expected parameters: srcFile, dstFile, patchFile");
    }

    std::string oldfile(info[0].ToString());
    std::string newfile(info[1].ToString());
    std::string patchfile(info[2].ToString());

    char error[1024];
    memset(error, 0, sizeof error);

    int ret = bspatch(error, oldfile.c_str(), newfile.c_str(), patchfile.c_str(), nullptr, nullptr);

    if (ret != 0) {
      throw Napi::Error::New(info.Env(), error);
    }
    return info.Env().Undefined();
  }

  Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("diff", Napi::Function::New(env, diff));
    exports.Set("diffSync", Napi::Function::New(env, diffSync));

    exports.Set("patch", Napi::Function::New(env, patch));
    exports.Set("patchSync", Napi::Function::New(env, patchSync));

    return exports;
  }

  NODE_API_MODULE(bsdiff, Init)
}
