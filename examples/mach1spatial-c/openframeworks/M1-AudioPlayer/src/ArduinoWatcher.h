
#ifndef ArduinoWatcher_h
#define ArduinoWatcher_h

#include "ofMain.h"

class ArduinoWatcher: public ofThread {
public:
    ArduinoWatcher() {
        
#ifndef _WIN32
        std::vector<std::string> result;
        
        result = separateString(ofSystem("ls /dev/cu.Mach1*"));
        
        
        if ((result.size() > 0) && (result.size() < 100))
            for (int i = 0; i < result.size(); i++) {
                ofLog() << i << " : " << result[i];
            }
#endif
        
    }
    
    void exit() {
        stopThread();
    }
    
    void start() {
        startThread();
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
        while (isThreadRunning()) {
            // Searching for new arduinos
            
            auto search = separateString(ofSystem("ls /dev/cu.Mach1*"));
            auto search2 = separateString(ofSystem("ls /dev/cu.usbserial-*"));
            auto search3 = separateString(ofSystem("ls /dev/cu.wchusbserial*"));
            auto search4 = separateString(ofSystem("ls /dev/cu.H-C-2010-06-01-DevB"));
            
            search.insert(search.end(), search2.begin(), search2.end());
            search.insert(search.end(), search3.begin(), search3.end());
            search.insert(search.end(), search4.begin(), search4.end());
            
            ofLog() << "total devices to check:" << search.size();
            for (int i = 0; i < search.size(); i++) {
                if (!searchMap(search[i])) {
                    ofLog() << "found new device: " << search[i];
                    
                    arduinoFound(search[i]);
                    
                    serialMap[search[i]] = true;
                    testSubjects.push_back(search[i]);
                }
            }
            
            ofLog() << "yet to test: " << testSubjects.size();
            //            if (testSubjects.size() == 0) {
            //                stopThread();
            //                return;
            //            }
            // Evaluating older ones
            
            while (testSubjects.size() > 0) {
                ofSerial serial;
                
                if (!isThreadRunning()) return;
                
                
                if (serial.setup(testSubjects[testSubjects.size() - 1], 115200)) {
                    float testStarted = ofGetElapsedTimef();
                    int gotBytes = 0;
                    std::string result = "";
                    while (((ofGetElapsedTimef() - testStarted) < 2)
                           && (gotBytes < 30)) {
                        
                        if (!isThreadRunning()) return;
                        
                        while ((serial.available()) && (gotBytes < 30)) {
                            
                            if (!isThreadRunning()) return;
                            
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
                    ofLog() << "Code Count: " << slCount << " ; time:" << ofGetElapsedTimef();
                    
                    //serial.flush();
                    //serial.close();
                    //while (serial.isInitialized()) {};
                    if (slCount > 4) arduinoFound(testSubjects[testSubjects.size() - 1]);
                    
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
        std::vector<std::string> result = std::vector<std::string>();
        
        if (arg.length() < 4) return std::vector<std::string>();
        
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

