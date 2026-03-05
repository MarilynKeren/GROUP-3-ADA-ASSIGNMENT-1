#include <Arduino.h>

#define BUFFER_SIZE 64

// ======================================================
//          Generic (Templated) Circular Buffer
// ======================================================
template <typename T>
class CircularBuffer {
private:
    T buffer[BUFFER_SIZE];   // Fixed-size array storage
    int head;                // Index where next item will be written
    int tail;                // Index where next item will be read
    int count;               // Number of elements currently stored

public:
    // Constructor initializes everything to empty state
    CircularBuffer() : head(0), tail(0), count(0) {}

    // --------------------------------------------------
    // Add item to buffer (Producer)
    // --------------------------------------------------
    bool enqueue(T item) {

        // If buffer is full, cannot insert
        if (isFull()) {
            return false;
        }

        buffer[head] = item;                 // Store item
        head = (head + 1) % BUFFER_SIZE;     // Move head circularly
        count++;                             // Increase stored items

        return true;
    }

    // --------------------------------------------------
    // Remove item from buffer (Consumer)
    // --------------------------------------------------
    bool dequeue(T& item) {

        // If buffer empty, nothing to remove
        if (isEmpty()) {
            return false;
        }

        item = buffer[tail];                 // Get oldest item
        tail = (tail + 1) % BUFFER_SIZE;     // Move tail circularly
        count--;                             // Decrease stored items

        return true;
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == BUFFER_SIZE;
    }

    int size() {
        return count;
    }
};

// ======================================================
//              Global Objects
// ======================================================

CircularBuffer<int> sensorDataBuffer;

unsigned long lastProducerTime = 0;
unsigned long lastConsumerTime = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial);
    Serial.println("Starting Circular Buffer Demo...");
}

void loop() {

    unsigned long currentMillis = millis();

    // ==================================================
    // Producer (1000 Hz → every 1 ms)
    // ==================================================
    if (currentMillis - lastProducerTime >= 1) {

        lastProducerTime = currentMillis;

        int simulatedSensorValue = random(0, 1000);

        // Try to insert into buffer
        if (!sensorDataBuffer.enqueue(simulatedSensorValue)) {
            Serial.println("Buffer Overflow!");
        }
    }

    // ==================================================
    // Consumer (10 Hz → every 100 ms)
    // ==================================================
    if (currentMillis - lastConsumerTime >= 100) {

        lastConsumerTime = currentMillis;

        int processedValue = 0;
        int itemsProcessedThisCycle = 0;

        // Process ALL available data
        while (!sensorDataBuffer.isEmpty()) {

            if (sensorDataBuffer.dequeue(processedValue)) {

                Serial.print("Processed: ");
                Serial.println(processedValue);

                itemsProcessedThisCycle++;
            }
        }

        Serial.print("--- Processed ");
        Serial.print(itemsProcessedThisCycle);
        Serial.println(" items this cycle ---\n");
    }
}
/*
Summary:
This program demonstrates the use of a circular buffer to manage data between a fast producer
(high-speed sensor generating data every 1 ms) and a slower consumer (processing data every
100 ms). The circular buffer temporarily stores incoming sensor readings using a fixed-size
array with head and tail indices to manage insertion and removal. The enqueue function adds
data to the buffer while dequeue retrieves it for processing. If the buffer becomes full,
an overflow warning is printed. This approach prevents immediate data loss and allows the
system to handle different data production and processing speeds efficiently.
*/