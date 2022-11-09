// Motion sensor
int led = 13;
int sensor = 2;
int val = 0;

// Distance sensor
int echoPin = 8;
int trigPin = 7;
long distance;
int duration;

// Graphing helper vars
long upper = 150;
long lower = 0;

// Serial signal
char* signal = "";

// Options
bool graphMode = false;

void setup() {
  pinMode(sensor, INPUT);
  pinMode(led, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  Serial.begin(115200);
}

void loop(){
  // Perform the proximity sensor operation. Turn LED on if motion detected
  val = digitalRead(sensor);
  if (val == HIGH) {
    digitalWrite(led, HIGH);
    if (graphMode) { Serial.println("Presence: 10"); }
  }
  if (val == LOW) {
    digitalWrite(led, LOW);
    if (graphMode) { Serial.println("Presence: 5"); }
  }

  // Code grabbed from: https://create.arduino.cc/projecthub/csbenk/range-finder-using-ultrasonic-sensor-563598?ref=user&ref_id=55932&offset=0

  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // The pulseIn function times the signal return after bouncing off the object
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (wave goes and comes back)
  // Displays the distance on the Serial Monitor
  if (graphMode){
    Serial.println("Distance: " + String(distance) + " cm");
    Serial.println("Upper: " + String(upper));
    Serial.println("Lower: " + String(lower));
  }
  else {
    delay(1000);
    SignalBuilder();
  }
}

void SignalBuilder(){
  // Construct the serial signal
  if (val == HIGH){
    signal = "1 ";
  }
  else{
    signal = "0 ";
  }
  Serial.print(signal + String(distance));
}