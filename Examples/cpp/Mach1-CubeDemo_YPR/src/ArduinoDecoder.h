//
//  ArduinoDecoder.h
//  ControllerEncodeDecode
//
//  Created by zebra on 01/10/16.
//
//

#ifndef ArduinoDecoder_h
#define ArduinoDecoder_h


struct CalcResult {
    bool gotA = false;
    bool gotB = false;
    bool gotC = false;
    bool gotD = false;
};

struct CalcResult6Piece {
    bool gotA = false;
    bool gotB = false;
    bool gotC = false;
    bool gotD = false;
    bool gotE = false;
    bool gotF = false;
};


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
        
        //ofLog() << "our string is " << numberString << "; numbers after dot = " << afterDotCounter;
        
        decodedNumber = numberString;
        
        if (afterDotCounter >= minDigitsAfterDot) {
            return true;
        } else return false;
    }
};


// TODO: make success a 4 separate bools to support
static CalcResult6Piece decode6PieceString (std::string stringToDecode,
                                            char symbol1, char symbol2,
                                            char symbol3, char symbol4,
                                            char symbol5, char symbol6,
                                            float &a, float &b,
                                            float &c, float &d,
                                            float &e, float &f,
                                            int numberOfDecimalPlaces) {
    
    int cursor = 0;
    
    
    
    CalcResult6Piece success;
    
    while (cursor < stringToDecode.size()) {
        
        const char currentChar = stringToDecode.at(cursor);
        
        std::string decodedNumber = "";
        
        if (currentChar == symbol1) {
            //ofLog() << symbol1 << " is at " << cursor;
            if (!utility::decodeNumberAtPoint(stringToDecode, cursor, numberOfDecimalPlaces, decodedNumber)) {
                
            }
            else { // succeeded getting the number and there is enough decimals
                success.gotA = true;
                a = ofToFloat(decodedNumber);
            }
        }
        
        if (currentChar == symbol2) {
            //ofLog() << symbol2 << " is at " << cursor;
            if (!utility::decodeNumberAtPoint(stringToDecode, cursor, numberOfDecimalPlaces, decodedNumber)) {
                
            }
            else { // succeeded getting the number and there is enough decimals
                success.gotB = true;
                b = ofToFloat(decodedNumber);
            }
        }
        
        if (currentChar == symbol3) {
            //ofLog() << symbol3 << " is at " << cursor;
            if (!utility::decodeNumberAtPoint(stringToDecode, cursor, numberOfDecimalPlaces, decodedNumber)) {
                
            }
            else { // succeeded getting the number and there is enough decimals
                success.gotC = true;
                c = ofToFloat(decodedNumber);
            }
        }
        
        if (currentChar == symbol4) {
            // ofLog() << symbol4 << " is at " << cursor;
            if (!utility::decodeNumberAtPoint(stringToDecode, cursor, numberOfDecimalPlaces, decodedNumber)) {
                
            }
            else { // succeeded getting the number and there is enough decimals
                success.gotD = true;
                d = ofToFloat(decodedNumber);
            }
        }
        
        if (currentChar == symbol5) {
            ofLog() << symbol5 << " is at " << cursor;
            if (!utility::decodeNumberAtPoint(stringToDecode, cursor, numberOfDecimalPlaces, decodedNumber)) {
                
            }
            else { // succeeded getting the number and there is enough decimals
                success.gotE = true;
                e = ofToFloat(decodedNumber);
            }
        }
        
        if (currentChar == symbol6) {
            ofLog() << symbol6 << " is at " << cursor;
            if (!utility::decodeNumberAtPoint(stringToDecode, cursor, numberOfDecimalPlaces, decodedNumber)) {
                
            }
            else { // succeeded getting the number and there is enough decimals
                success.gotF = true;
                f = ofToFloat(decodedNumber);
            }
        }
        
        //            if (success)
        cursor++;
        //            else return false;
    }
    
    return success;
    
};



// TODO: make success a 4 separate bools to support
static CalcResult decode4PieceString (std::string stringToDecode,
                                      char symbol1, char symbol2,
                                      char symbol3, char symbol4,
                                      float &a, float &b,
                                      float &c, float &d,
                                      int numberOfDecimalPlaces) {
    
    int cursor = 0;
    
    CalcResult success;
    
    while (cursor < stringToDecode.size()) {
        
        const char currentChar = stringToDecode.at(cursor);
        
        std::string decodedNumber = "";
        
        if (currentChar == symbol1) {
            // ofLog() << symbol1 << " is at " << cursor;
            if (!utility::decodeNumberAtPoint(stringToDecode, cursor, numberOfDecimalPlaces, decodedNumber)) {
                
            }
            else { // succeeded getting the number and there is enough decimals
                success.gotA = true;
                a = ofToFloat(decodedNumber);
            }
        }
        
        if (currentChar == symbol2) {
            //ofLog() << symbol2 << " is at " << cursor;
            if (!utility::decodeNumberAtPoint(stringToDecode, cursor, numberOfDecimalPlaces, decodedNumber)) {
                
            }
            else { // succeeded getting the number and there is enough decimals
                success.gotA = true;
                b = ofToFloat(decodedNumber);
            }
        }
        
        if (currentChar == symbol3) {
            //ofLog() << symbol3 << " is at " << cursor;
            if (!utility::decodeNumberAtPoint(stringToDecode, cursor, numberOfDecimalPlaces, decodedNumber)) {
                
            }
            else { // succeeded getting the number and there is enough decimals
                success.gotA = true;
                c = ofToFloat(decodedNumber);
            }
        }
        
        if (currentChar == symbol4) {
            //ofLog() << symbol4 << " is at " << cursor;
            if (!utility::decodeNumberAtPoint(stringToDecode, cursor, numberOfDecimalPlaces, decodedNumber)) {
                
            }
            else { // succeeded getting the number and there is enough decimals
                success.gotA = true;
                d = ofToFloat(decodedNumber);
            }
        }
        
        //            if (success)
        cursor++;
        //            else return false;
    }
    
    return success;
    
};


class ArduinoDecoder: public ofSerial {
public:
    ArduinoDecoder() {
        
    }
    
    // Quaternion:
    
    ofQuaternion latestResult;
    float quatA, quatB, quatC, quatD;
    
    // 2 angle:
    
    int lastY = 0, lastP = 0;
    
    vector<unsigned char> queueBuffer;
    vector<unsigned char> queueBackupDebug;
    
    std::function<void(float, float, float)> gotNewValues = [&](float YV, float PV, float RV) {};
    
    void update() {
        if (available()) {
            
            while (available() > 0) {
                unsigned char i = readByte();
                queueBuffer.push_back(i);
            }
            
        }
        
        vector<unsigned char> queueBackupDebug = queueBuffer;
        while (queueBuffer.size() > 16) {
            
            
            float A, B, C, D, E, F;
            
            std::string queueBufferString = "";
            
            int howManyBytesToDecode = queueBuffer.size();
            if (howManyBytesToDecode > 100) {
                howManyBytesToDecode = 100;
            }
            
            for (auto i = 0; i < howManyBytesToDecode; i++) {
                queueBufferString += queueBuffer[i];
            }
            
            queueBuffer.clear();
            
            // Notice the following lines.
            // gotNewData is an array of bool, 6 pieces of bool and everyone of them
            // means that the responding number changed. You can decide here
            // what are you going to do if it's changed. Either way,
            // variables A, B, C, D are filled with the new data from string.
            
            //ofLog() << "bufer size = " << queueBuffer.size();
            
            auto gotNewData = decode4PieceString (queueBufferString,
                                                  '!', '@', '#', '$',
                                                  A, B, C, D,
                                                  2);
            
            if ((gotNewData.gotA) || (gotNewData.gotB) || (gotNewData.gotC)) {
                gotNewValues(A, B, C);
            }
            
            // The following is an example to how this can work. I put new numbers into
            // the quat variables.
            //
            
            /*
             auto gotNewData = decode4PieceString (queueBufferString,
             '!', '@', '#', '$',
             A, B, C, D,
             2);
             
             if (gotNewData.gotA) {
             quatA = A;
             }
             if (gotNewData.gotB) {
             quatB = B;
             }
             if (gotNewData.gotC) {
             quatC = C;
             }
             if (gotNewData.gotD) {
             quatD = D;
             }
             */
            
        }
        
    }
    
    
};


#endif /* ArduinoDecoder_h */
