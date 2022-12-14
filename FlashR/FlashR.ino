#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#include <splash.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN  2    // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//Sets the temp sensor
DHT dht(DHTPIN, DHTTYPE);


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//If you want to use the display you can use these commands:
//
//
//  display.clearDisplay(); This clears the display.
//  display.setTextSize(1); This sets the text size. Text can be quite big so have fun.
//  display.setTextColor(WHITE); Our display has no colour settings so this doesnt matter much past initialization.
//  display.setCursor(a, b); This sets the position of the cursor. a is for right to left and b is for up and down. Use numbers ofc.
//  display.print("text"); This will put text on the spot you set the cursor.
//  display.display(); This will make it display the text.



//Input your desired port here.
int port = 20080;

#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
byte mac[] = {
  0xA8, 0x61, 0x0A, 0xAE, 0x95, 0x5F };
IPAddress ip(192, 168, 2, 177);

// Running on Port int port.
EthernetServer server(port);

bool alreadyConnected = false;
String command = ""; // buffer for incoming commands
bool delayRunning = false; //This can stop/start the Timer.
int delayStart; //Stores the start time of the Timer.
float hum;  //Stores humidity value
float temp; //Stores temperature value
void setup() {

  //Initializes the temp sensor
  dht.begin();
  
  //Pulls data from temp sensor.
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  //Initializes timer.
  delayRunning = true;
  delayStart = millis();

  
  // set pin 7 to output
  pinMode(7, OUTPUT);

  // initialize the Ethernet device
  Ethernet.init(10);
  Ethernet.begin(mac, ip);

  // Open serial communications for debugging
  Serial.begin(9600);

  //This only applies to your display. If there is no display connected, feel free to comment out the following lines:
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

  //Initializes display.
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  //Displays IP on line 1
  display.print(Ethernet.localIP());
  //Displays port on line 2
  display.setCursor(0,10);
  display.print("Port: "+ String(port));
  display.display();

  
  // start listening for clients
  server.begin();
  //This prints the IP in serial monitor.
  Serial.print("IoT server address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  //this pulls the data from the temp/hum sensor every 5 seconds (5000mili)
  //If you want anything based on a timer copy this and adjust what you need.
  if (delayRunning && ((millis() - delayStart) >= 5000)) {
      
      temp = dht.readTemperature();
      hum = dht.readHumidity();
      delayStart = millis();
  }



  //This makes it so the display displays.
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

      //This goes through every byte send. It will go through each character seperatly
      
      
      // echo received the bytes to the serial port

      //If you want to add commands add their identifier in here.
      //If it will display something add display.clearDisplay();
      //You can copy the rest.

      //This is for the <ECHO> command.
      Serial.write(thisChar);
      if (thisChar == '>')
      {
        Serial.println("");
        String respons = reply(command);
        Serial.println("Responding: " + respons);
        client.println(respons);
        command = "";
      }

      //This is for the Message command
      if(thisChar == '$')
      {
        
        display.clearDisplay();
        Serial.println("");
        String respons = reply(command);
        Serial.println("Responding: " + respons);
        client.println(respons);
        command = "";
      }

      //This is for the Temp& command      
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


//This is the reply function. It reads the command and if it matches anything it will run that code.
//To put in your own command add an If statement with the indentifier. If you use identifiers at the end instead of hardcoding the entire command its cleaner.

String reply(String cmd)
{
  Serial.println("Received command: " + cmd);

  //This is the general connection command.
  if (cmd == "<ECHO>")
  {
    Serial.println("Echo command recognized");
    return "<ECHO>";
  }

  //This sends back the temperature.
  if (cmd == "Temp&")
  {
    Serial.println("Reading Temperature"); 
    return String("Temp: "+String(temp) + "C Humd: "+String(hum) +"%");
  }

  //This is the text command. In this case the arduino recieves text and dislays it on the screen.
  //After that it sends back an <ok>  to signify its been recieved.
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
