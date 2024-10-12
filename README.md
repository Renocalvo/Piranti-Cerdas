# Piranti Cerdas

Smart devices are advanced tools equipped with sophisticated technology that allows them to connect and interact with users and their environment. In the context of learning, smart devices include various types of gadgets, such as smartphones, tablets, and Internet of Things (IoT) devices.

---

**This repo contains my code for my task in the Piranti Cerdas class.**

## Preview
*Illustration* <br>
<img src="https://www.ezlopi.com/wp-content/uploads/2024/03/automated-led-lamp-control-using-ldr-digital-sensor-wiring-diagram.webp" width="400">

## Utilities
### Tools Required

| Tool   | Description          |
|--------|----------------------|
| ESP32  | Microcontroller       |
| LED    | Light Emitting Diode  |
| Resistor | Current Limiting Device |
| LDR    | Light Dependent Resistor |


### Example Code Snippet

```cpp
int ledPin = 12;  // Define the LED pin

void setup() {
    pinMode(ledPin, OUTPUT);  // Set LED pin as output
}

void loop() {
    digitalWrite(ledPin, HIGH);  // Turn on LED
    delay(1000);                 // Wait for a second
    digitalWrite(ledPin, LOW);   // Turn off LED
    delay(1000);                 // Wait for a second
}
