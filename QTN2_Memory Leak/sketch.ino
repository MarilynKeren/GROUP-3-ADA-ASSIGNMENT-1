/*Original code with the leak fixed
#include <Arduino.h>

struct SensorData {
  float temperature;
  float humidity;
  int light;
};

// Step 1: Identify and fix the memory leak in this function.
// Note: In Step 1, we still return a pointer, but we must ensure it is managed later. [cite: 207, 210]
SensorData* readSensors() {
  // Memory is allocated on the heap using 'new'. 
  // This memory stays occupied until 'delete' is called. [cite: 208]
  SensorData* data = new SensorData; 
  data->temperature = random(200, 300) / 10.0;
  data->humidity = random(300, 600) / 10.0;
  data->light = random(0, 1024);
  return data;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
}

void loop() {
  // We receive a pointer to heap memory. [cite: 223, 245]
  SensorData* sensor = readSensors();

  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap()); // Monitoring this helps verify the fix. 
  Serial.print(" | Temp: ");
  Serial.print(sensor->temperature);
  Serial.print(", Hum: ");
  Serial.print(sensor->humidity);
  Serial.print(", Light: ");
  Serial.println(sensor->light);

  
  // After using the data, we must explicitly free the memory. 
  // Without this line, the 'Free Heap' will decrease every second until crash. 
  delete sensor; 
  

  delay(1000);
}
*/
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
/*Analysis
Problem: The original program crashes because readSensors() allocates memory using new in the loop but never releases it.

Symptom: This creates a memory leak, causing ESP.getFreeHeap() to decrease until the ESP32 runs out of RAM.

Primary Fix: The leak is stopped by adding delete sensor; after the data is used to free the heap memory.

Design Flaw: Even when fixed, repeated new and delete operations are inappropriate for long-running embedded systems.

Fragmentation: Constant allocation/deallocation creates "holes" in memory, known as heap fragmentation, which eventually causes system failure.

Overhead: Searching the heap for free space adds CPU overhead and makes function execution time unpredictable (non-deterministic).

Robust Solution: The best approach is using a single stack-based or global SensorData variable to avoid the heap entirely.

Efficiency: By passing this variable by reference (&data), the program reuses the same memory address indefinitely.

Stability: This "zero-allocation" strategy ensures 100% predictable memory usage and prevents fragmentation-related crashes.

Conclusion: For microcontrollers, static allocation is safer and more performant than dynamic memory management.
*/