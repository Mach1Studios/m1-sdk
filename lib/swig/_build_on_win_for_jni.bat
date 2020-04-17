mkdir jni\java\com\mach1\Mach1SpatialLibs
mkdir jni\cpp\mach1
D:\swigwin-4.0.0\swig.exe -c++ -java -outdir jni\java\com\mach1\Mach1SpatialLibs -I..\ -o jni\cpp\mach1\Mach1FloatArrayModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1FloatArrayModule.i 
D:\swigwin-4.0.0\swig.exe -c++ -java -outdir jni\java\com\mach1\Mach1SpatialLibs -I..\ -o jni\cpp\mach1\Mach1Point3DModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1Point3DModule.i 
D:\swigwin-4.0.0\swig.exe -c++ -java -outdir jni\java\com\mach1\Mach1SpatialLibs -I..\ -o jni\cpp\mach1\Mach1Point4DModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1Point4DModule.i 
D:\swigwin-4.0.0\swig.exe -c++ -java -outdir jni\java\com\mach1\Mach1SpatialLibs -I..\ -o jni\cpp\mach1\Mach1DecodeModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1DecodeModule.i 
D:\swigwin-4.0.0\swig.exe -c++ -java -outdir jni\java\com\mach1\Mach1SpatialLibs -I..\ -o jni\cpp\mach1\Mach1EncodeModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1EncodeModule.i 
D:\swigwin-4.0.0\swig.exe -c++ -java -outdir jni\java\com\mach1\Mach1SpatialLibs -I..\ -o jni\cpp\mach1\Mach1DecodePositionalModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1DecodePositionalModule.i
D:\swigwin-4.0.0\swig.exe -c++ -java -outdir jni\java\com\mach1\Mach1SpatialLibs -I..\ -o jni\cpp\mach1\Mach1UtilsModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1UtilsModule.i
del /Q jni\java\com\mach1\*Module.java
pause
