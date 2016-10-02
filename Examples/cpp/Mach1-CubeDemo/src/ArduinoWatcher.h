
#ifndef ArduinoWatcher_h
#define ArduinoWatcher_h

#include "ofMain.h"

class ArduinoWatcher: public ofThread {
public:
    ArduinoWatcher() {
#ifndef _WIN32
        auto result = separateString(ofSystem("ls /dev/cu*"));
        for (int i = 0; i < result.size(); i++) {
            ofLog() << i << " : " << result[i];
        }
        startThread();
#endif
    }
    
    void exit() {
        stopThread();
    }
    
    std::function<void(std::string address)> arduinoFound = [](std::string address) { ofLog() << "FOUND at " << address; };
    
    size_t stringCount(const std::string& referenceString,
                       const std::string& subString) {
        
        const size_t step = subString.size();
        
        size_t count(0);
        size_t pos(0) ;
        
        while( (pos=referenceString.find(subString, pos)) !=std::string::npos) {
            pos +=step;
            ++count ;
        }
        
        return count;
        
    }
    
    void threadedFunction() {
        while (true) {
            // Searching for new arduinos
            
            auto search = separateString(ofSystem("ls /dev/cu.Mach1*"));
            for (int i = 0; i < search.size(); i++) {
                if (!searchMap(search[i])) {
                    ofLog() << "found new device: " << search[i];
                    serialMap[search[i]] = true;
                    testSubjects.push_back(search[i]);
                }
            }
            
            // Evaluating older ones
            
            while (testSubjects.size() > 0) {
                ofSerial serial;
                //ofSleepMillis(500);
                if (serial.setup(testSubjects[testSubjects.size() - 1], 115200)) {
                    float testStarted = ofGetElapsedTimef();
                    int gotBytes = 0;
                    std::string result = "";
                    while (((ofGetElapsedTimef() - testStarted) < 2)
                           && (gotBytes < 100)) {
                        while ((serial.available()) && (gotBytes < 100)) {
                            result += (char)serial.readByte();
                            gotBytes++;
                        }
                    }
                    ofLog() << "got data: " << result;
                    for (int i = 0; i < result.size(); i++) {
                        ofLog() << " ::: " << result[i];
                        
                    }
                    ofLog() << "how many symbols: " << result.length();
                    ofLog() << ((ofGetElapsedTimef() - testStarted) > 7);
                    ofLog() << (gotBytes > 10);
                    
                    //                        ofLog() << "got from serial: " << result;
                    
                    std::string testStr;
                    testStr += '\377';
                    int slCount = stringCount(result, testStr);
                    ofLog() << "Code Count: " << slCount;
                    
                    serial.close();
                    if (slCount > 10) arduinoFound(testSubjects[testSubjects.size() - 1]);
                    
                } else {
                    ofLog() << "failed to open port " << testSubjects[testSubjects.size() - 1];
                }
                testSubjects.pop_back();
            }
        }
    }
    
    bool searchMap(std::string arg) {
        auto f = serialMap.find(arg);
        return (f != serialMap.end());
    }
    
    std::vector<std::string> separateString(std::string arg) {
        int i = 0, lastWordIndex = 0;
        std::vector<std::string> result;
        
        if (arg.length() < 4) return;
        
        while (i <= arg.size()-1) {
            if ((arg[i] == 10)||(i == arg.size() - 1)) {
                if (i != 0) {
                    result.push_back(arg.substr(lastWordIndex, i - lastWordIndex));
                }
                lastWordIndex = i + 1;
            }
            
            
            i++;
        }
        return result;
    }
    
    ofSerial serial;
    std::map<std::string, bool> serialMap;
    std::vector<std::string> testSubjects;
    std::string currentPort = "";
};

#endif /* ArduinoWatcher_h */
