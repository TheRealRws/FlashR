#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <Ethernet.h>
#include <splash.h>
#include <SPI.h>
#include <Wire.h>


#define DHTPIN  2    //What pin we're connected to.
#define DHTTYPE DHT22   //DHT 22  (AM2302)
//Sets the temperature sensor
DHT dht(DHTPIN, DHTTYPE);


//Wiring: SDA pin is connected to A4 and SCL pin to A5.
//Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered).
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.


//Input your desired port here.
int port = 20080;

//Enter a MAC address and IP address for your controller below.
//The IP address will be dependent on your local network.
byte mac[] = {
  0xA8, 0x61, 0x0A, 0xAE, 0x95, 0x5F };
IPAddress ip(192, 168, 2, 177);

//Running on Port int port.
EthernetServer server(port);

bool alreadyConnected = false; //Sets the default condition of the connection.
bool ledOn = false; //The bool that toggles the LED on and off.
String command = ""; //Buffer for incoming commands.

long delayStart; //Stores the start time of the Timer.
long delayStart3; //Stores the start time of another Timer.
long delayStart4; //Stores the start time of another Timer.

int messageLedSignal[25]={1500, 1500, 1500, 1500, 1500, 1500, 500, 1500, 500, 1500, 500, 1500, 500, 1500, 1500, 1500, 500, 1500, 1500, 1500, 1500, 1500, 1500, 1500}; //Array for the signal for LED, reacting on a message.
int ledCounter = 0;

float hum;  //Stores humidity value.
float temp; //Stores temperature value.

//These are the preset messages.
String Messages[4]={"ok","no","give me 5 minutes", " "};
String respons = "";
//The variables for us to manage time.
long time = 0;
int timeAmp = 0;
//Scroll function variables.
bool scrollAllow = false;
int scrollCounter = 0;
int scrollAmount= 0;




void setup() {


  //Initializes the temperature sensor.
  dht.begin();
  
  //Pulls data from temperature sensor.
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  //Initializes timers.
  delayStart = millis();
  delayStart3 = millis();
  delayStart4 = millis();
  
  //Set pin 3 to output for notification LED.
  pinMode(3, OUTPUT);

  //Pins for the buttons.
  pinMode(8, INPUT); //mes1
  pinMode(9, INPUT); //mes2
  pinMode(6, INPUT); //mes3

  //Initialize the Ethernet device.
  Ethernet.init(10);
  Ethernet.begin(mac, ip);

  //Open serial communications for debugging.
  Serial.begin(9600);

  //Initializes the lcd-display. 
  lcd.init();
  lcd.backlight();


  //Check if the Ethernet hardware is present.
  if (Ethernet.hardwareStatus() == EthernetNoHardware) 
  {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    lcd.print ("Ethernet shield was not found.  Sorry, can't run without hardware. :(");  
    while (true) 
    {
      delay(1); //Do nothing, there is no pointin running without Ethernet hardware.
    }
  }

  //Check for an Ethernet Cable.
  if (Ethernet.linkStatus() == LinkOFF) 
  {
    Serial.println("Ethernet cable is not connected.");
    lcd.print(" Ethernet cable ");
    lcd.setCursor(0, 1)  ;
    lcd.print(" not connected.");
    while (true) 
    {    
      delay(1); //Do nothing, there is no point in running without an Ethernet cable.
    }  
  }


  if (Ethernet.linkStatus() != LinkOFF) 
  {
    lcd.setCursor(0, 0); //Set the cursor on the third column and first row.
    lcd.print(Ethernet.localIP()); //Print the IP-address. 
    lcd.setCursor(0, 1); //Set the cursor on the third column and the second row (counting starts at 0!).
    lcd.print(port);//Print the port.
  }

  //Start listening for clients.
  server.begin();

  //This prints the IP-address in the serial monitor.
  Serial.print("IoT server address: ");
  Serial.println(Ethernet.localIP());
}


void loop() {

  //This makes it so we don't get interger overflows with the timers if millis goes over 2147483647.
  //If millis is higher than the long storage we add 1 to the time amp which will remove 2147483647 from millis. 
  //Time basically resets every 2147483647 ticks.
  if (time > 2147483620)
  {
    timeAmp++;
  }
  time = (millis()- (timeAmp * 2147483647));

  //Wait for a new client:
  EthernetClient client = server.available();

  //This pulls the data from the temperature/humidity sensor every 5 seconds (5000 milliseconds).
  if (((time - delayStart) >= 5000)) {
      
      temp = dht.readTemperature();
      hum = dht.readHumidity();
      delayStart = time;
  }

  //This allows the display to scroll.
  //To make the display scroll adjust:
  //scrollAllow to True
  //scrollAmount to the .lenght() of the string.
  //delayStart4 to time;
  //This will then make the text go back and forth only ONCE.

  //scrollCounter is the amount of times we have scrolled.
  if ((time - delayStart4) >= 500 && scrollAllow) 
  {
      
      if(scrollCounter >= scrollAmount/2)
      {
        lcd.scrollDisplayRight();
      }
      else if(scrollAllow < scrollAmount/2)
      {
        lcd.scrollDisplayLeft();
      }

      scrollCounter++;
      delayStart4 = time;

      if(scrollCounter >= scrollAmount)
      {
        scrollAllow = false;
        scrollCounter = 0;
        scrollAmount = 0;
      }
  }

  //This is the loop that toggles the LED to do the specific secret Morse signal whenever the cmd is triggered. 
  if ((time - delayStart3) >= (messageLedSignal[ledCounter]/2) && ledOn) 
  {
    ledCounter++;
    if (ledCounter >= 25)
    {
        ledOn = false;
        ledCounter = 0;
    }

    delayStart3 = time; //Initializes this timer.
    digitalWrite(3, !digitalRead(3));
  }

  //Turns off the LED if we dont run the loop.
  if (ledOn == false)
  {
    digitalWrite(3, LOW);
  }
    


  //This sets the receiving message to the preset message, matching the button and displays the change on the display.
  if(digitalRead(8) > 0)
  {
      lcd.clear();
      Serial.println("Changed message:" + Messages[0]);
      lcd.print("Changed message:");  
      lcd.setCursor(0, 1);
      lcd.print(Messages[0]);
      Messages[3] = Messages[0];  
  }

  if(digitalRead(9) > 0)
  {
    lcd.clear();
    Serial.println("Changed message:" + Messages[1]);
    lcd.print("Changed message:");
    lcd.setCursor(0, 1);
    lcd.print(Messages[1]);
    Messages[3] = Messages[1];
  }

  if(digitalRead(6))
  {
    lcd.clear();    
    Serial.println("Changed message:" + Messages[2]);
    lcd.print("Changed message:");
    lcd.setCursor(0, 1);
    lcd.print(Messages[2]);
    Messages[3] = Messages[2];
  }

  //When the client sends the first byte, say hello:
  if (client) 
    {
    //This makes it so on the first time you connect we display that as feedback on the display.
    if (!alreadyConnected) {
      // Clear out the input buffer:
      client.flush();
      Serial.println("App connected");
      lcd.clear();
      lcd.print("App connected!");
      alreadyConnected = true;
    }
  
    if (client.available() > 0) 
      {
      
      //Read the bytes incoming from the client:
      char thisChar = client.read();
      command = String(command + thisChar);

      //This is for all the commands.
      //If any of the indentifiers match with the byte recieved by the arduino we set the response to output string of reply. 
      if(thisChar == '#' || thisChar == '*' || thisChar == '&'|| thisChar == '$'|| thisChar == '>')
      {
        Serial.println("");
        respons = reply(command);
        Serial.println("Responding: " + respons);
        client.println(respons);
        respons = "";
        command = "";
      }
    }
  }
}

//This is the reply function. It reads the command and if it matches anything it will run that code.
//To put in your own command add an If-statement with the command/identifier.

String reply(String cmd)
{
  //We print the received command.
  Serial.println("Received command: " + cmd);

  //This is the general connection command.
  if (cmd == "<ECHO>")
  {
    Serial.println("Echo command recognized");
    return "<ECHO>";
  }   

  //This sends back the temperature + humidity in a formatted string.
  if (cmd == "Temp&")
  {
    Serial.println("Reading Temperature"); 
    return String("Temp: "+String(temp) + "C Humd: "+String(hum) +"%");
  }  
 
  //This is both for updating the preset messages on the phone and for sending messages to the phone.
  //Messages[3] is for the sending to the phone.
  //The # indentifier is for sending the message to the phone.
  //The * indentifier is for changing a message on the arduino.
  if (cmd[cmd.length()-1] == '#' || cmd[cmd.length()-1] == '*')
  {
    String Temp="";
    
    if (cmd[cmd.length()-1] == '#')
    {
      int index = (cmd[cmd.length()-2]-'0');
      Temp = Messages[index - 1];
      Serial.println("Send message: "+Temp); 
      return String( Temp +'#');
    }

    if (cmd[cmd.length()-1] == '*' && cmd[cmd.length()-2] != '4')
    {
      //We store the index so we can use it to change the text after modifying the cmd string.
      int index = (cmd[cmd.length()-2]-'0');
      
      Serial.println("Changing Mes" +String(index)+" to "+cmd);
      //We remove the last 2 chars from the string so it's clean and then we set it in the correct index of Messages.
      cmd.remove(cmd.length()-2);
      Messages[index-1] = cmd;
      return "<OK>";
      
    }
  }

  //This is the text command. In this case the arduino receives text and dislays it on the screen.
  //After that it sends back an <OK> to signify its been received.
  if (cmd[cmd.length()-1] == '$')
  {
    //We remove the $ from the string here.
    cmd.remove(cmd.length()-1);
    Serial.println("<OK>" +  String(cmd.length()));

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Recieved message:");
    lcd.setCursor(0, 1);
    lcd.print(cmd);
    //We turn on the notification LED.
    ledOn = true;

    //If the message is longer than what the display can show, we scroll.
    if(cmd.length()>10)
    {
      scrollAmount = cmd.length();
      delayStart4 = time;
      scrollAllow = true;
    }
    return "<OK>";
  }

}
