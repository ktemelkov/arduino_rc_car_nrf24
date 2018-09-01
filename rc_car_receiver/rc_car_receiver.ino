#include <Servo.h>
#include <RF24.h>

#define WHEELS_STRAIGHT 90
#define WHEELS_LEFT     106
#define WHEELS_RIGHT    73


RF24 _radio(7, 8);
Servo _servo;
uint8_t	_control;
unsigned long _timestamp;

const byte _radio_address[6] = "00001";

void setup()
{
  Serial.begin(9600);
  
	_radio.begin();
	_radio.openReadingPipe(0, _radio_address);
	_radio.startListening();

	_servo.attach(9);
	_servo.write(90);

	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);

	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
	digitalWrite(4, HIGH);

	_control = 0;
	_timestamp = millis();

  Serial.println("Setup ...");
}


void apply()
{
	if (_control & 0x01)
	{
		_servo.write(WHEELS_LEFT);
    Serial.println("Left");
	} 
	else if (_control & 0x02)
	{
		_servo.write(WHEELS_RIGHT);
    Serial.println("Right");
	}
	else
	{
		_servo.write(WHEELS_STRAIGHT);
	}

	if (_control & 0x04)
	{
		digitalWrite(2, LOW);
		digitalWrite(3, HIGH);
    Serial.println("Forward");
	}
	else if (_control & 0x08)
	{
		digitalWrite(2, HIGH);
		digitalWrite(3, LOW);
    Serial.println("Backward");
	}
	else
	{
		digitalWrite(2, LOW);
		digitalWrite(3, LOW);
	}
}


void loop()
{
	if (_radio.available())
	{
		_radio.read(&_control, 1);
		_timestamp = millis();

    Serial.println("Data");
	}

	if (millis() - _timestamp > 2000)
  {
    Serial.println("Reset");    
    _control = 0;
    _timestamp = millis();
  }
 
	apply();

	delay(10);
}

