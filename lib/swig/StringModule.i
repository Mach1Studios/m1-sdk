%module StringModule

%include "typemaps.i"
%include "cpointer.i"
%include "carrays.i"
%include "various.i"

%inline %{
char* convertToString(void* in)
{
    return (char*)in;
}
%}

%pragma(java) jniclassclassmodifiers="class"
%pragma(java) jniclasscode=%{ 
  static {
    System.loadLibrary("StringModule_lib");
  }
%}
