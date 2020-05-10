//library button flash.
#include <EasyButton.h>
//library WifiManager
#include <ESP8266WiFi.h>         
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   
//library Json
#include <ArduinoJson.h>  
//library MQTT   
#include <PubSubClient.h>


//Variable
/// button flash
#define BUTTON_PIN 0
EasyButton button(BUTTON_PIN);

/// delay
#define D0 16   
#define LED D0  

/// flag for saving data
bool shouldSaveConfig = false;

/// define your default values here, if there are different values in config.json, they are overwritten.
#define mqtt_server       "www.km1.io"
#define mqtt_port         "1883"
#define mqtt_user         "toon"
#define mqtt_pass         "10205"
#define mqtt_topic        "/toon/room5"

WiFiClient espClient;
PubSubClient client(espClient);

//Function
/// button flash is pressed.
void onPressed() {
    //clean FS for testing 
    SPIFFS.format();
    
    //reset WifiManager 
    wifiManager.resetSettings();

    //reboot board
    ESP.restart();
}

/// callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

//System Startup
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  
  // Initialize the button.
  button.begin();
  
  // Add the callback function to be called when the button is pressed.
  button.onPressed(onPressed);
  
  //Swicts
  pinMode(LED,OUTPUT);
  
  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_user, json["mqtt_user"]);
          strcpy(mqtt_pass, json["mqtt_pass"]);
          strcpy(mqtt_topic, json["mqtt_topic"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read



  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqtt_user, 20);
  WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", mqtt_pass, 20);
  WiFiManagerParameter custom_mqtt_topic("topic", "mqtt topic", mqtt_topic, 40);
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;


  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //add all your parameters here
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_pass);
  wifiManager.addParameter(&custom_mqtt_topic);


  //if it does not connect it starts an access point with the specified name
  //here  "Smart_PlugIN"
  wifiManager.autoConnect("Smart_PlugIN", "vsalab2020")

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(mqtt_user, custom_mqtt_user.getValue());
  strcpy(mqtt_pass, custom_mqtt_pass.getValue());
  strcpy(mqtt_topic, custom_mqtt_topic.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["mqtt_user"] = mqtt_user;
    json["mqtt_pass"] = mqtt_pass;
    json["mqtt_topic"] = mqtt_topic;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  String inString = "";  
  inString += mqtt_port;
  const uint16_t mqtt_port_x = inString.toInt(); 
  client.setServer(mqtt_server, mqtt_port_x);
  client.setCallback(callback);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// on/off delay
void callback(char* topic, byte* payload, unsigned int length) {
 String msg = "";
 int i=0;
 while (i<length) msg += (char)payload[i++];
 if (msg == "on" or msg == "ON" or msg == "1") {
   client.publish(mqtt_topic,"Switch-ON"); 
   digitalWrite(LED,HIGH);
   return;
 }
 else if(msg == "off" or msg == "OFF" or msg == "0"){
   client.publish(mqtt_topic,"Switch-OFF"); 
   digitalWrite(LED,LOW);
   return;
 }
}

//System Running
void loop() {
  // Continuously read the status of the button. 
  button.read();

  // put your main code here, to run repeatedly:
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
