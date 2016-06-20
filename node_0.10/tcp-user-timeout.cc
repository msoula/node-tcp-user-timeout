// hello.cc
#include <node.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace v8;

namespace ndmac {

    static int ndmac_tcp_user_timeout(int fd, unsigned int timeout) {
        if (setsockopt(fd, SOL_TCP, TCP_USER_TIMEOUT, (void *)&timeout, sizeof(timeout)))
            return -errno;
        return 0;
    }

    Handle<Value> SetTcpUserTimeout(const Arguments& args) {
        HandleScope scope;

        if (args.Length() < 2) {
            ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
            return scope.Close(Undefined());
        }

        if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
            ThrowException(Exception::TypeError(String::New("Wrong arguments")));
            return scope.Close(Undefined());
        }

        int fd               = args[0]->NumberValue();
        unsigned int timeout = args[1]->NumberValue();

        Local<Number> err = Number::New(ndmac_tcp_user_timeout(fd, timeout));
        return scope.Close(err);
    }

    void Init(Handle<Object> exports) {
        exports->Set(String::NewSymbol("setSocketTcpUserTimeout"),
                FunctionTemplate::New(SetTcpUserTimeout)->GetFunction());
    }
    NODE_MODULE(ndmac, Init)

}  // namespace demo

