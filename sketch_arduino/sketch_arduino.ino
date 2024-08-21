const int OUTER_SENSOR_PIN = 3; // First when you enter.
const int INNER_SENSOR_PIN = 5; // First when you leave.

int lastOuterSensorState = 0;
int lastInnerSensorState = 0;

int peopleInCount = 0;

int SENSOR_IS_ACTIVE = LOW; // It has random behavior... HIGH when not active, and viceversa.
int SENSOR_IS_INACTIVE = HIGH;

void setup() {
  pinMode(OUTER_SENSOR_PIN, INPUT);
  pinMode(INNER_SENSOR_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("{\"action\":\"START\"}");
}

void printAction(bool personEntered, int peopleCount) {
  if (personEntered) {
    Serial.println("{\"action\":\"IN\", \"count\":" + String(peopleInCount) + "}");
  } else {
    Serial.println("{\"action\":\"OUT\", \"count\":" + String(peopleInCount) + "}");
  }
}

bool didPersonGoThrough(int firstSensorPin, int secondSensorPin) {
  int firstSensorState = digitalRead(firstSensorPin);
  int secondSensorState = digitalRead(secondSensorPin);
  int firstSensorStateAfterDelay = 0;
  int secondSensorStateAfterDelay = 0;
  bool beginningToPass = firstSensorState == SENSOR_IS_ACTIVE && secondSensorState == SENSOR_IS_INACTIVE;

  if (!beginningToPass) {
    return false;
  }

  // Wait to avoid false positives.
  delay(100);
  secondSensorStateAfterDelay = digitalRead(secondSensorPin);

  if (secondSensorStateAfterDelay == SENSOR_IS_INACTIVE) {
    return false;
  }

  // Wait until person passes completely
  while(secondSensorStateAfterDelay == SENSOR_IS_ACTIVE) {
    secondSensorStateAfterDelay = digitalRead(secondSensorPin);
  }

  return true;
}

void loop() {
  bool didPersonEnter = didPersonGoThrough(OUTER_SENSOR_PIN, INNER_SENSOR_PIN);  

  if (didPersonEnter) {
    peopleInCount += 1;
    printAction(true, peopleInCount);
  }

  bool didPersonExit = didPersonGoThrough(INNER_SENSOR_PIN, OUTER_SENSOR_PIN);

  if (didPersonExit) {
    peopleInCount -= 1;
    printAction(false, peopleInCount);
  }
}

