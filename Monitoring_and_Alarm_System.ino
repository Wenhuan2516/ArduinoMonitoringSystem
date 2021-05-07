/* The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 */

// include the library code:
#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#include <Wire.h>
#include "RTClib.h"


#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
RTC_DS3231 rtc;
void showTime(void);
DateTime now;

int buzzer = 6;//the pin of the active buzzer

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);

   if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC Module");
    while (1);
  }

  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

   pinMode(buzzer,OUTPUT);//initialize the buzzer pin as an output
  
}

void loop() {
  // put your main code here, to run repeatedly:

  now = rtc.now();
  showTime();
  
  float temperature;
  float humidity;

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( &temperature, &humidity ) == true )
  { 
    lcd.setCursor(0, 1);
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );

   if ( humidity >= 85) 
    {
       unsigned char i;
        while(1)
        {
          //output an frequency
          for(i=0;i<80;i++)
          {
            digitalWrite(buzzer,HIGH);
            delay(1);//wait for 1ms
            digitalWrite(buzzer,LOW);
            delay(1);//wait for 1ms
          }
          //output another frequency
          for(i=0;i<100;i++)
          {
            digitalWrite(buzzer,HIGH);
            delay(2);//wait for 2ms
            digitalWrite(buzzer,LOW);
            delay(2);//wait for 2ms
          }
        }
        int H = now.hour();
        int M = now.minute();
        int S = now.minute();
        lcd.setCursor(0,0);
        lcd.print("Time:");
        if ( H < 10)
         {
           lcd.print('0');
           lcd.print(H);
         } else {
            lcd.print(H);
         }
         
         lcd.print(':');
          if(M < 10) {
            lcd.print('0');
            lcd.print(M);
            } else {
            lcd.print(M);
            }
        lcd.print(':');
        if (S < 10) 
        {
            lcd.print('0');
            lcd.print(S);
         }else {
            lcd.print(S);
        }
     }
  }
}
void showTime()
 {
  lcd.setCursor(0,0);
  lcd.print("Time:");
     if ( now.hour() < 10)
     {
      lcd.print('0');
      lcd.print(now.hour());
     } else {
       lcd.print(now.hour());
     }
        
  lcd.print(':');
    if ( now.minute() < 10) 
    {
      lcd.print('0');
      lcd.print(now.minute());  
      } else {
        lcd.print(now.minute());
        }
    lcd.print(':');
     if (now.second() < 10) 
     {
      lcd.print('0');
      lcd.print(now.second());
     }else {
      lcd.print(now.second());
     }
 } 
