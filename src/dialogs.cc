#include <nfd.h>
#include <napi.h>
#include <string>
#include <iostream>
#include <vector>
#include "basePicker.cc"

typedef std::vector<std::string> vstring;

class FileDialogWorker : public FSPicker {
public:
    FileDialogWorker(const Napi::Env& env, vstring names, vstring exts, std::string path, bool multi)
        : FSPicker(env, path, multi) {
        filterNames = std::move(names);
        filterExtensions = std::move(exts);
    }

    ~FileDialogWorker() {}

    void Execute() override {
        NFD_Init();
        
        std::vector<nfdu8filteritem_t> filters;
        for(size_t i = 0; i < filterNames.size(); i++) {
            filters.push_back({
                filterNames[i].c_str(),
                filterExtensions[i].c_str()
            });
        }

        nfdopendialogu8args_t args = { 0 };
        args.filterList = filters.data();
        args.filterCount = filters.size();
        if(!defaultPath.empty()) args.defaultPath = defaultPath.c_str();
        nfdresult_t result;

        if(multiple) {
            const nfdpathset_t* outPaths; 
            result = NFD_OpenDialogMultipleU8_With(&outPaths, &args);

            if (result == NFD_OKAY) {
                // Get the paths from the path set
                nfdpathsetsize_t numPaths;
                NFD_PathSet_GetCount(outPaths, &numPaths);

                nfdpathsetsize_t i;
                for(i = 0; i < numPaths; ++i) {
                    nfdu8char_t *outPath;
                    NFD_PathSet_GetPath(outPaths, i, &outPath);
                    selectedPaths.push_back(std::string(outPath));
                    NFD_PathSet_FreePathU8(outPath);
                }
            }
        } else {
            nfdu8char_t *outPath = nullptr;
            result = NFD_OpenDialogU8_With(&outPath, &args);

            if (result == NFD_OKAY) {
                selectedPath = std::string(outPath);
                NFD_FreePathU8(outPath);
            }
        }

        if (result == NFD_CANCEL) {
            wasCancelled = true;
        } else if(result == NFD_ERROR) {
            error = std::string(NFD_GetError());
            SetError(error);
        }

        NFD_Quit();
    }

private:
    vstring filterNames;
    vstring filterExtensions;
};

std::pair<vstring, vstring> parseFilters(const Napi::Array& filters) {
    vstring filterNames;
    vstring filterExtensions;

    for(uint32_t i = 0; i < filters.Length(); i++) {
        Napi::Value filter = filters[i];
        Napi::Object filterObj = filter.As<Napi::Object>();
        Napi::String name = filterObj.Get("name").As<Napi::String>();
        Napi::String extensions = filterObj.Get("extensions").As<Napi::String>();

        filterNames.push_back(name.Utf8Value());
        filterExtensions.push_back(extensions.Utf8Value());
    }

    return { filterNames, filterExtensions };
}

Napi::Promise selectFiles(const Napi::CallbackInfo& info, bool multiple) {
    Napi::Array filters = info[0].As<Napi::Array>();
    Napi::String defaultPath = info[1].As<Napi::String>();
    
    std::pair<vstring, vstring> filterPair = parseFilters(filters);    
    std::string defaultPathStr = defaultPath.Utf8Value();

    Napi::Env env = info.Env();
    FileDialogWorker* worker = new FileDialogWorker(env, filterPair.first, filterPair.second, defaultPathStr, multiple);
    worker->Queue();
    return worker->GetPromise();
}

Napi::Value PickFile(const Napi::CallbackInfo& info) {
    return selectFiles(info, false);
}

Napi::Value PickFiles(const Napi::CallbackInfo& info) {
    return selectFiles(info, true);
}

class FolderDialogWorker : public FSPicker {
public:
    FolderDialogWorker(const Napi::Env& env, std::string path, bool multi) : FSPicker(env, path, multi) {}

    ~FolderDialogWorker() {}

    void Execute() override {
        NFD_Init();

        nfdpickfolderu8args_t args = { 0 };
        args.defaultPath = defaultPath.c_str();
        nfdresult_t result;

        if(multiple) {
            const nfdpathset_t* outPaths; 
            result = NFD_PickFolderMultipleU8_With(&outPaths, &args);

            if (result == NFD_OKAY) {
                // Get the paths from the path set
                nfdpathsetsize_t numPaths;
                NFD_PathSet_GetCount(outPaths, &numPaths);

                nfdpathsetsize_t i;
                for(i = 0; i < numPaths; ++i) {
                    nfdu8char_t *outPath;
                    NFD_PathSet_GetPath(outPaths, i, &outPath);
                    selectedPaths.push_back(std::string(outPath));
                    NFD_PathSet_FreePathU8(outPath);
                }
            }
        } else {
            nfdu8char_t *outPath = nullptr;
            result = NFD_PickFolderU8_With(&outPath, &args);

            if (result == NFD_OKAY) {
                selectedPath = std::string(outPath);
                NFD_FreePathU8(outPath);
            }
        }

        if (result == NFD_CANCEL) {
            wasCancelled = true;
        } else if(result == NFD_ERROR) {
            error = std::string(NFD_GetError());
            SetError(error);
        }

        NFD_Quit();
    }
};

Napi::Promise selectDirectory(const Napi::CallbackInfo& info, bool multiple) {
    Napi::String defaultPath = info[0].As<Napi::String>();
    
    Napi::Env env = info.Env();
    FolderDialogWorker* worker = new FolderDialogWorker(env, defaultPath, multiple);
    worker->Queue();
    return worker->GetPromise();
}

Napi::Value PickDirectory(const Napi::CallbackInfo& info) {
    return selectDirectory(info, false);
}

Napi::Value PickDirectories(const Napi::CallbackInfo& info) {
    return selectDirectory(info, true);
}

class SaveFileDialogWorker : public FSPickerSingle {
public:
    SaveFileDialogWorker(const Napi::Env& env, vstring names, vstring exts, std::string path, std::string name)
        : FSPickerSingle(env, path) {
        filterNames = std::move(names);
        filterExtensions = std::move(exts);
        defaultName = std::move(name);
    }

    ~SaveFileDialogWorker() {}

    void Execute() override {
        NFD_Init();

        std::vector<nfdu8filteritem_t> filters;
        for(size_t i = 0; i < filterNames.size(); i++) {
            filters.push_back({
                filterNames[i].c_str(),
                filterExtensions[i].c_str()
            });
        }

        nfdu8char_t *outPath = nullptr;
        nfdsavedialogu8args_t args = { 0 };
        args.filterList = filters.data();
        args.filterCount = filters.size();
        if(!defaultPath.empty()) args.defaultPath = defaultPath.c_str();
        if(!defaultName.empty()) args.defaultName = defaultName.c_str();

        nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);

        if (result == NFD_OKAY) {
            selectedPath = std::string(outPath);
            NFD_FreePathU8(outPath);
        } else if (result == NFD_CANCEL) {
            wasCancelled = true;
        } else {
            error = std::string(NFD_GetError());
            SetError(error);
        }

        NFD_Quit();
    }
private:
    vstring filterNames;
    vstring filterExtensions;
    std::string defaultName;
};

Napi::Value PickSaveFile(const Napi::CallbackInfo& info) {
    Napi::Array filters = info[0].As<Napi::Array>();
    Napi::String defaultPath = info[1].As<Napi::String>();
    Napi::String defaultName = info[2].As<Napi::String>();

    std::pair<vstring, vstring> filterPair = parseFilters(filters);
    
    Napi::Env env = info.Env();
    SaveFileDialogWorker* worker = new SaveFileDialogWorker(env, filterPair.first, filterPair.second,
        defaultPath.Utf8Value(), defaultName.Utf8Value());
    worker->Queue();
    return worker->GetPromise();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("pickFile", Napi::Function::New(env, PickFile));
    exports.Set("pickFiles", Napi::Function::New(env, PickFiles));
    exports.Set("pickDirectory", Napi::Function::New(env, PickDirectory));
    exports.Set("pickDirectories", Napi::Function::New(env, PickDirectories));
    exports.Set("pickSaveFile", Napi::Function::New(env, PickSaveFile));
    return exports;
}

NODE_API_MODULE(node-fs-dialogs, Init)