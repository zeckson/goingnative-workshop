#include <nan.h>
#ifndef _WIN32
#include <unistd.h>
#endif

using namespace v8;

// A worker class extending the NanAsyncWorker helper
// class, a simple encapsulation of worker-thread
// logic to make simple tasks easier

class MyWorker : public Nan::AsyncWorker {
 public:
  // Constructor
  MyWorker(Nan::Callback *callback, int delay)
    : Nan::AsyncWorker(callback), delay(delay) {}
  // Destructor
  ~MyWorker() {}

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute () {
      #ifdef _WIN32
      Sleep(this->delay);
      #else
      usleep(this->delay * 1000);
      #endif
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback () {
    Nan::HandleScope scope;

    // Nan::Callback#Call() does a Nan::MakeCallback() for us
    callback->Call(0, NULL);
  }

 private:
  int delay;
};

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

  Nan::AsyncQueueWorker(new MyWorker(new Nan::Callback(cb), value));
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
