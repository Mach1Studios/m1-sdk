mkdir jni\java\com\mach1\spatiallibs
mkdir jni\cpp\mach1
..\..\..\swigwin\swig.exe -c++ -java -outdir jni\java\com\mach1\spatiallibs -I..\ -o jni\cpp\mach1\Mach1FloatArrayModule_wrap.cxx -package com.mach1.spatiallibs Mach1FloatArrayModule.i 
..\..\..\swigwin\swig.exe -c++ -java -outdir jni\java\com\mach1\spatiallibs -I..\ -o jni\cpp\mach1\Mach1Point3DModule_wrap.cxx -package com.mach1.spatiallibs Mach1Point3DModule.i 
..\..\..\swigwin\swig.exe -c++ -java -outdir jni\java\com\mach1\spatiallibs -I..\ -o jni\cpp\mach1\Mach1Point4DModule_wrap.cxx -package com.mach1.spatiallibs Mach1Point4DModule.i 
..\..\..\swigwin\swig.exe -c++ -java -outdir jni\java\com\mach1\spatiallibs -I..\ -o jni\cpp\mach1\Mach1DecodeModule_wrap.cxx -package com.mach1.spatiallibs Mach1DecodeModule.i 
..\..\..\swigwin\swig.exe -c++ -java -outdir jni\java\com\mach1\spatiallibs -I..\ -o jni\cpp\mach1\Mach1EncodeModule_wrap.cxx -package com.mach1.spatiallibs Mach1EncodeModule.i 
..\..\..\swigwin\swig.exe -c++ -java -outdir jni\java\com\mach1\spatiallibs -I..\ -o jni\cpp\mach1\Mach1DecodePositionalModule_wrap.cxx -package com.mach1.spatiallibs Mach1DecodePositionalModule.i
..\..\..\swigwin\swig.exe -c++ -java -outdir jni\java\com\mach1\spatiallibs -I..\ -o jni\cpp\mach1\Mach1UtilsModule_wrap.cxx -package com.mach1.spatiallibs Mach1UtilsModule.i
del /Q jni\java\com\mach1\*Module.java
pause
