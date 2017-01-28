#include <nan.h>

using namespace v8;

void Print(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  printf("I am a native addon and I AM ALIVE!\n");
}

void Init(Local<Object> exports) {
  exports->Set(Nan::New("print").ToLocalChecked(),
               Nan::New<FunctionTemplate>(Print)->GetFunction());
}

NODE_MODULE(myaddon, Init)
