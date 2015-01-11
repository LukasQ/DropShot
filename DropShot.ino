#define LED 13 //LED Pin for Status
#define FLASH 2 // Optocouppler PIN for the flash
#define SHUTTER 4 // Optocouppler Pin for the shutter
#define TRIGGER 0 // Lightbar receiver to start the rec
#define VALVE 6 // Valve Output Pin -> Transistor
#define SWITCH 8 // Trigger button to open the valve

#define DROPSIZE 90 // Valve open time
#define FLASHDELAY 180 // Delay between trigger and flash
//Usually t= sqrt((2h)/9,81)

// No interrupt neaded. Timing is @ 16Mhz fair accurate

int value = 0;
boolean done = false;

void setup() {
  //Pin modes
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH); //Show status
  pinMode(FLASH, OUTPUT);
  pinMode(SHUTTER, OUTPUT);
  pinMode(VALVE, OUTPUT);
  pinMode(TRIGGER, INPUT);
  pinMode(SWITCH, INPUT);

  Serial.begin(9600);  //Begin serial communcation -> Debug

  // collect sensor data
  for(int i = 0; i <= 10; i++){ 
    value += analogRead(TRIGGER);
    delay(100);
  }
  value = value/10; // average sensor data
  digitalWrite(LED, LOW); //End startup
}

void loop() {
  //Serial.println(analogRead(TRIGGER)); //Write the value of the photoresistor to the serial monitor.

  //Start the shooting
  if(digitalRead(SWITCH) && done == false){
    //noInterrupts();
    shoot();
    delay(100); // wait for mechanical camera parts 
    valve();
    done=true;
  }

  // If trigger -> make a photo
  if(analogRead(TRIGGER) > value){
    delay(FLASHDELAY);
    flash();
    delay(1000);
    done = false; // reenable
  }
}

//Photo trigger function
void shoot(){
  digitalWrite(SHUTTER, HIGH);
  delay(60);
  digitalWrite(SHUTTER, LOW);
  done = true;
  Serial.println("shoot!");

}

//Flash light function
void flash(){
  digitalWrite(FLASH, HIGH);
  delay(60);
  digitalWrite(FLASH, LOW);
  Serial.println("flash!");
}

//Valve open function
void valve(){
  digitalWrite(VALVE, HIGH);
  delay(DROPSIZE);
  digitalWrite(VALVE, LOW);
  done = true;
  Serial.println("value opende");
}
