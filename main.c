#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float Celsius = 0;

const int rainfall_pin = A3;   // Rainfall sensor pin
const int forward_relay_pin = 2;   // Relay control pin for forward direction
const int reverse_relay_pin = 3;   // Relay control pin for reverse direction

bool forwardConditionSatisfied = false;  // Flag to track forward direction condition
bool reverseConditionSatisfied = false;  // Flag to track reverse direction condition

void setup() {
  sensors.begin();
  Serial.begin(9600);    // Initialize serial communication
  pinMode(forward_relay_pin, OUTPUT);  // Set forward relay pin as output
  pinMode(reverse_relay_pin, OUTPUT);  // Set reverse relay pin as output
}

void loop() {

  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);

  // Read rainfall sensor
  int rainfall_val = analogRead(rainfall_pin);  // Read rainfall sensor

  // Print temperature and rainfall readings
  Serial.print(Celsius);
  Serial.print(" C  \n");
  Serial.print("Rainfall: ");
  Serial.println(rainfall_val);
  delay(1000);
  
  digitalWrite(forward_relay_pin, HIGH);
  digitalWrite(reverse_relay_pin, HIGH);

  // Check conditions for forward direction
  if (rainfall_val < 100 || Celsius > 35) {
    Serial.println(" EXTREME WEATHER!! SHED COVERS");
    if (!forwardConditionSatisfied) {
      // Activate forward relay
      Serial.println("Forward Relay ON");
      digitalWrite(forward_relay_pin, LOW);   // Turn ON forward relay
      delay(2800);  // Keep relay ON for 2 seconds
      forwardConditionSatisfied = true;
      
    }
  } else {
    // Deactivate forward relay
    if (forwardConditionSatisfied) {
      Serial.println("Forward Relay OFF");
      digitalWrite(forward_relay_pin, HIGH);   // Turn OFF forward relay
      forwardConditionSatisfied = false;
    }
  }

  // Check conditions for reverse direction
  if (rainfall_val > 100 && Celsius < 35) {
    Serial.println("SHED OPENS");
    if (!reverseConditionSatisfied) {
      // Activate reverse relay
      Serial.println("Reverse Relay ON");
      digitalWrite(reverse_relay_pin, LOW);   // Turn ON reverse relay
      delay(2800);  // Keep relay ON for 2 seconds
      reverseConditionSatisfied = true;
      
    }
  } else {
    // Deactivate reverse relay
    if (reverseConditionSatisfied) {
      Serial.println("Reverse Relay OFF");
      digitalWrite(reverse_relay_pin, HIGH);   // Turn OFF reverse relay
      reverseConditionSatisfied = false;
    }
  }

  delay(1000);  // Delay before next iteration
}
