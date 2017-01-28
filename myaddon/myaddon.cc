#include <nan.h>

using namespace v8;

void Print(const Nan::FunctionCallbackInfo<Value>& info) {
  Nan::MaybeLocal<String> str = Nan::To<String>(info[0]);
  if (str.IsEmpty()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  printf("%s\n", *String::Utf8Value(str.ToLocalChecked()));
}

void Init(Local<Object> exports) {
  exports->Set(Nan::New("print").ToLocalChecked(),
               Nan::New<FunctionTemplate>(Print)->GetFunction());
}

NODE_MODULE(myaddon, Init)
