//
//  ArduinoDecoder.h
//  M1 Player
//
//  Created by zebra on 21/09/16.
//
//

#ifndef ArduinoDecoder_h
#define ArduinoDecoder_h

class ArduinoDecoderYP: public ofSerial {
public:
    ArduinoDecoderYP() {
        
    }
    
    float lastY = 0, lastP = 0, lastR = 0;
    
    vector<unsigned char> queueBuffer;
    vector<unsigned char> queueBackupDebug;
    
    std::string queueString = "";
    
    std::function<void(float, float, float)> gotNewValues = [&](float YV, float PV, float RV) {};
    
    void update() {
        if (available()) {
            if (queueBuffer.size() > 0) {
                queueBuffer.clear();
            }
            vector<unsigned char> inputs;
            while (available() > 0) {
                unsigned char i = readByte();
                inputs.push_back(i);
            }
            for (int i = 0; i < inputs.size(); i++) {
                queueBuffer.insert(queueBuffer.begin() + i, (int)inputs[i]);
                
                queueString.push_back(inputs[i]);
            }
        }
        
        if (queueString.length() > 0) {
            float Y = lastY, P = lastP, R = lastR;
            
            auto decoded = decode3PieceString(queueString, 'Y', 'P', 'R', 4);
            
            bool anythingNewDetected = false;
            
            if (decoded.gotY) {
                Y = decoded.y;
                anythingNewDetected = true;
                
                lastY = Y;
            }
            
            if (decoded.gotP) {
                P = decoded.p;
                anythingNewDetected = true;
                
                lastP = P;
            }
            
            if (decoded.gotR) {
                R = decoded.r;
                anythingNewDetected = true;
                
                lastR = R;
            }
            
            if (anythingNewDetected)
                gotNewValues(Y, P, R);
            /*
             if (getNewDataFromQueue(Y, P, R)) {
             if ((lastY != Y) || (lastP != P) || (lastR != R)) {
             lastY = Y;
             lastP = P;
             lastR = R;
             gotNewValues(Y, P, R);
             }
             }
             */
            
            queueString.clear();
        }
        
        
    }
    
    struct CalcResult {
        bool gotY = false;
        bool gotP = false;
        bool gotR = false;
        
        float y, p, r;
    };
    
    static CalcResult decode3PieceString (std::string stringToDecode,
                                          char symbolY, char symbolP,
                                          char symbolR,
                                          int numberOfDecimalPlaces) {
        
        int cursor = 0;
        
        struct utility {
            static bool decodeNumberAtPoint(std::string str, int startIndex, int minDigitsAfterDot, std::string & decodedNumber) {
                int cursor = startIndex + 1;
                
                std::string numberString = "";
                
                bool stringFinished = false;
                int afterDotCounter = -99999;
                
                while ((cursor < str.length()) && (!stringFinished)) {
                    if ((std::isdigit(str[cursor])) || (str[cursor] == '.') || (str[cursor] == '-'))
                        numberString += str[cursor];
                    else stringFinished = true;
                    
                    if (str[cursor] == '.')
                        afterDotCounter = 0;
                    else {
                        if (std::isdigit(str[cursor]))
                            afterDotCounter++;
                    }
                    
                    cursor++;
                }
                
                //                ofLog() << "our string is " << numberString << "; numbers after dot = " << afterDotCounter;
                
                decodedNumber = numberString;
                
                if (afterDotCounter >= minDigitsAfterDot) {
                    return true;
                } else return false;
            }
        };
        
        CalcResult success;
        
        while (cursor < stringToDecode.size()) {
            
            const char currentChar = stringToDecode.at(cursor);
            
            std::string decodedNumber = "";
            
            if (currentChar == symbolY) {
                if (!utility::decodeNumberAtPoint(stringToDecode, cursor, 4, decodedNumber)) {
                    
                }
                else { // succeeded getting the number and there is enough decimals
                    success.gotY = true;
                    success.y = ofToFloat(decodedNumber);
                }
            }
            
            if (currentChar == symbolP) {
                if (!utility::decodeNumberAtPoint(stringToDecode, cursor, 4, decodedNumber)) {
                    
                }
                else { // succeeded getting the number and there is enough decimals
                    success.gotP = true;
                    success.p = ofToFloat(decodedNumber);
                }
            }
            
            if (currentChar == symbolR) {
                if (!utility::decodeNumberAtPoint(stringToDecode, cursor, 4, decodedNumber)) {
                    
                }
                else { // succeeded getting the number and there is enough decimals
                    success.gotR = true;
                    success.r = ofToFloat(decodedNumber);
                }
            }
            
            cursor++;
        }
        
        return success;
        
    };
    
    bool getNewDataFromQueue(float & Y, float & P, float & R) {
        
        
        auto decoded = decode3PieceString(queueString, 'Y', 'P', 'R', 4);
        
        bool anythingNewDetected = false;
        
        if (decoded.gotY) {
            Y = decoded.y;
            anythingNewDetected = true;
        }
        
        if (decoded.gotP) {
            P = decoded.p;
            anythingNewDetected = true;
        }
        
        if (decoded.gotR) {
            R = decoded.r;
            anythingNewDetected = true;
        }
        
        
        return anythingNewDetected;
        
    }
    
};

#endif /* ArduinoDecoder_h */

