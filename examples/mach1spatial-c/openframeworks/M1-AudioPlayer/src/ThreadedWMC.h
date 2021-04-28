//
//  ThreadedWMC.h
//
//  Created by Dylan Marcus on 1/5/21.
//

#ifndef ThreadedWMC_h
#define ThreadedWMC_h

#include "ofMain.h"
#include "witmotionController.h"
#include <atomic>

class ThreadedWMC: public ofThread
{
public:
    witmotionController wmc;

    ~ThreadedWMC(){
        stop();
        waitForThread(false);
    }

    void setup(){
        wmc.setup();
        wmc.setRefreshRate(0x0b);
        start();
    }

    /// Start the thread.
    void start(){
        startThread();
    }

    void stop(){
        stopThread();
    }

    void threadedFunction(){
        while(isThreadRunning()){
            if (wmc.isConnected) {
                wmc.update();
                threadFrameNum++;
            }
        }
    }

    int getThreadFrameNum(){
        return threadFrameNum;
    }
    
protected:
    int threadFrameNum = 0;
};


#endif /* ThreadedWMC_h */
