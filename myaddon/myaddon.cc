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

void Length(const Nan::FunctionCallbackInfo<Value>& info) {
  Nan::MaybeLocal<String> str = Nan::To<String>(info[0]);
  if (str.IsEmpty()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  int length = strlen(*String::Utf8Value(str.ToLocalChecked()));

//  printf("%d\n", length);

  Local<Number> num = Nan::New(length);

  info.GetReturnValue().Set(num);
}

void Init(Local<Object> exports) {
  exports->Set(Nan::New("print").ToLocalChecked(),
               Nan::New<FunctionTemplate>(Print)->GetFunction());
  exports->Set(Nan::New("length").ToLocalChecked(),
               Nan::New<FunctionTemplate>(Length)->GetFunction());
}

NODE_MODULE(myaddon, Init)
