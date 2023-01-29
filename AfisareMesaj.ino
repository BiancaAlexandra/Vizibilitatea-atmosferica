#include <LiquidCrystal.h>
#include <SimpleDHT.h>

int pinDHT11 = 2;

SimpleDHT11 dht11;

LiquidCrystal lcd(12, NULL, 11, 9,8,7,6);

int sensorPin = A4; // select the input pin for the potentiometer
int sensorValue = 0; // variable to store the value coming from the sensor

int measurePin = A5;
int ledPower = 5;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup() {
  
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  
  pinMode(ledPower,OUTPUT);
  pinMode(sensorPin, INPUT); 
  
  
}

void loop() {

  byte temp, hum;  
  
  byte chk = dht11.read(pinDHT11, &temp, &hum, NULL);
  
  lcd.setCursor(0, 0);

  //Senzor temperatura
  lcd.print("Temp:");
  lcd.print(temp);
  lcd.print(char(223));
  lcd.print("C ");

  //Senzor umiditate
  lcd.print("Umid:");
  lcd.print(hum);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  
  //Senzor lumina
  int vout1 = analogRead(sensorPin); // Read the analogue pin

  float volts0=vout1*(5.0/1024);   // calculate the voltage
  Serial.print(volts0);  //raw voltage
  Serial.println(" Volts.");
  Serial.print((500/((10.72/(5-volts0))*volts0)), 2);   //lux calculation
  Serial.println(" lux");
  float lux =500/((10.72/(5-volts0))*volts0);   //lux calculation
  
  lcd.print("Iluminare:");
  lcd.print(lux);  
  lcd.print("Lux "); 
 // lcd.clear(); 
  
  lcd.setCursor(0, 2);
  
  //Senzor praf

  digitalWrite(ledPower,LOW);
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin);

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured*(5.0/1024);
  dustDensity = 0.17*calcVoltage-0.1;

  if ( dustDensity < 0)
  {
    dustDensity = 0.00;
  }

  Serial.println("Raw Signal Value (0-1023):");
  Serial.println(voMeasured);

  Serial.println("Voltage:");
  Serial.println(calcVoltage);

  Serial.println("Dust Density:");
  Serial.println(dustDensity);

  lcd.print("Dust:");
  lcd.print(dustDensity);
  lcd.print("ug/m3");

  lcd.setCursor(0, 3);
  
  /*temp =14;
  hum = 63;
  lux = 266.04;
  dustDensity = 0.1;*/

  //aproximarea mea
  if((hum >=90 && hum <=100) && (temp <1) && (lux >=0 && lux <100) && (dustDensity >=0.3))
  {
    lcd.print("Viz<1m");
  }
  else
  {
    if((hum >=70 && hum <=90) && (lux >=100) && (dustDensity >=0.3))
    {
      lcd.print("Viz:1-2km");
    }
    else
    {
      if((hum >=50 && hum <=70) && (dustDensity >=0 && dustDensity <=0.1))
      {
        lcd.print("Viz:2-10km");
      }
      else
      {
        if((hum >=50 && hum <=70) && (temp >=5) && (lux >=200) && (dustDensity >=0.25 && dustDensity <=0.4))
        {
          lcd.print("Viz:1-10km");
        }
        else
        {
          if((hum >=70 && hum <=90) && (temp >=0 && temp <10) && (lux <=100) && (dustDensity >=0.5))
          {
            lcd.print("Viz:0-50m");
          }
          else
          {
            if((hum >=50 && hum <70) && (lux <= 150) && (dustDensity >=0.4))
            {
              lcd.print("Viz:50-200m");
            }
            else
            {
              if((hum >=80 && hum <=90) && (lux <= 200) && (dustDensity <=0.2))
              {
                lcd.print("Viz:200-500m");
              }
              else
              {
                if((hum >=50 && hum <60) && (lux >=200 && lux <285) && (dustDensity <=0.2))
                {
                  lcd.print("Viz:500-1000m");
                }
                else
                {
                  if((hum >=40 && hum <=50) && (lux >=200) && (dustDensity >=0.2 && dustDensity <=0.3))
                  {
                    lcd.print("Viz:4-10km");
                  }
                  else
                  {
                    lcd.print("Viz>10km");
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  
  //aproximare stiintifica
  if((hum >=90 && hum <=100) && (temp >=2.5 && temp <=5) && (lux >= 285) && (dustDensity >=0 && dustDensity <=0.1))
  {
    //lcd.print("Vizib:500m-1km");
    lcd.print(":500m-1km");
  }
  else
  {
    if((hum >=90 && hum <=100) && (temp >=0.5 && temp <2.5) && (lux >=100 && lux <285) && (dustDensity >=0 && dustDensity <=0.1))
    {
      //lcd.print("Vizib:200m-500m");
      lcd.print(":200m-500m");
    }
    else
    {
      if((hum >=90 && hum <=100) && (temp >=-2 && temp <0.5) && (lux >=12 && lux <100) && (dustDensity >=0 && dustDensity <=0.2))
      {
        //lcd.print("Vizib:50m-200m");
        lcd.print(":50m-200m");
      }
      else
      {
        if((hum >=90 && hum <=100) && (temp >=-5 && temp <-2) && (lux >=0 && lux <12) && (dustDensity >=0 && dustDensity <=0.2))
        {
          //lcd.print("Vizib<50m");
          lcd.print(":<50m");
        }
        else
        {
          if((hum >=70 && hum <=100) && (lux >=100) && (dustDensity >=0.35))
          {
            //lcd.print("Vizib:1-10km");
            lcd.print(":1-10km");
          }
          else
          {
            if((hum >=0 && hum <70) && (lux >=100) && (dustDensity >=0.35))
            {
              //lcd.print("Vizib:1-10km");
              lcd.print(":1-10km");
            }
            else
            {
              if((hum >=70 && hum <=100) && (dustDensity >=0.25 && dustDensity <0.35))
              {
                //lcd.print("Vizib:1-2km");
                lcd.print(":1-2km");
              }
              else
              {
                if((hum >=0 && hum <70) && (dustDensity >=0 && dustDensity <0.35))
                {
                  //lcd.print("Vizib:2-10km");
                  lcd.print(":2-10km");
                }
                else
                {
                  if((hum >=90 && hum <=100) && (lux >=0 && lux <=100) && (dustDensity >=0.35))
                  {
                    //lcd.print("Vizib<1m");
                    lcd.print(":<1m");
                  }
                  else
                  {
                    //lcd.print("Vizib>10km");
                    lcd.print(":>10km");
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  delay(1000);

}
