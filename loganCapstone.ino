//Libraries
#include <LCD_I2C.h>
#include <max6675.h>

// Buttons
int btnLEFT = 4;
int btnRIGHT = 3;
int btnSTART = 2;

// Menu setup
int menu = 1;

// Actuators
int R1 = 37;
int R2 = 36;

// LCD screen
LCD_I2C lcd(0x27, 16, 2);

// MAX6675 module pins
int thermoDO1 = 23, thermoCS1 = 9, thermoCLK1 = 10;
int thermoDO2 = 22, thermoCS2 = 11, thermoCLK2 = 12;
int thermoDO3 = 25, thermoCS3 = 5, thermoCLK3 = 6;
int thermoDO4 = 24, thermoCS4 = 7, thermoCLK4 = 8;

// Heater relay pins
int heater1 = 30, heater2 = 31, heater3 = 32, heater4 = 33;


// MAX6675 instances
 MAX6675 thermocouple1(thermoCLK1, thermoCS1, thermoDO1);
 MAX6675 thermocouple2(thermoCLK2, thermoCS2, thermoDO2);
 MAX6675 thermocouple3(thermoCLK3, thermoCS3, thermoDO3);
 MAX6675 thermocouple4(thermoCLK4, thermoCS4, thermoDO4);


//===========================================================================================//
//===========================================================================================//
//                                       VOID SETUP                                          //
//===========================================================================================//
//===========================================================================================//
void setup()   {
  //Setting up the actuators and ensuring default state
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);

   //Actuators need to be in same state at all times
   digitalWrite(R1, HIGH);
   digitalWrite(R2, HIGH);

pinMode(heater1 , OUTPUT);
pinMode(heater2 , OUTPUT);
pinMode(heater3 , OUTPUT);
pinMode(heater4 , OUTPUT);

digitalWrite(heater1, HIGH); // Turn heater off
digitalWrite(heater2, HIGH); // Turn heater off
digitalWrite(heater3, HIGH); // Turn heater off
digitalWrite(heater4, HIGH); // Turn heater off 


//screen initialization
  lcd.begin();
                 
  lcd.backlight();

 //===========================================================================================//
 //                                       START UP                                            //
 //===========================================================================================//
    //start up and test for devices
    lcd.setCursor(6, 0);
    lcd.print("APSU");
  //Actuators need to be in same state at all times
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    delay(1500); //for the length of time it takes for the actuators to test
  //Actuators need to be in same state at all times
    digitalWrite(R1, HIGH);
    digitalWrite(R2, HIGH);
    delay(1500);


      // scroll 8 positions (display length + string length) to the left
    for (int positionCounter = 0; positionCounter < 8; positionCounter++)  {
        // scroll one position left:
        lcd.scrollDisplayLeft();
        // wait a bit:
        delay(200);
       }


    lcd.clear();
    delay(500);

    //Button innitialization; happens after start up
    pinMode(btnLEFT, INPUT_PULLUP);
    pinMode(btnRIGHT, INPUT_PULLUP);
    pinMode(btnSTART, INPUT_PULLUP);

}


//===========================================================================================//
//===========================================================================================//
//                                       VOID LOOP                                           //
//===========================================================================================//
//===========================================================================================//

void loop()  {
  //Set up for the action of the RIGHT button
  if (!digitalRead(btnRIGHT)){
   menu++;
   updateMenu();
   delay(100);
   while(!digitalRead(btnRIGHT));
  }
  //Set up for the action of the LEFT button
  if (!digitalRead(btnLEFT)){
    menu--;
    updateMenu();
    delay(100);
    while(!digitalRead(btnLEFT));
  }
  //Set up for the action of the START button
 if (!digitalRead(btnSTART)){
    executeAction();
    updateMenu();
    delay(100);
    while(!digitalRead(btnSTART));
  }
 }

  //===========================================================================================//
  //                                         MENU                                              //
  //===========================================================================================//
void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
   //Cursor is on PLA   
    case 1:
      lcd.clear();
      lcd.print(">PLA");
      lcd.setCursor(5,0);
      lcd.print("PETG");
      lcd.setCursor(9,0);
      lcd.print(" Kydex");
      lcd.setCursor(2,1);
      lcd.print("Weld at 200F");
      break;
   //Cursor is on PETG 
    case 2:
      lcd.clear();
      lcd.print(" PLA");
      lcd.setCursor(4,0);
      lcd.print(">PETG");
      lcd.setCursor(9,0);
      lcd.print(" Kydex");
      lcd.setCursor(2,1);
      lcd.print("Weld at 164F");
      break;
   //Cursor is on Kydex  
    case 3:
    lcd.clear();
      lcd.print(" PLA");
      lcd.setCursor(5,0);
      lcd.print("PETG");
      lcd.setCursor(9,0);
      lcd.print(">Kydex");
      lcd.setCursor(2,1);
      lcd.print("Weld at 200F");
      break;
    case 5:
      menu = 3;
      break;
      }
}

//allows for the start button to execute the actions
void executeAction() {
  switch (menu) {
    case 1:
      action1();
      break;
    case 2:
      action2();
      break;
    case 3:
      action3();
      break;
  }
}


  //===========================================================================================//
  //                                         ACTION 1                                          //
  //===========================================================================================//

// Helper Functions Block

// Calculates the average of a given array
float getAverage(float A[]) {
  return ((A[0]+A[1]+A[2]+A[3])/4.0);
}

// Updates the current and last temperature arrays
void updateTemperature(float lastTemperatures[], float temperatures[]) {
  lastTemperatures[0] = temperatures[0];
  lastTemperatures[1] = temperatures[1];
  lastTemperatures[2] = temperatures[2];
  lastTemperatures[3] = temperatures[3];

  temperatures[0] = thermocouple1.readFahrenheit();
  temperatures[1] = thermocouple2.readFahrenheit();
  temperatures[2] = thermocouple3.readFahrenheit();
  temperatures[3] = thermocouple4.readFahrenheit();
}

// Updates the currentBoostValue (heating or welding)
void updateCurrentBoostValue(float currentBoostValue[], float boostTempbyValue) {
  currentBoostValue[0] = thermocouple1.readFahrenheit() + boostTempbyValue;
  currentBoostValue[1] = thermocouple2.readFahrenheit() + boostTempbyValue;
  currentBoostValue[2] = thermocouple3.readFahrenheit() + boostTempbyValue;
  currentBoostValue[3] = thermocouple4.readFahrenheit() + boostTempbyValue;
}

// Toggle the heaters on and off >> toggleHeater(true) : On | toggleHeater(false) : Off
void toggleHeater(bool status) {
  if (status == true) {
    // Turn all the heaters on
    digitalWrite(heater1, LOW);
    digitalWrite(heater2, LOW); 
    digitalWrite(heater3, LOW); 
    digitalWrite(heater4, LOW);
  }
  else if (status == false) {
    // Turn all the heaters off
    digitalWrite(heater1, HIGH);
    digitalWrite(heater2, HIGH); 
    digitalWrite(heater3, HIGH); 
    digitalWrite(heater4, HIGH);
  }
}
// End Helper function block

void action1() {
// VARIABLE BLOCK
  const int targetTemp = 410; // Farenheit
  const float boostTempbyValueHeating = 20; // Farenheit
  const float boostTempbyValueWelding = 5; // Farenheit
  
  // Current temperature + boost temp value.
  float currentBoostValueHeating[4];
  float currentBoostValueWelding[4];
  
  // Stores the current temp for each heater (in Farenheit): temperature[0] is heater1, temperature[1] is heater2, ..., temperature[3] is heater 4 
  float temperatures[4] = {70, 70, 70, 70};

  // Stores the last temperatures before reupdating the temperatures array
  float lastTemperatures[4] = {0, 0, 0, 0};

// END OF VARIABLE BLOCK

 // Reset the LCD screen and print the mode the machine is in
  lcd.clear();
  lcd.print("Executing PLA");
  delay(2000);

  // read the temperatures from all 4 sensors 
  updateTemperature(lastTemperatures, temperatures);  

  // Get the average temperature
  lcd.clear();
  lcd.print("Stage 1"); // Stage 1
  lcd.setCursor(0, 1);
  lcd.print("AVG TEMP: ");
  float avgTemp = getAverage(temperatures);
  lcd.print(avgTemp, 1);
  lcd.print("F");
  delay(1000); // Delay for stability and readability;

  // Turn all the heaters on
  toggleHeater(true);

  // save the temperatures from all 4 sensors + CurrentBoostValue >> (20) to create our starting boost
  updateCurrentBoostValue(currentBoostValueHeating, boostTempbyValueHeating);

  while(true) {
    // save the current and last temperatures from all 4 sensors
    updateTemperature(lastTemperatures, temperatures);

    // Find the heater with the highest current temperature using Linear search
    float highest = temperatures[0];
    for (int i=1; i <= 3; i++) {
      if (temperatures[i] > highest) {
        highest = temperatures[i];
      }
    }
 
    // The average of all the current boost values (Temperatures + boostTempbyValue)
    float currentBoostValueHeatingAverage = getAverage(currentBoostValueHeating);

    if (highest < currentBoostValueHeatingAverage) {
    // update the screen with the new avg temperature while temperature is less than currentBoostValueHeating
      lcd.clear();
      lcd.print("Stage 2,1");
      lcd.setCursor(0, 1);
      lcd.print("AVG TEMP: ");
      avgTemp = getAverage(temperatures);
      lcd.print(avgTemp, 1);
      lcd.print("F");
      delay(1000); // Delay for stability and readability
      continue; // Start at the beginning of the while loop
    }

    if (highest >= currentBoostValueHeatingAverage) {
      // add the updated temperatures to the screen
      lcd.clear();
      lcd.print("Stage 2,2");
      lcd.setCursor(0, 1);
      lcd.print("AVG TEMP: ");
      avgTemp = getAverage(temperatures);
      lcd.print(avgTemp, 1);
      lcd.print("F");
      delay(1000); // Delay for stability and readability

      // Turn off heaters and delay until current temperature is lower than last temperature twice
      // Turn all the heaters off
      toggleHeater(false);

      // save the temperatures from all 4 sensors + CurrentBoostValue >> (20) to create our NEW UPDATED boost for the next go-around
      updateCurrentBoostValue(currentBoostValueHeating, boostTempbyValueHeating);

      // Check if the heaters drop below the saved temperatures twice
      int temperatureDecreaseCount[4] = {0, 0, 0, 0};

      // Check if each heater has cooled down twice (Checking if temperature is less than its previous temperature from the last cycle)
      while (temperatureDecreaseCount[0] < 2 || temperatureDecreaseCount[1] < 2 || temperatureDecreaseCount[2] < 2 || temperatureDecreaseCount[3] < 2) {
        updateTemperature(lastTemperatures, temperatures);

        for (int i = 0; i <= 3; i++) {
          if (temperatures[i] < lastTemperatures[i]) {
            temperatureDecreaseCount[i]++;
          }
        }
        lcd.clear();
        lcd.print("Stage 2,3");
        lcd.setCursor(0, 1);
        lcd.print("AVG TEMP: ");
        avgTemp = getAverage(temperatures);
        lcd.print(avgTemp, 1);
        lcd.print("F");
        delay(1000); // Delay for stability and readability
      }

      updateTemperature(lastTemperatures, temperatures);

      // Turn the heaters back on since the temperature has stabilized
      toggleHeater(true);

      // Go back to the start of the while loop
      continue;
  }

  // Starts the welding phase once the target temp has been reached
  if (highest >= targetTemp) {
    break;
  }
}

// Start the welding phase
  lcd.clear();
  lcd.print("WELDING IN");
  lcd.setCursor(0, 1);
  lcd.print("PROGRESS");
  delay(500); // Pre-welding delay

 // Lower the actuators
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  delay(1500);

// (Actual Welding Time)
  for(int i=0; i < 3;) {
    // save the temperatures from all 4 sensors
    updateTemperature(lastTemperatures, temperatures);

    // save the temperatures from all 4 sensors + CurrentBoostValue >> (20)
    updateCurrentBoostValue(currentBoostValueWelding, boostTempbyValueWelding);
     
    // Find the heater with the highest value using selection sort
    float highest = temperatures[0];
    for (int j=1; j <= 3; j++) {
      if (temperatures[j] > highest) {
        highest = temperatures[j];
      }
    }

    // The average of all the (Temperatures + boostTempbyValue)
    float currentBoostValueWeldingAverage = getAverage(currentBoostValueWelding);

    if (highest < currentBoostValueWeldingAverage) {
      continue;
    }

    if (highest >= currentBoostValueWeldingAverage) {
    // Turn off heaters and delay until current temperature is lower than last temperature twice
      // Turn all the heaters off
      toggleHeater(false);

      // Check if the heaters drop below the saved temperatures twice
      int temperatureDecreaseCount[4] = {0, 0, 0, 0};
        
      // Check if each heater has cooled down twice (Checking if temperature is less than its previous temperature from the last cycle)
      while (temperatureDecreaseCount[0] < 2 || temperatureDecreaseCount[1] < 2 || temperatureDecreaseCount[2] < 2 || temperatureDecreaseCount[3] < 2) {
        updateTemperature(lastTemperatures, temperatures);
        delay(500);
        for (int i = 0; i <= 3; i++) {
          if (temperatures[i] < lastTemperatures[i]) {
            temperatureDecreaseCount[i]++;
          } 
          else {
            temperatureDecreaseCount[i] = 0;
          }
        }
      }

      // Turn the heaters back on since the temperature has stabilized
      toggleHeater(true);

      i++;
    }

    // Turn all the heaters off
   toggleHeater(false);

    // Lift the actuators back up
    digitalWrite(R1, HIGH);
    digitalWrite(R2, HIGH);
    delay(500);

    // Cooling phase
    lcd.clear();
    lcd.print("COOLING");
    do {
      // Update temperatures and print the new average
      updateTemperature(lastTemperatures, temperatures);
    
      // Update the LCD with the current average temperature
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("AVG TEMP: ");
      avgTemp = getAverage(temperatures);
      lcd.print(avgTemp);
      lcd.print("F");

      delay(1000); // Update temperature every second
    } while (avgTemp > 200); // Loop until the average temperature is <= 90 degrees Fahrenheit
  }
}


  //===========================================================================================//
  //                                         ACTION 2                                          //
  //===========================================================================================//
//Action for PETG
void action2() {
 lcd.clear();
 lcd.print(">Executing PETG");
 delay(1500);

}

  //===========================================================================================//
  //                                         ACTION 3                                          //
  //===========================================================================================//
//Action for Kydex
void action3() {
 lcd.clear();
 lcd.print(">Executing Kydex");
 delay(1500);

}