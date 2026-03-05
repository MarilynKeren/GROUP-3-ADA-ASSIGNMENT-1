#include <Arduino.h>

// ================= SENSOR DATA STRUCT =================
struct SensorData {
  float temperature;
  float humidity;
  int light;
};

// ================= GLOBAL SENSOR DATA =================
SensorData sensor;  // Single global struct to avoid dynamic allocation

// ================= SENSOR READING FUNCTION =================
void readSensors(SensorData &data) {
  data.temperature = random(200, 300) / 10.0; // 20.0°C to 30.0°C
  data.humidity = random(300, 600) / 10.0;    // 30% to 60%
  data.light = random(0, 1024);               // 0 to 1023
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Memory Leak Prevention Demo");
}

// ================= LOOP =================
void loop() {
  // Fill global struct with new sensor readings
  readSensors(sensor);

  // Print sensor data + free heap
  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.print(" | Temp: ");
  Serial.print(sensor.temperature);
  Serial.print("°C, Hum: ");
  Serial.print(sensor.humidity);
  Serial.print("%, Light: ");
  Serial.println(sensor.light);

  delay(1000); // simulate 1 second sampling
}
/*The program crashes because memory is allocated using new
inside the loop but never freed. 
This causes the free heap memory (ESP.getFreeHeap()) to decrease continuously. 
The issue is fixed by adding delete sensor; after using the data. 
However, using new and delete repeatedly is not ideal for embedded systems. 
The best solution is to use a single stack-based SensorData variable instead of dynamic allocation.
*/