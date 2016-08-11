#include <functional>
#include <map>
#include <vector>

#ifndef ARGHANDLER_H
#define ARGHANDLER_H

using namespace std;

struct Handler {
    function<void(vector<string>)> handle;
    function<void(void)> handleNotFound;
    int expectedArgs = -1;
    bool found = false;
};

class argErr : public std::exception
{
  public:
    argErr(string argName) : std::exception() {
        this->err = (string("Argument Error: ") + argName).c_str();
    }
    ~argErr() {}
    virtual const char* what() const throw()
    {
        return this->err.c_str();
    }
  protected:
    string err;
};

class tooFewArgs : public argErr {
  public:
    tooFewArgs(string argName) : argErr(argName) {
        this->err = string("Too Few Arguments for ") + argName;
    }
};

class argNotFound : public argErr {
  public:
    argNotFound(string argName) : argErr(argName) {
        this->err = string("Argument ") + argName + string(" not found!");
    }
};

class argHandler {
  public:
    void setArgs(int argc, char* argv[]);
    void setExplicitHandlerArgsNumber(map<string,int> handlerArgsNumbers);
    void setHandlers(map<string,function<void(vector<string>)>> handlers);
    void setNotFoundHandlers(map<string,function<void(void)>> handlers);
    void parse();
    vector<string> getArgs(Handler hand);

  private:
    int pos = 0;
    int argc;
    char **argv;
    map<string,Handler> handlerMap;
};
#endif
