#include <Servo.h>
#include <RF24.h>

RF24 _radio(7, 8);
uint8_t	_control;
unsigned long _timestamp;

const byte _radio_address[6] = "00001";

uint8_t histL = 0; 
uint8_t histR = 0;

uint8_t histF = 0;
uint8_t histB = 0;


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  
	_radio.begin();
  _radio.openWritingPipe(_radio_address);
  _radio.stopListening();
  
	pinMode(2, INPUT_PULLUP);
	pinMode(3, INPUT_PULLUP);
	pinMode(4, INPUT_PULLUP);
	pinMode(5, INPUT_PULLUP);

	_control = 0;
	_timestamp = millis();

  Serial.println("Setup");
}

void updateControl(uint8_t* pHist, uint8_t pin, uint8_t bit)
{
	*pHist = (*pHist << 1) | !digitalRead(pin);

	if ((*pHist & 0b11000111) == 0b00000111)
	{
    Serial.println("Pressed");
		
		*pHist = 0xFF;
		_control |= bit;
	}
	
	if ((*pHist & 0b11000111) == 0b11000000)
	{
    Serial.println("Released");

		_control &= ~bit;
		*pHist = 0x00;
	}
}

// the loop function runs over and over again until power down or reset
void loop() {

	updateControl(&histL, 2, 0x01);
	updateControl(&histR, 3, 0x02);
	updateControl(&histF, 4, 0x04);
	updateControl(&histB, 5, 0x08);
	
	if (millis() - _timestamp > 100)
	{
		_radio.write(&_control, sizeof(_control));
		_timestamp = millis();
	}

	delay(10);
}
