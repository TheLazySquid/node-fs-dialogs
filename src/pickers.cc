#include <napi.h>

class SinglePicker : protected Napi::AsyncWorker {
protected:
    Napi::Promise::Deferred deferred;
    std::string selectedPath;
    bool wasCancelled = false;

    void OnResult(nfdresult_t result, nfdu8char_t *outPath) {
        if(result == NFD_OKAY) {
            selectedPath = std::string(outPath);
            NFD_FreePathU8(outPath);
        } else if(result == NFD_CANCEL) {
            wasCancelled = true;
        } else {
            auto error = std::string(NFD_GetError());
            SetError(error);
        }
    }

    void OnOK() override {
        if(wasCancelled) {
            deferred.Resolve(Env().Null());
        } else {
            deferred.Resolve(Napi::String::New(Env(), selectedPath));
        }
    }

    void OnError(const Napi::Error& e) override {
        deferred.Reject(e.Value());
    }

public:
    SinglePicker(const Napi::Env& env) : Napi::AsyncWorker(env), deferred(Napi::Promise::Deferred::New(env)) {
        this->Queue();
    }

    Napi::Promise getPromise() {
        return deferred.Promise();
    }
};

class MultiPicker : protected Napi::AsyncWorker {
protected:
    Napi::Promise::Deferred deferred;
    std::vector<std::string> selectedPaths;
    bool wasCancelled = false;

    void OnResult(nfdresult_t result, const nfdpathset_t *outPaths) {
        if(result == NFD_OKAY) {
            nfdpathsetsize_t numPaths;
            NFD_PathSet_GetCount(outPaths, &numPaths);

            for(nfdpathsetsize_t i = 0; i < numPaths; i++) {
                nfdu8char_t *path;
                NFD_PathSet_GetPathU8(outPaths, i, &path);
                selectedPaths.push_back(std::string(path));
                NFD_FreePathU8(path);
            }
        } else if(result == NFD_CANCEL) {
            wasCancelled = true;
        } else {
            auto error = std::string(NFD_GetError());
            SetError(error);
        }
    }

    void OnOK() override {
        if(wasCancelled) {
            deferred.Resolve(Env().Null());
        } else {
            Napi::Array paths = Napi::Array::New(Env(), selectedPaths.size());
            for(size_t i = 0; i < selectedPaths.size(); i++) {
                paths.Set(i, Napi::String::New(Env(), selectedPaths[i]));
            }
            deferred.Resolve(paths);
        }
    }

    void OnError(const Napi::Error& e) override {
        deferred.Reject(e.Value());
    }

public:
    MultiPicker(const Napi::Env& env) : Napi::AsyncWorker(env), deferred(Napi::Promise::Deferred::New(env)) {
        this->Queue();
    }

    Napi::Promise getPromise() {
        return deferred.Promise();
    }
};