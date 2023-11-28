#define BUFFER_SIZE 200

char ssid[32] = "";
char password[32] = "";
char host[32] = "";
char port[32] = "8001";
char device_name[32] = "";

char inputBuffer[BUFFER_SIZE];
WiFiClient client;

int connectToHost() {
  if (connectToWifi()) {
    connStatus(CONN_SERVER_CONNECTING);
    Serial.println(String("connecting to ") + host + ":" + port);
    if (!client.connect(host, String(port).toInt())) {
      connStatus(CONN_SERVER_FAIL);
      Serial.println("Connection failed");
      return false;
    } else {
      connStatus(CONN_SERVER_CONNECTED);
      Serial.println("Connected");
      return sendLogin();
    }    
  }
  return false;
}

int sendLogin() {
  StaticJsonDocument<BUFFER_SIZE> login;
  login["device"] = String(device_name);
  send(login);
  return true;
}

boolean isConnectedToServer() {
  return client.connected();
}

int connectToWifi() {  
  connStatus(CONN_WIFI_CONNECTING);
  Serial.println(String("Connecting to ") + ssid);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int count = 0;
  while (WiFi.status() != WL_CONNECTED && (count++) < 300) {
    delay(100);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    connStatus(CONN_WIFI_FAIL);
    Serial.println();
    Serial.println("WiFi failed to connect.");
    return false;
  } else {
    connStatus(CONN_WIFI_CONNECTED);
    Serial.println();
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());         
    return true;
  }
}


void readJsonLine() {
  int count = 0;
  char c;
  do {
    c = readByte();
    inputBuffer[count] = c;
    count++;
  } while (c != '\n' && count < BUFFER_SIZE - 2);
  inputBuffer[count] = 0;
}

void readInput() {
  if (client.connected()) {    
    if (client.available() > 0) {    
      StaticJsonDocument<BUFFER_SIZE> doc;
      readJsonLine();
      DeserializationError error = deserializeJson(doc, inputBuffer);
      if (error) {
        Serial.println("JSON deserialization error");
        return;
      }
      
      Serial.print("Received "); serializeJson(doc, Serial); Serial.println();
      
      String type = doc["type"];
      if (type == "brightness")
      {
        int rawValue = doc["value"];
        setTargetBrightness(map(rawValue, 0, 255, 0, 255));
        setRelayTargetState(rawValue > 0 ? 1 : 0);
      } else {
        Serial.println("Ignoring message");
      }
    }
  }
}

int sendMeasurement(String type, float value) {
  Serial.print("Sending "); Serial.print(type); Serial.print("="); Serial.println(value);
  StaticJsonDocument<BUFFER_SIZE> doc;
  doc["type"] = type;
  doc["value"] = value;
  send(doc); 
  return 1; 
}

void send(StaticJsonDocument<BUFFER_SIZE> message) {
  serializeJson(message, client);
  client.println();
  client.flush();
  Serial.print("Sent "); serializeJson(message, Serial); Serial.println();  
}

byte readByte() {
  int count = 0;
  while(client.available() < 1 && (count++) < 1000) {
    delay(1);
  }
  return client.read();
}

void sendPing() {
  StaticJsonDocument<BUFFER_SIZE> doc;
  doc["ping"] = "ping";    
  send(doc);  
}
