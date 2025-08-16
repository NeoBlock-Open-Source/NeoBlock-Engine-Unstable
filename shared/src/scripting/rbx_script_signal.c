#include "rbx_script_signal.h"
#include <stdlib.h>

struct RBXScriptSignal {
    Instance* Instance;
    char SignalName[64];
    RBXScriptConnection** Connections;
    int ConnectionsCount;
};

struct RBXScriptConnection {
    RBXScriptSignal* Signal;
    LuaFunctionCallback Callback;
    void* UserData;
    bool Connected;
};

RBXScriptSignal* RBXScriptSignalNew(Instance* instance, const char* signalName) {
    RBXScriptSignal* signal = malloc(sizeof(RBXScriptSignal));
    signal->Instance = instance;
    strncpy(signal->SignalName, signalName, 64);
    signal->Connections = NULL;
    signal->ConnectionsCount = 0;
    return signal;
}

void RBXScriptSignalFire(RBXScriptSignal* signal, int numArgs, ...) {
    va_list args;
    va_start(args, numArgs);
    
    for (int i = 0; i < signal->ConnectionsCount; i++) {
        if (signal->Connections[i]->Connected) {
            signal->Connections[i]->Callback(
                signal->Instance,
                signal->Connections[i]->UserData,
                numArgs,
                args
            );
        }
    }
    
    va_end(args);
}

RBXScriptConnection* RBXScriptSignalConnect(
    RBXScriptSignal* signal, 
    LuaFunctionCallback callback,
    void* userData
) {
    RBXScriptConnection* conn = malloc(sizeof(RBXScriptConnection));
    conn->Signal = signal;
    conn->Callback = callback;
    conn->UserData = userData;
    conn->Connected = true;
    
    signal->Connections = realloc(
        signal->Connections, 
        (signal->ConnectionsCount + 1) * sizeof(RBXScriptConnection*)
    );
    signal->Connections[signal->ConnectionsCount++] = conn;
    
    return conn;
}
