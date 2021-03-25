#define ON_PRESS        1
#define BUZZER_PIN      6

class AmperkaKB
{
public:
    AmperkaKB(uint8_t = 255, uint8_t = 255, uint8_t = 255, uint8_t = 255, uint8_t = 255, uint8_t = 255, uint8_t = 255);
   
    void read();

    void begin();
    
    bool justPressed();
 
    uint8_t getNum = 0;
  
    char getChar = 0;
private:
    uint8_t _pinsKB[7];
    
    uint8_t _state;
    
    uint8_t _numberKeyNow;
    
    uint8_t _massNumberKey[16] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xE, 0x0, 0xF, 0x0, 0x0, 0x0, 0x0};

    char _massCharKey[16] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#', '0', '0', '0', '0'};
       

    void _findPressKeyNow();
};


AmperkaKB::AmperkaKB(uint8_t pin1KB, uint8_t pin2KB, uint8_t pin3KB, uint8_t pin4KB, uint8_t pin5KB, uint8_t pin6KB, uint8_t pin7KB) {
    _pinsKB[0] = pin1KB;
    _pinsKB[1] = pin2KB;
    _pinsKB[2] = pin3KB;
    _pinsKB[3] = pin4KB;
    _pinsKB[4] = pin5KB;
    _pinsKB[5] = pin6KB;
    _pinsKB[6] = pin7KB;
}


void AmperkaKB::begin() {
  
     pinMode(_pinsKB[0], OUTPUT);
     pinMode(_pinsKB[1], OUTPUT);
     pinMode(_pinsKB[2], OUTPUT);
     pinMode(_pinsKB[3], OUTPUT);
     digitalWrite(_pinsKB[0], HIGH);
     digitalWrite(_pinsKB[1], HIGH);
     digitalWrite(_pinsKB[2], HIGH);
     digitalWrite(_pinsKB[3], HIGH);
          
     pinMode(_pinsKB[4], INPUT_PULLUP);
     pinMode(_pinsKB[5], INPUT_PULLUP);
     pinMode(_pinsKB[6], INPUT_PULLUP);
        
}


void AmperkaKB::read() {
    _state = false;
    
    _findPressKeyNow();
    if (_numberKeyNow != 255) {
        getNum =  _massNumberKey[_numberKeyNow];
        getChar = _massCharKey[_numberKeyNow];
        _state = ON_PRESS;
    }
}

bool AmperkaKB::justPressed() {
    return _state == ON_PRESS ? 1 : 0;
}

void AmperkaKB::_findPressKeyNow() {
    _numberKeyNow = 255;
   
    for (uint8_t i = 0; i < 4; i++) {
                
        digitalWrite(_pinsKB[i], LOW);
        for (uint8_t j = 0; j < 3; j++) {
                    
            if (!digitalRead(_pinsKB[j + 4])) {
                _numberKeyNow = i * 3 + j;
            }
        }
        
        digitalWrite(_pinsKB[i], HIGH);
    }
}


AmperkaKB KB(13, 12, 11, 10, 9, 8, 7);

 
void setup() {
  Serial.begin(9600);
  KB.begin();
  pinMode(BUZZER_PIN, OUTPUT);
}
 
void loop() {
    int frequency = 475.73 / 4;
    KB.read();
    if (KB.justPressed()) {
        int i = KB.getNum;
        if (i == 0) {
            i = 11;
        }
        if (i == 14) {
            i = 10;
        }
        if (i == 15) {
            i = 12;
        }
        Serial.println(i);
        tone(BUZZER_PIN, frequency * pow(1.0595, i), 10);
    }
}
