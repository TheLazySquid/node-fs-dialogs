#include <string>
#include <napi.h>

class FSPicker : public Napi::AsyncWorker {
public:
    FSPicker(const Napi::Env& env, std::string path, bool multi)
        : Napi::AsyncWorker(env), deferred(Napi::Promise::Deferred::New(env)) {
        defaultPath = std::move(path);
        multiple = multi;
    }

    ~FSPicker() {}

    void OnOK() override {
        Napi::HandleScope scope(Env());

        if (wasCancelled) {
            deferred.Resolve(Env().Null());
        } else if(multiple) {
            // Create a JS array to hold the selected paths
            Napi::Array pathsArray = Napi::Array::New(Env(), selectedPaths.size());
            for (size_t i = 0; i < selectedPaths.size(); ++i) {
                pathsArray.Set(i, Napi::String::New(Env(), selectedPaths[i]));
            }
            deferred.Resolve(pathsArray);
        } else {
            deferred.Resolve(Napi::String::New(Env(), selectedPath));
        }
    }

    void OnError(const Napi::Error& e) override {
        Napi::HandleScope scope(Env());
        deferred.Reject(e.Value());
    }

    Napi::Promise GetPromise() const {
        return deferred.Promise();
    }

    std::string defaultPath;
    bool multiple;
    Napi::Promise::Deferred deferred;
    std::string selectedPath;
    std::vector<std::string> selectedPaths;
    std::string error;
    bool wasCancelled = false;
};

class FSPickerSingle : public Napi::AsyncWorker {
public:
    FSPickerSingle(const Napi::Env& env, std::string path)
        : Napi::AsyncWorker(env), deferred(Napi::Promise::Deferred::New(env)) {
        defaultPath = std::move(path);
    }

    ~FSPickerSingle() {}

    void OnOK() override {
        Napi::HandleScope scope(Env());

        if (wasCancelled) {
            deferred.Resolve(Env().Null());
        } else {
            deferred.Resolve(Napi::String::New(Env(), selectedPath));
        }
    }

    void OnError(const Napi::Error& e) override {
        Napi::HandleScope scope(Env());
        deferred.Reject(e.Value());
    }

    Napi::Promise GetPromise() const {
        return deferred.Promise();
    }

    std::string defaultPath;
    Napi::Promise::Deferred deferred;
    std::string selectedPath;
    std::vector<std::string> selectedPaths;
    std::string error;
    bool wasCancelled = false;
};