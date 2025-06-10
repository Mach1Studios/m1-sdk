%module Mach1UtilsModule

%include "typemaps.i"
%include "cpointer.i"
%include "carrays.i"
%include "various.i"

%inline %{
char* convertToString(void* in)
{
    return (char*)in;
}

void* getitemVoid(void* in, int index)
{
    return (void*)((void**)in)[index];
}
%}

%pragma(java) jniclassclassmodifiers="class"
%pragma(java) jniclasscode=%{ 
  static {
    System.loadLibrary("Mach1UtilsModule_lib");
  }
%}

