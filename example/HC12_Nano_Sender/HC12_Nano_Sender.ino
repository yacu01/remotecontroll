
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //RX, TX


int buttonPin = 8;
int TRIGGER_COMMAND       = 1111;
int GARAGE_STATUS_COMMAND = 8888;

boolean IN_RANGE = false;       // out of range
boolean GARAGE_STATUS = false;  // closed


void setup() {
  pinMode(buttonPin, INPUT);
  mySerial.begin(9600);
  Serial.begin(115200);
  // prep GPIO13
  pinMode(13, OUTPUT);
  
}



boolean extendTime = false;
boolean motionDetected = false;
unsigned long last = millis();//set timer
boolean sendData = false;
int prevButtonState = 0; 
int lastSent = 1111;
boolean  buttonPressed  = false;

char SerialByteIn;                              // Temporary variable
char HC12ByteIn;                                // Temporary variable
String HC12ReadBuffer = "";                     // Read/Write Buffer 1 for HC12
String SerialReadBuffer = "";                   // Read/Write Buffer 2 for Serial
boolean SerialEnd = false;                      // Flag to indicate End of Serial String
boolean previousInRange = false;


void loop() {


 
    int buttonState = digitalRead(buttonPin);
    buttonPressed = false;
    if ( buttonState != prevButtonState ) {      
        if ( prevButtonState == 1 and buttonState == 0 ) {
            buttonPressed = true;
        }    
    }
    prevButtonState = buttonState;
  
    Serial.print("buttonState= ");Serial.println(buttonState);
    Serial.print("prevButtonState= ");Serial.println(prevButtonState); 
    Serial.print("sendData= ");Serial.println(sendData);
    if ( buttonPressed ){  
        mySerial.println(TRIGGER_COMMAND);//send unique code to the receiver in this case 1234
    }



    if (  mySerial.available() ) { 
       IN_RANGE = true;
       mySerial.println(GARAGE_STATUS_COMMAND);
          }else{
        IN_RANGE = false;       
    }      
    
    // Get for Garage Status          
    if ( mySerial.available() > 1){  
        int input = mySerial.parseInt();    
        Serial.print("cmd recevied: ");Serial.println(input);
        // get garage status    
        if ( input == 8001 )  {
              GARAGE_STATUS = true;
              Serial.println("Garage Status OPEN"); 
              IN_RANGE = true;
              previousInRange = true;
              
        }else if ( input == 9001 ) {
              GARAGE_STATUS = false;
              Serial.println("Garage Status CLOSED");

              if ( ! previousInRange ) {  
                  mySerial.println(TRIGGER_COMMAND);
                  Serial.println("Sent a trigger");
              }
              previousInRange = false;
        }
    }
    
    mySerial.flush();//clear the serial buffer for unwanted inputs     
    delay(500);//delay little for better serial communication
}


