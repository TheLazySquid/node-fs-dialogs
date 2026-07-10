#include <nfd.h>
#include <napi.h>
#include <string>
#include <vector>
#include <memory>
#include "./pickers.cc"

void destroyFileArgs(nfdopendialogu8args_t* p) {
    // Never let me touch c++ again
    for(nfdfiltersize_t i = 0; i < p->filterCount; i++) {
        free((void *)p->filterList[i].name);
        free((void *)p->filterList[i].spec);
    }
    delete p->filterList;

    if(p->defaultPath) free((void *)p->defaultPath);
    if(p->title) free((void *)p->title);
    if(p->acceptLabel) free((void *)p->acceptLabel);
    if(p->cancelLabel) free((void *)p->cancelLabel);
}

void destroyDirArgs(nfdpickfolderu8args_t* p) {
    if(p->defaultPath) free((void *)p->defaultPath);
    if(p->title) free((void *)p->title);
    if(p->acceptLabel) free((void *)p->acceptLabel);
    if(p->cancelLabel) free((void *)p->cancelLabel);
}

void destroySaveArgs(nfdsavedialogu8args_t* p) {
    for(nfdfiltersize_t i = 0; i < p->filterCount; i++) {
        free((void *)p->filterList[i].name);
        free((void *)p->filterList[i].spec);
    }
    delete p->filterList;

    if(p->defaultPath) free((void *)p->defaultPath);
    if(p->defaultName) free((void *)p->defaultName);
    if(p->title) free((void *)p->title);
    if(p->acceptLabel) free((void *)p->acceptLabel);
    if(p->cancelLabel) free((void *)p->cancelLabel);
}

typedef std::unique_ptr<nfdopendialogu8args_t, decltype(&destroyFileArgs)> fileargs;
typedef std::unique_ptr<nfdpickfolderu8args_t, decltype(&destroyDirArgs)> dirargs;
typedef std::unique_ptr<nfdsavedialogu8args_t, decltype(&destroySaveArgs)> saveargs;

class FilePicker : public SinglePicker {
    fileargs args;
    
    void Execute() override {
        NFD_Init();

        nfdu8char_t *outPath;
        auto result = NFD_OpenDialogU8_With(&outPath, args.get());

        OnResult(result, outPath);
        NFD_Quit();
    }

public:
    FilePicker(const Napi::Env& env, fileargs& args) : SinglePicker(env), args(std::move(args)) {}
};

class MultiFilePicker : public MultiPicker {
    fileargs args;
    
    void Execute() override {
        NFD_Init();

        const nfdpathset_t *outPaths;
        auto result = NFD_OpenDialogMultipleU8_With(&outPaths, args.get());

        OnResult(result, outPaths);
        NFD_Quit();
    }

public:
    MultiFilePicker(const Napi::Env& env, fileargs& args) : MultiPicker(env), args(std::move(args)) {}
};

class DirPicker : public SinglePicker {
    dirargs args;
    
    void Execute() override {
        NFD_Init();

        nfdu8char_t *outPath;
        auto result = NFD_PickFolderU8_With(&outPath, args.get());

        OnResult(result, outPath);
        NFD_Quit();
    }

public:
    DirPicker(const Napi::Env& env, dirargs& args) : SinglePicker(env), args(std::move(args)) {}
};

class MultiDirPicker : public MultiPicker {
    dirargs args;
    
    void Execute() override {
        NFD_Init();

        const nfdpathset_t *outPaths;
        auto result = NFD_PickFolderMultipleU8_With(&outPaths, args.get());

        OnResult(result, outPaths);
        NFD_Quit();
    }

public:
    MultiDirPicker(const Napi::Env& env, dirargs& args) : MultiPicker(env), args(std::move(args)) {}
};

class SaveFilePicker : public SinglePicker {
    saveargs args;
    
    void Execute() override {
        NFD_Init();

        nfdu8char_t *outPath;
        auto result = NFD_SaveDialogU8_With(&outPath, args.get());

        OnResult(result, outPath);
        NFD_Quit();
    }

public:
    SaveFilePicker(const Napi::Env& env, saveargs& args) : SinglePicker(env), args(std::move(args)) {}
};

std::vector<nfdu8filteritem_t>* createFilterItems(const Napi::Array& filters) {
    auto filterCount = filters.Length();
    auto filterItems = new std::vector<nfdu8filteritem_t>(filterCount);

    for(uint32_t i = 0; i < filterCount; i++) {
        Napi::Object filter = filters[i].As<Napi::Object>();
        Napi::String name = filter.Get("name").As<Napi::String>();
        Napi::String extensions = filter.Get("extensions").As<Napi::String>();

        (*filterItems)[i] = {
            strdup(name.Utf8Value().c_str()),
            strdup(extensions.Utf8Value().c_str())
        };
    }

    return filterItems;
}

fileargs createPickFileArgs(const Napi::CallbackInfo& info) {
    Napi::Array filters = info[0].As<Napi::Array>();

    auto filterItems = createFilterItems(filters);
    auto args = std::unique_ptr<nfdopendialogu8args_t, decltype(&destroyFileArgs)>(new nfdopendialogu8args_t {
        filterItems->data(),
        (nfdfiltersize_t) filterItems->size(),
        0, { 0, 0 }, 0, 0, 0
    }, destroyFileArgs);

    if(!info[1].IsUndefined()) args.get()->defaultPath = strdup(info[1].As<Napi::String>().Utf8Value().c_str());
    if(!info[2].IsUndefined()) args.get()->title = strdup(info[2].As<Napi::String>().Utf8Value().c_str());
    if(!info[3].IsUndefined()) args.get()->acceptLabel = strdup(info[3].As<Napi::String>().Utf8Value().c_str());
    if(!info[4].IsUndefined()) args.get()->cancelLabel = strdup(info[4].As<Napi::String>().Utf8Value().c_str());

    return args;
}

dirargs createPickDirArgs(const Napi::CallbackInfo& info) {
    auto args = std::unique_ptr<nfdpickfolderu8args_t, decltype(&destroyDirArgs)>(new nfdpickfolderu8args_t {
        0, { 0, 0 }, 0, 0, 0
    }, destroyDirArgs);

    if(!info[0].IsUndefined()) args.get()->defaultPath = strdup(info[0].As<Napi::String>().Utf8Value().c_str());
    if(!info[1].IsUndefined()) args.get()->title = strdup(info[1].As<Napi::String>().Utf8Value().c_str());
    if(!info[2].IsUndefined()) args.get()->acceptLabel = strdup(info[2].As<Napi::String>().Utf8Value().c_str());
    if(!info[3].IsUndefined()) args.get()->cancelLabel = strdup(info[3].As<Napi::String>().Utf8Value().c_str());

    return args;
}

saveargs createPickSaveFileArgs(const Napi::CallbackInfo& info) {
    Napi::Array filters = info[0].As<Napi::Array>();

    auto filterItems = createFilterItems(filters);
    auto args = std::unique_ptr<nfdsavedialogu8args_t, decltype(&destroySaveArgs)>(new nfdsavedialogu8args_t {
        filterItems->data(),
        (nfdfiltersize_t) filterItems->size(),
        0, 0, { 0, 0 }, 0, 0, 0
    }, destroySaveArgs);

    if(!info[1].IsUndefined()) args.get()->defaultPath = strdup(info[1].As<Napi::String>().Utf8Value().c_str());
    if(!info[2].IsUndefined()) args.get()->defaultName = strdup(info[2].As<Napi::String>().Utf8Value().c_str());
    if(!info[3].IsUndefined()) args.get()->title = strdup(info[3].As<Napi::String>().Utf8Value().c_str());
    if(!info[4].IsUndefined()) args.get()->acceptLabel = strdup(info[4].As<Napi::String>().Utf8Value().c_str());
    if(!info[5].IsUndefined()) args.get()->cancelLabel = strdup(info[5].As<Napi::String>().Utf8Value().c_str());

    return args;
}

Napi::Promise PickFile(const Napi::CallbackInfo& info) {
    auto args = createPickFileArgs(info);
    auto picker = new FilePicker(info.Env(), args);
    return picker->getPromise();
}

Napi::Promise PickFiles(const Napi::CallbackInfo& info) {
    auto args = createPickFileArgs(info);
    auto picker = new MultiFilePicker(info.Env(), args);
    return picker->getPromise();
}

Napi::Promise PickDirectory(const Napi::CallbackInfo& info) {
    auto args = createPickDirArgs(info);
    auto picker = new DirPicker(info.Env(), args);
    return picker->getPromise();
}

Napi::Promise PickDirectories(const Napi::CallbackInfo& info) {
    auto args = createPickDirArgs(info);
    auto picker = new MultiDirPicker(info.Env(), args);
    return picker->getPromise();
}

Napi::Promise PickSaveFile(const Napi::CallbackInfo& info) {
    auto args = createPickSaveFileArgs(info);
    auto picker = new SaveFilePicker(info.Env(), args);
    return picker->getPromise();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "pickFile"), Napi::Function::New(env, PickFile));
    exports.Set(Napi::String::New(env, "pickFiles"), Napi::Function::New(env, PickFiles));
    exports.Set(Napi::String::New(env, "pickDirectory"), Napi::Function::New(env, PickDirectory));
    exports.Set(Napi::String::New(env, "pickDirectories"), Napi::Function::New(env, PickDirectories));
    exports.Set(Napi::String::New(env, "pickSaveFile"), Napi::Function::New(env, PickSaveFile));
    return exports;
}

NODE_API_MODULE(node-fs-dialogs, Init)