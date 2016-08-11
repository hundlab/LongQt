#include "argHandler.h"
using namespace std;

void argHandler::setArgs(int argc, char *argv[]) {
    this->argc = argc;
    this->argv = argv;
}

void argHandler::setExplicitHandlerArgsNumber(map<string,int> handlerArgsNumbers) {
    for(auto handlerArgsNumber: handlerArgsNumbers) {
        this->handlerMap[handlerArgsNumber.first].expectedArgs = handlerArgsNumber.second;
    }
}

void argHandler::setHandlers(map<string,function<void(vector<string>)>> handlers) {
    for(auto handler : handlers) {
        this->handlerMap[handler.first].handle = handler.second;
    }
}

void argHandler::setNotFoundHandlers(map<string,function<void(void)>> handlers) {
     for(auto handler : handlers) {
        this->handlerMap[handler.first].handleNotFound = handler.second;
    }
}

void argHandler::parse() {
    for(pos = 1; pos < argc; pos++) {
        for(auto& handlerPair : this->handlerMap) {
            if(0 == handlerPair.first.compare(argv[pos])) {
                handlerPair.second.handle(getArgs(handlerPair.second));
                handlerPair.second.found = true;
            } else {
                throw argNotFound(argv[pos]);
            }
        }
    }
    for(auto handlerPair : this->handlerMap) {
        if(!handlerPair.second.found) {
            handlerPair.second.handleNotFound();
        }
    }
}

vector<string> argHandler::getArgs(Handler hand) {
    vector<string> args;
    if(hand.expectedArgs >= 0) {
        int j = 1;
        for(j = 0; j <= hand.expectedArgs && j+pos < argc; j++) {
            args[j] = string(argv[j+pos]);
        }
        if(j < hand.expectedArgs) {
            throw tooFewArgs(argv[pos]);
        }
        pos = pos + j;
    } else {
        int j = 1;
        while(j+pos < argc && '-' != argv[j+pos][0]) {
            args[j] = string(argv[j+pos]);
        }
    }
    return args; 
}
