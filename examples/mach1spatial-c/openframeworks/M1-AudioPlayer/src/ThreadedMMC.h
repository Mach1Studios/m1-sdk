//
//  ThreadedMMC.h
//
//  Created by Dylan Marcus on 1/5/21.
//

#ifndef ThreadedMMC_h
#define ThreadedMMC_h

#include "ofMain.h"
#include "metamotionController.h"
#include <atomic>

class ThreadedMMC: public ofThread
{
public:
    metamotionController mmc;
    
    ~ThreadedMMC(){
        stop();
        waitForThread(false);
    }

    void setup(){
        mmc.setup();
        start();
    }

    /// Start the thread.
    void start(){
        startThread();
    }

    void stop(){
        mmc.disconnectDevice();
        stopThread();
    }

    void threadedFunction(){
        while(isThreadRunning()){
            mmc.update();
            threadFrameNum++;
        }
    }

    int getThreadFrameNum(){
        return threadFrameNum;
    }
    
protected:
    int threadFrameNum = 0;
};


#endif /* ThreadedMMC_h */
