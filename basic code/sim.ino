// Define Hardware Serial 2 for the SIM7670C module (RX = 16, TX = 17)
HardwareSerial sim7670(2);

const int ledPin = 4;    // D4 pin for LED
const int buzzerPin = 2; // D2 pin for Buzzer

void setup() {
  // Start PC Serial Monitor
  Serial.begin(115200);
  delay(1000);

  // Start communication with SIM7670C
  sim7670.begin(115200, SERIAL_8N1, 16, 17);

  // Configure output pins
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);

  Serial.println("Initializing SIM7670C...");
  delay(3000);

  // Set SMS to text mode
  sim7670.println("AT+CMGF=1");
  delay(500);
  
  // Route incoming SMS text directly to serial output
  sim7670.println("AT+CNMI=2,2,0,0,0");
  delay(500);

  Serial.println("Ready! Waiting for calls or SMS...");
}

void loop() {
  // Check if data is received from the SIM7670C module
  if (sim7670.available() > 0) {
    String response = sim7670.readString();
    
    // Print raw response to Serial Monitor for debugging
    Serial.print(response);

    // Detect an incoming call
    if (response.indexOf("RING") != -1) {
      Serial.println("-> Incoming call detected! Alerting via Buzzer and LED...");
      incomingCallAlert();
    }

    // Convert response to lowercase to handle mixed/upper/lower case commands smoothly
    String lowerResponse = response;
    lowerResponse.toLowerCase();

    // SMS Command Handling
    if (lowerResponse.indexOf("buzzer on") != -1) {
      digitalWrite(buzzerPin, HIGH);
      Serial.println("-> SMS Command: Buzzer turned ON");
    } 
    else if (lowerResponse.indexOf("buzzer off") != -1) {
      digitalWrite(buzzerPin, LOW);
      Serial.println("-> SMS Command: Buzzer turned OFF");
    }

    if (lowerResponse.indexOf("led on") != -1) {
      digitalWrite(ledPin, HIGH);
      Serial.println("-> SMS Command: LED turned ON");
    } 
    else if (lowerResponse.indexOf("led off") != -1) {
      digitalWrite(ledPin, LOW);
      Serial.println("-> SMS Command: LED turned OFF");
    }
  }

  // Allow manual AT command typing from Serial Monitor
  if (Serial.available() > 0) {
    sim7670.write(Serial.read());
  }
}

void incomingCallAlert() {
  // Pattern: buzzer beeps and LED blinks
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  }
}