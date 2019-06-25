mkdir -p go/cpp/
swig -c++ -go -outdir go/ -I../ -o go/cpp/FloatArrayModule_wrap.cxx -package com.Mach1 FloatArrayModule.i 
swig -c++ -go -outdir go/ -I../ -o go/cpp/Mach1Point3DModule_wrap.cxx -package com.Mach1 Mach1Point3DModule.i 
swig -c++ -go -outdir go/ -I../ -o go/cpp/Mach1Point4DModule_wrap.cxx -package com.Mach1 Mach1Point4DModule.i 
swig -c++ -go -outdir go/ -I../ -o go/cpp/Mach1DecodeModule_wrap.cxx -package com.Mach1 Mach1DecodeModule.i 
swig -c++ -go -outdir go/ -I../ -o go/cpp/Mach1EncodeModule_wrap.cxx -package com.Mach1 Mach1EncodeModule.i 
swig -c++ -go -outdir go/ -I../ -o go/cpp/Mach1DecodePositionalModule_wrap.cxx -package com.Mach1 Mach1DecodePositionalModule.i 
echo "Removing intermediary modules"
rm -rf go/*Module.java