#include <nan.h>
#ifndef _WIN32
#include <unistd.h>
#endif

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

  Local<Number> num = Nan::New(length);

  info.GetReturnValue().Set(num);
}

void Delay(const Nan::FunctionCallbackInfo<Value>& info) {
  Nan::MaybeLocal<Number> delay = Nan::To<Number>(info[0]);
  Local<Function> cb = info[1].As<Function>();
  if (delay.IsEmpty()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  int value = delay.ToLocalChecked()->IntegerValue();
  #ifdef _WIN32
  Sleep(value);
  #else
  usleep(value * 1000);
  #endif

  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, 0, NULL);
}

void Init(Local<Object> exports) {
  exports->Set(Nan::New("print").ToLocalChecked(),
               Nan::New<FunctionTemplate>(Print)->GetFunction());
  exports->Set(Nan::New("length").ToLocalChecked(),
               Nan::New<FunctionTemplate>(Length)->GetFunction());
  exports->Set(Nan::New("delay").ToLocalChecked(),
                 Nan::New<FunctionTemplate>(Delay)->GetFunction());
}

NODE_MODULE(myaddon, Init)
