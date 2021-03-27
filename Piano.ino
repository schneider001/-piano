#define ON_PRESS 1
#define BUZZER_PIN 6

#define LEDPIN 3

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) 
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

class Piano
{
public:
    Piano(uint8_t = 255, uint8_t = 255, uint8_t = 255, uint8_t = 255, uint8_t = 255, uint8_t = 255, uint8_t = 255);

    uint8_t _redPin = 10;
    uint8_t _greenPin = 5;
    uint8_t _bluePin = 9;
  
    void read();

    void begin();

    bool pressed();

    void setColor(int key);

    uint8_t getNum = 0;

    char getChar = 0;

    enum _color
    {
        RED = 0,
        YELLOW = 1,
        GREEN = 2,
        LIGHT_BLUE = 3,
        BLUE = 4,
        PURPLE = 5 
    };

private:
    uint8_t _pinsKB[7];

    uint8_t _state;

    uint8_t _numberKeyNow;

    uint8_t _massNumberKey[16] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xE, 0x0, 0xF, 0x0, 0x0, 0x0, 0x0};

    void _findPressKeyNow();
};

Piano::Piano(uint8_t pin1KB, uint8_t pin2KB, uint8_t pin3KB, uint8_t pin4KB, uint8_t pin5KB, uint8_t pin6KB, uint8_t pin7KB)
{
    _pinsKB[0] = pin1KB;
    _pinsKB[1] = pin2KB;
    _pinsKB[2] = pin3KB;
    _pinsKB[3] = pin4KB;
    _pinsKB[4] = pin5KB;
    _pinsKB[5] = pin6KB;
    _pinsKB[6] = pin7KB;
}

void Piano::begin()
{
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);

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

void Piano::read()
{
    _state = false;

    _findPressKeyNow();
    if (_numberKeyNow != 255)
    {
        getNum = _massNumberKey[_numberKeyNow];
        _state = ON_PRESS;
    }
}

bool Piano::pressed()
{
    return _state == ON_PRESS ? 1 : 0;
}

void Piano::_findPressKeyNow()
{
    _numberKeyNow = 255;

    for (uint8_t i = 0; i < 4; i++)
    {
        digitalWrite(_pinsKB[i], LOW);
        for (uint8_t j = 0; j < 3; j++)
        {

            if (!digitalRead(_pinsKB[j + 4]))
            {
                _numberKeyNow = i * 3 + j;
            }
        }
        digitalWrite(_pinsKB[i], HIGH);
    }
}

void Piano::setColor(int key)
{
    int red = 0;
    int green = 0;
    int blue = 0;

    Serial.println(key);

    if (key == 1 || key == 7)
    {
      red = 255;
    }
    else if (key == 2 || key == 8)
    {
      red = 255;
      green = 80;
    }
    else if (key == 3 || key == 9)
    {
      green = 255;
    }
    else if (key == 4 || key == 10)
    {
      blue = 255;
      green = 255;
    }
    else if (key == 5 || key == 11)
    {
      blue = 255;
    }
    else if (key == 6 || key == 12)
    {
      red = 255;
      blue = 255;
    }
    
    analogWrite(_redPin, red);
    analogWrite(_greenPin, green);
    analogWrite(_bluePin, blue);
}

Piano piano(13, 12, 6, 2, 4, 8, 7);

const int resolution = 200;
char wave[resolution];

void setup()
{
    Serial.begin(9600);
    piano.begin();
    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, HIGH);
      Serial.begin(9600);      // открываем последовательное соединение

    // инициализация Timer1
    cli();  // отключить глобальные прерывания
    //Настройка таймеров 1 и 2
    TCCR1A = 0;   
    TCCR1B = 0;
    TCCR2A = 0;
    TCCR2B = 0;

    sbi(TCCR1B, WGM12); //Режим CTC
    sbi(TCCR1B, CS10);  //Тактовая частота не делится
    
    sbi(TCCR2A, WGM21);
    sbi(TCCR2A, WGM20); //Режим Fast PWM
    sbi(TCCR2B, CS20);  //Тактовая частота не делится

    sbi(TIMSK1, OCIE1A);  //Прерывание по совпадению с OCR1A
    Serial.print(TIMSK1); //Отладочная информация

    for (int i = 0; i < resolution; i++)
    {
        wave[i] = 127. + 127. * sin(2. * PI / (float)(resolution - 1) * (float)i);
    }                     //Массив со значениями синуса

    setFreq(0);
    
    sei(); //Разрешить прерывания
}

void loop()
{
    int frequency = 475.73 / 4;

    piano.read();

    if (piano.pressed())
    {
        int i = piano.getNum;
        if (i == 0)
        {
            i = 11;
        }
        if (i == 14)
        {
            i = 10;
        }
        if (i == 15)
        {
            i = 12;
        }

        Serial.println(i);
        piano.setColor(i);
        setFreq(frequency * pow(1.0595, i));
    }
    else
    {
        setFreq(0);
        piano.setColor(0);
    }
}

ISR(TIMER1_COMPA_vect)  //Обработка прерывания
{
    static int t = 0;
    analogWritePin3(wave[t]);
    t = (t + 1) % 100;
}

void setFreq(int freq)
{
    if (freq)
    {
        sbi(TIMSK1, OCIE1A);  //Разрешить прерывания по совпадению
        OCR1A = 16000000. / (float) resolution / (float)freq;
    }
    else
        cbi(TIMSK1, OCIE1A);  //Запретить прерывания по совпадению
}

void analogWritePin3(int val) //Ускоренная версия analogWrite() для пина 3
{
    sbi(TCCR2A, COM2B1);
    OCR2B = val;
}
