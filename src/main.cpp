#define USE_ARDUINO_INTERRUPTS true

#include <Arduino.h>
#include <PulseSensorPlayground.h>
#include <ArduinoJson.h>

#define PULSE_SENSOR_PIN 0 // 'S' Signal pin connected to A0
#define LED13 13		   // The on-board Arduino LED

int Threshold = 550;			   // Determine which Signal to "count as a beat" and which to ignore
PulseSensorPlayground pulseSensor; // Creates an object
int myBPM = 0; // Calculates BPM
int myECG = 0;

void setup()
{
	Serial.begin(9600);

	pinMode(10, INPUT); // Setup for leads off detection LO +
	pinMode(11, INPUT); // Setup for leads off detection LO -

	// Configure the PulseSensor object, by assigning our variables to it
	pulseSensor.analogInput(PULSE_SENSOR_PIN);
	pulseSensor.blinkOnPulse(LED13); // Blink on-board LED with heartbeat
	pulseSensor.setThreshold(Threshold);

	// Double-check the "pulseSensor" object was created and began seeing a signal
	if (pulseSensor.begin())
	{
		// Serial.println("PulseSensor object created!");
	}
}

void loop()
{
	// put your main code here, to run repeatedly:
	myBPM = pulseSensor.getBeatsPerMinute(); // Calculates BPM

	if (pulseSensor.sawStartOfBeat())
	{												  // Constantly test to see if a beat happened
		// Serial.println("♥  A HeartBeat Happened ! "); // If true, print a message
		// Serial.print("BPM: ");
		// Serial.println(myBPM); // Print the BPM value
	}

	if ((digitalRead(10) == 1) || (digitalRead(11) == 1))
	{
		// Serial.println('!');
	}
	else
	{
		// send the value of analog input 0:
		myECG = analogRead(A1);
		// Serial.println(myECG);
	}
	StaticJsonDocument<200> doc;
	doc["ecg"] = myECG;
	doc["heartrate"] = myBPM;
	String json;
	serializeJson(doc, json);
	Serial.println(json);

	delay(1000);
}