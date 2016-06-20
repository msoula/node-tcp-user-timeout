// hello.cc
#include <node.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

using namespace v8;

namespace ndmac {

    static int ndmac_tcp_user_timeout(int fd, unsigned int timeout) {
        if (setsockopt(fd, SOL_TCP, TCP_USER_TIMEOUT, (void *)&timeout, sizeof(timeout)))
            return -errno;
        return 0;
    }

    void SetTcpUserTimeout(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = Isolate::GetCurrent();
        HandleScope scope(isolate);

        if (args.Length() < 2) {
            isolate->ThrowException(Exception::TypeError(
                        String::NewFromUtf8(isolate, "Wrong number of arguments")));
            return;
        }

        if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
            isolate->ThrowException(Exception::TypeError(
                        String::NewFromUtf8(isolate, "Wrong arguments")));
            return;
        }

        int fd               = args[0]->NumberValue();
        unsigned int timeout = args[1]->NumberValue();

        Local<Number> err = Number::New(isolate, ndmac_tcp_user_timeout(fd, timeout));
        args.GetReturnValue().Set(err);
    }

    void Init(Handle<Object> exports) {
        Isolate* isolate = Isolate::GetCurrent();
        exports->Set(String::NewFromUtf8(isolate, "setSocketTcpUserTimeout"),
                FunctionTemplate::New(isolate, SetTcpUserTimeout)->GetFunction());
    }
    NODE_MODULE(ndmac, Init)

}  // namespace demo

