void doorStatus() {
  closed = digitalRead(SWITCH_PIN);
  Serial.print("Door closed: ");
  Serial.println(closed);
  delay(100);
}

void closeDoor() {
  Serial.println( "Closing Door");
  delay(3000);
  locked = true;
}

void openDoor() {
  Serial.println( "Opening Door");
  delay(3000);
  locked = false;
}
