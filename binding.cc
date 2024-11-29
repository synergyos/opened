#include <napi.h>
#if defined(_WIN32)
#include <io.h>
#include <windows.h>
#endif

int open(const char* path) {
#if defined(_WIN32)
  int chars = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
  if (chars == 0) return GetLastError();
  WCHAR* pathw = (WCHAR*)malloc(chars * sizeof(WCHAR));
  if (pathw == NULL) return ERROR_OUTOFMEMORY;
  MultiByteToWideChar(CP_UTF8, 0, path, -1, pathw, chars);
  HANDLE handle = CreateFileW(
    pathw,
    FILE_GENERIC_READ | FILE_GENERIC_WRITE,
    0L,
    NULL,
    OPEN_EXISTING,
    NULL,
    NULL
  );
  free(pathw);
  if (handle == INVALID_HANDLE_VALUE) return GetLastError();
  CloseHandle(handle);
  return 0;
#else
  return -1;
#endif
}

class OpenWorker : public Napi::AsyncWorker {
 public:
  OpenWorker(Napi::Buffer<char>& pathHandle, Napi::Function& callback)
      : Napi::AsyncWorker(callback), path(pathHandle.Data()), pathRef(Napi::Persistent(pathHandle)) {
    // Keep a reference to the buffer to prevent it from being garbage collected
  }

  ~OpenWorker() {}

  void Execute() override {
    error = open(path);
  }

  void OnOK() override {
    Napi::HandleScope scope(Env());
    Callback().Call({Napi::Number::New(Env(), error)});
  }

 private:
  const char* path;
  int error;
  Napi::Reference<Napi::Buffer<char>> pathRef;
};

Napi::Value opened(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2 || !info[0].IsBuffer() || !info[1].IsFunction()) {
    Napi::TypeError::New(env, "Expected arguments: (Buffer path, Function callback)").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Buffer<char> pathHandle = info[0].As<Napi::Buffer<char>>();
  Napi::Function callback = info[1].As<Napi::Function>();

  OpenWorker* worker = new OpenWorker(pathHandle, callback);
  worker->Queue();

  return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("opened", Napi::Function::New(env, opened));
  return exports;
}

NODE_API_MODULE(binding, Init)
