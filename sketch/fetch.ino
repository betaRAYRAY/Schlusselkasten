

//Get File from Server
void fetchTeri() {
  state = 2;
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    WiFiClientSecure httpsClient;    //Declare object of class WiFiClient

    httpsClient.setFingerprint(fingerprint);
    httpsClient.setTimeout(15000); // 15 Seconds
    delay(1000);

    Serial.print("HTTPS Connecting");
    int r = 0; //retry counter
    while ((!httpsClient.connect(host, port)) && (r < 30)) {
      delay(100);
      Serial.print(".");
      r++;
    }
    if (r == 30) {
      Serial.println("Connection failed");
    }
    else {
      Serial.println("Connected to web");
    }

    Serial.print("requesting URL: ");
    Serial.println(String(host) + String(param));

    httpsClient.print(String("GET ") + param + " HTTP/1.1\r\n" +
                      "Host: " + host + "\r\n" +
                      "Connection: close\r\n\r\n");

    Serial.println("request sent");

    while (httpsClient.connected()) {
      String line = httpsClient.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    String tmp;
    int counter = 0;
    int len = -1;
    while (httpsClient.available()) {
      tmp = httpsClient.readStringUntil('\n');  //Read Line by Line
      if (counter == 1) {
        len = tmp.toInt();
        Serial.println("Length: " + String(len));
      }
      else if (counter-2 < len) {
        allowedUID[counter-2] = tmp;
      }
      counter++;

    }
    Serial.println("closing connection");
  }
  state = 3;
}
