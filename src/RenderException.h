#ifndef RENDEREXCEPTION_H
#define RENDEREXCEPTION_H

#include <exception>
#include <string>
using namespace std;

class RenderException : public exception {
  public:
    RenderException(string msg) : msg(msg) { }
    virtual ~RenderException() throw() { }
    virtual const char* what() const throw() { return msg.c_str(); }
  private:
    const string msg;
};
#endif
