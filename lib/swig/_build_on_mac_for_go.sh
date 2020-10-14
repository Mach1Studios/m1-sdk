mkdir -p go/cpp/
swig -c++ -go -outdir go/ -I../ -o go/cpp/Mach1FloatArrayModule_wrap.cxx -package com.mach1.spatiallibs Mach1FloatArrayModule.i 
swig -c++ -go -outdir go/ -I../ -o go/cpp/Mach1Point3DModule_wrap.cxx -package com.mach1.spatiallibs Mach1Point3DModule.i 
swig -c++ -go -outdir go/ -I../ -o go/cpp/Mach1Point4DModule_wrap.cxx -package com.mach1.spatiallibs Mach1Point4DModule.i 
swig -c++ -go -outdir go/ -I../ -o go/cpp/Mach1DecodeModule_wrap.cxx -package com.mach1.spatiallibs Mach1DecodeModule.i 
swig -c++ -go -outdir go/ -I../ -o go/cpp/Mach1EncodeModule_wrap.cxx -package com.mach1.spatiallibs Mach1EncodeModule.i 
swig -c++ -go -outdir go/ -I../ -o go/cpp/Mach1DecodePositionalModule_wrap.cxx -package com.mach1.spatiallibs Mach1DecodePositionalModule.i 