#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#include <splash.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 1     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int port = 20080;

#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
byte mac[] = {
  0xA8, 0x61, 0x0A, 0xAE, 0x95, 0x5F };
IPAddress ip(192, 168, 2, 177);

// Running on Port int port, select your port!
EthernetServer server(port);

bool alreadyConnected = false;
// buffer for incoming commands
String command = "";

float hum;  //Stores humidity value
float temp; //Stores temperature value
void setup() {


  dht.begin();
  // set pin 7 to output
  pinMode(7, OUTPUT);

  // initialize the Ethernet device
  Ethernet.init(10);
  Ethernet.begin(mac, ip);

  // Open serial communications for debugging
  Serial.begin(9600);

  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }


  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.print(Ethernet.localIP());

  display.setCursor(0,10);
  display.print("Port: "+ String(port));
  display.display();
  // start listening for clients
  server.begin();

  Serial.print("IoT server address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  display.display(); 
  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("App connected");
      alreadyConnected = true;

      display.clearDisplay();

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);

      display.print("Connected!");

    }
  
    if (client.available() > 0) {
      
      // read the bytes incoming from the client:
      char thisChar = client.read();
      command = String(command + thisChar);
      // echo received the bytes to the serial port
      Serial.write(thisChar);
      if (thisChar == '>')
      {
        Serial.println("");
        String respons = reply(command);
        Serial.println("Responding: " + respons);
        client.println(respons);
        command = "";
      }
      if(thisChar == '$')
      {
        
        display.clearDisplay();
        Serial.println("");
        String respons = reply(command);
        Serial.println("Responding: " + respons);
        client.println(respons);
        command = "";
      }
      if(thisChar == '&')
      {
        
        Serial.println("");
        String respons = reply(command);
        Serial.println("Responding: " + respons);
        client.println(respons);
        command = "";
      }
    }
  }
}

String reply(String cmd)
{
  Serial.println("Received command: " + cmd);

  if (cmd == "<ECHO>")
  {
    Serial.println("Echo command recognized");
    return "<ECHO>";
  }

  
    
  if (cmd == "Humd&")
  {
    Serial.println("Reading Humidity");  
    return String(hum);
  }

  
  if (cmd == "Temp&")
  {
    // Serial.println("Reading Temperature"); 
    // temp = dht.readTemperature();
    // // hum = dht.readHumidity();
    // return String("Temp: "+String(temp) + "C Humd: "+String(hum) +"%");
    return String(analogRead(A1));
  }


  
  if (cmd[cmd.length()-1] == '$')
  {
    cmd.remove(cmd.length()-1);
    Serial.println("<OK>");
    Serial.println(cmd);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    // Display static text

    display.print(cmd);
    display.display();
    return "<OK>";
  }

}
