%module FloatArrayModule

%include "typemaps.i"
%include "cpointer.i"
%include "carrays.i"

%typemap(javacode) floatArray %{
  public long asVoidPtr() {
    return getCPtr(this);    
  }
%}

%typemap(javafinalize) floatArray %{
  @SuppressWarnings("deprecation")
  protected void finalize() {
    //delete();
  }
%}

%pragma(java) jniclassclassmodifiers="class"
%pragma(java) jniclasscode=%{ 
  static {
    System.loadLibrary("FloatArrayModule_lib");
  }
%}

%array_class(float, floatArray);
