int buzzerPin = 4;
int buttonPin = 26;
int ledPin = 12;


void setup()
{
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600); 
}


void loop()
{
  int buttonState = digitalRead(buttonPin);
  
  // Jika tombol ditekan (LOW karena pull-up)
  if (buttonState == LOW) 
  {
    digitalWrite(ledPin, HIGH);  // Nyalakan LED
    digitalWrite(buzzerPin, HIGH); // Nyalakan buzzer
    Serial.println("Digital value : 1");
  } 
  else 
  {
    digitalWrite(ledPin, LOW);   // Matikan LED
    digitalWrite(buzzerPin, LOW); // Matikan buzzer
    Serial.println("Digital value : 0");
  }

  delay(500);
}
