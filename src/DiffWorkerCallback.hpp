#ifndef DiffWorkerCallback_H
#define DiffWorkerCallback_H

#include <napi.h>
#include <sys/types.h>

struct DiffWorkerData {
    int percentage;
    const Napi::AsyncProgressWorker<int>::ExecutionProgress* progressWorker;
};

class DiffWorkerCallback : public Napi::AsyncProgressWorker<int> 
{
public:
    DiffWorkerCallback(const Napi::Function& callback, const std::string& oldfile, const std::string& newfile, const std::string& patchfile);
    ~DiffWorkerCallback();

    virtual void Execute(const ExecutionProgress& progress) override;
    virtual void OnProgress(const int* data, size_t count) override;
    virtual void OnOK() override;

private:
    static void CCallback(off_t current, off_t total, void* data)
    {
        ((DiffWorkerData*)data)->percentage = static_cast<int>(current * 100 / total);
        ((DiffWorkerData*)data)->progressWorker->Send(&((DiffWorkerData*)data)->percentage, 2);
    }

private:
    std::string _oldfile, _newfile, _patchfile;
    std::string _error;
};

#endif  // DiffWorkerCallback_H