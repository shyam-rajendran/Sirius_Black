const int pyro1_con = 41;  // Status input
const int pyro2_con = 42;  // Status input
const int pyro1_en  = 48;  // Fire output
const int pyro2_en  = 47;  // Fire output

void setup() {
  Serial.begin(115200);
  
  pinMode(pyro1_con, INPUT);
  pinMode(pyro2_con, INPUT);
  pinMode(pyro1_en, OUTPUT);
  pinMode(pyro2_en, OUTPUT);
  
  digitalWrite(pyro1_en, LOW);
  digitalWrite(pyro2_en, LOW);

  Serial.println("System Ready.");
  Serial.println("Send 'Y' to fire Ematch1, 'U' to fire Ematch2");
  Serial.println();
}

void loop() {
  // Show status of both ematches
  int EM1 = digitalRead(pyro1_con);
  int EM2 = digitalRead(pyro2_con);
  
  Serial.print("Ematch1: ");
  Serial.print(EM1 == HIGH ? "CONNECTED" : "DISCONNECTED");
  Serial.print(" | Ematch2: ");
  Serial.println(EM2 == HIGH ? "CONNECTED" : "DISCONNECTED");

  // Check for serial input
  if (Serial.available()) {
    char input = Serial.read();

    if (input == 'Y' || input == 'y') {
      Serial.println("🔥 Firing Ematch1!");
      digitalWrite(pyro1_en, HIGH);
      delay(3000);
      digitalWrite(pyro1_en, LOW);
    }

    if (input == 'U' || input == 'u') {
      Serial.println("🔥 Firing Ematch2!");
      digitalWrite(pyro2_en, HIGH);
      delay(3000);
      digitalWrite(pyro2_en, LOW);
    }
  }

  delay(500); // Check status every 0.5 sec
}
