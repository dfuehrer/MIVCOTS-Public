#include "Adafruit_GPS.h"  //for the GPS

//HardwareSerial recv_gps = Serial1;
Adafruit_GPS GPS(&Serial1);

void setup() {
 Serial.begin(115200);
 GPS.begin(9600);
 
 GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  // Output string formatting
 GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);     // 1 Hz updates
 //GPS.sendCommand(PGCMD_ANTENNA);                // antenna status
 
 delay(1000);  
 
 Serial1.println(PMTK_Q_RELEASE);
}

uint32_t timer = millis();
uint32_t msgCounter = 0;

// Zero pads days, months, hours, minutes, or seconds for transmission (or anything 2 digits or less, base 10)
void printDateTime(uint8_t date) {
  if (date < 10) {
    Serial.print('0');
  }
  Serial.print(date);
}

// Prints a time in milliseconds and pads with zeros up to 3 digits
void print_ms(uint16_t ms) {
  if (ms < 10){
    Serial.print('0');
  }

  if (ms < 100) {
    Serial.print('0');
  }

  Serial.print(ms);
}

// Prints a lattitude or longitude with the correct sign based on the dir char
void printLatLon(float lat_lon, char dir){
  if ((dir == 's') || (dir == 'w')) {
    Serial.print((long)(-lat_lon * 1000000));
  }
  else {
    Serial.print((long)(lat_lon * 1000000));
  }
}

void loop() {
  char c = GPS.read();
  //Serial.println(c);

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
  
  if (millis() - timer > 2000) { 
    timer = millis(); // reset the timer

    if (GPS.fix) {
      Serial.print("#,"); Serial.print(msgCounter); Serial.print(",");
      Serial.print("AA,"); Serial.print('0'); Serial.print(",");
      Serial.print("HB,"); Serial.print('9'); Serial.print(",");
      Serial.print("AD,"); Serial.print("20"); Serial.print(GPS.year); printDateTime(GPS.month); printDateTime(GPS.day); Serial.print(",");
      Serial.print("AE,"); printDateTime(GPS.hour); printDateTime(GPS.minute); printDateTime(GPS.seconds); print_ms(GPS.milliseconds); Serial.print(",");
      Serial.print("AF,"); printLatLon(GPS.longitudeDegrees, GPS.lon); Serial.print(",");
      Serial.print("AG,"); printLatLon(GPS.latitudeDegrees, GPS.lat); Serial.print(",");
      Serial.print("AJ,"); Serial.print((long)(GPS.altitude * 100)); Serial.print(","); //TODO: is this the right scale factor?
      Serial.print("AH,"); Serial.print((long)(GPS.angle * 100)); Serial.print(",");
      //TODO: satellites, temp?
      Serial.print("!\n"); 
      ++msgCounter;
    } // if has fix
  } // if enough time has passed
  
} // loop()

// Old example printing code
 /*   
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS.latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(GPS.longitudeDegrees, 4);
      
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
*/
