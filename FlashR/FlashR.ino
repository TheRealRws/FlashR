#include <LiquidCrystal_I2C.h>

#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#include <splash.h>


#include <SPI.h>
#include <Wire.h>


#define DHTPIN  2    // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//Sets the temp sensor
DHT dht(DHTPIN, DHTTYPE);


// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.


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

bool alreadyConnected = false; //sets the default condition of the connection
String command = ""; // buffer for incoming commands
int delayStart; //Stores the start time of the Timer.
int delayStart2;
float hum;  //Stores humidity value
float temp; //Stores temperature value

//These are the preset messages.
String Messages[4]={"ok","no","give me 5 minutes", "Something"};
String respons = "";







void setup() {


  //Initializes the temp sensor
  dht.begin();
  
  //Pulls data from temp sensor.
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  //Initializes timers.
  delayStart = millis();
  delayStart2 = millis();
  
  // set pin 7 to output
  pinMode(7, OUTPUT);

  //Pins for the buttons.
  pinMode(8, INPUT); //mes1
  pinMode(9, INPUT); //mes2
  pinMode(10, INPUT); //mes3
  pinMode(11, INPUT); //Button

  // initialize the Ethernet device
  Ethernet.init(10);
  Ethernet.begin(mac, ip);

  // Open serial communications for debugging
  Serial.begin(9600);



  lcd.init();
  lcd.backlight();


  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) 
  {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    lcd.print ("Ethernet shield was not found.  Sorry, can't run without hardware. :(");  

             while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
lcd.print("  Ethernet cable not connected.");  
//This is the loop that makes the LCD text scroll to the right, after which text eventually becomes visible again and then on and on. 
  if (((millis() - delayStart2) >= 700)) {
            lcd.scrollDisplayLeft();
      delayStart2 = millis(); //initializes this timer

 }  
  }


if (Ethernet.linkStatus() != LinkOFF) {
    lcd.setCursor(2, 0); // Set the cursor on the third column and first row.
  lcd.print(Ethernet.localIP()); // Print the IP
  lcd.setCursor(2, 1); //Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print(port);//Print the port

}


  
  
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
  if (((millis() - delayStart) >= 5000)) {
      
      temp = dht.readTemperature();
      hum = dht.readHumidity();
      delayStart = millis();
  }



//TO DO: MES4(MEs1) Koppelen aan buttons: TUESDAY. LED TOEVOEGEN
// LED: Array aan ints aanmaken voor verschillende morse LED signalen. Elke keer als timer afgaat telt int op, wordt index van array.  
    


 // This sets the recieving message to the preset message matching the button.
  // if(!digitalRead(8))
  // {
  //     Serial.println("changed to 1");
  //   mes4 = mes1;
  // }
  // if(!digitalRead(9))
  // {
  //     Serial.println("changed to 2");
  //   mes4 = mes2;
  // }
  // if(!digitalRead(10))
  // {
  //     Serial.println("changed to 3");
  //   mes4 = mes3;
  // }

  // when the client sends the first byte, say hello:
  if (client) 
    {
    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("App connected");
      alreadyConnected = true;
    }
  
    if (client.available() > 0) 
      {
      
      // read the bytes incoming from the client:
      char thisChar = client.read();
      command = String(command + thisChar);


      //This is for the <ECHO> command.
      Serial.write(thisChar);
      if (thisChar == '>')
        {
          Serial.println("");
          respons = reply(command);
          Serial.println("Responding: " + respons);
          client.println(respons);
          respons = "";
          command = "";
        }

      if (thisChar == '$')
      {
        Serial.println("");
        respons = reply(command);
        Serial.println("Responding: " + respons);
        client.println(respons);
        respons = "";
        command = "";
      }

      //This is for the Message command
      if(thisChar == '#')
      {
        //display.clearDisplay();
        Serial.println("");
        respons = reply(command);
        Serial.println("Responding: " + respons);
        client.println(respons);
        respons = "";
        command = "";
      }

      //This is for the Temp& command.     
      if(thisChar == '&')
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
//To put in your own command add an If statement with the indentifier. If you use identifiers at the end instead of hardcoding the entire command its cleaner.

String reply(String cmd)
{
  Serial.println("Received command: " + cmd);

  //This is the general connection command.
  if (cmd == "<ECHO>")
  {
    Serial.println("Echo command recognized");
  lcd.clear();
  lcd.print("Echo received!");
  return "<ECHO>";

 }  

    

  

  //This sends back the temperature.
  if (cmd == "Temp&")
  {
    Serial.println("Reading Temperature"); 
    return String("Temp: "+String(temp) + "C Humd: "+String(hum) +"%");
 }  



  //This is both for updating the preset messages on the phone and for sending messages to the phone.
  //Mes4 is for the sending to the phone.
  if (cmd[cmd.length()-1] == '#' || cmd[cmd.length()-1] == '*')
  {
    String Temp="";
    
    if (cmd[cmd.length()-1] == '#')
    {
      Temp = Messages[cmd[cmd.length()-2]-'0'];
      Serial.println("Send message: "+Temp); 
      return String( Temp +'#');
    }

    if (cmd[cmd.length()-1] == '*' && cmd[cmd.length()-2] != '4')
    {
      int index = cmd[cmd.length()-2]-'0';
      
      Serial.println("Changing Mes" +String(index)+" to "+cmd);
      cmd.remove(cmd.length()-1);
      cmd.remove(cmd.length()-2);

      
      Messages[index] = cmd;
      return "<OK>";
      
    }
  }


  //This is the text command. In this case the arduino recieves text and dislays it on the screen.
  //After that it sends back an <ok>  to signify its been recieved.
  if (cmd[cmd.length()-1] == '$')
  {
    cmd.remove(cmd.length()-1);
    Serial.println("<OK>");

    //!!!!!!!!!!!!!!!!!Display the cmd here on display.!!!!!!!!!!!!!!!!!!!!!

    return "<OK>";
  }


}
