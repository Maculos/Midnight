/*
Midnight IO Processor
*/

/*
Pico Serial Ports
Serial - USB Port Output (ignores baud rate)
Serial1 - default pins 1 & 2
*/

//Includes
#include <NMEAGPS.h> //Parses GPS data
#include <Streamers.h> //Needed for NeoGPS
#include <SPI.h>
#include <SD.h>

File logFile;

//Required setup for devices
static NMEAGPS  gps; //GPS Setup

char buffer[50]; //buffer for string conversions

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.print(F("fix object size = "));
  Serial.println(sizeof(gps.fix()));
  Serial.print(F("NMEAGPS object size = "));
  Serial.println(sizeof(gps));
  Serial.flush();
  Serial1.begin(9600);

  pinMode(22, OUTPUT);

  if (!SD.begin(17)) {
    Serial.println("SD Error, ignoring");
  }
  logFile = SD.open("gps.txt", FILE_WRITE);
}

static void processGPS(const gps_fix & fix)
{
  if(fix.valid.location) {
    Serial.print("\nLatitude: ");
    Serial.println(fix.latitude(), 6);
    Serial.print("Longitude: ");
    Serial.println(fix.longitude(), 6);

    logFile.print("\nLatitude: ");
    logFile.println(fix.latitude(), 6);
    logFile.print("Longitude: ");
    logFile.println(fix.longitude(), 6);

    sprintf(buffer, "D/M/Y Date: %d.%d.%d", fix.dateTime.date, fix.dateTime.month, fix.dateTime.year); //This is *Bad* code, will fix
    Serial.println(buffer);
    logFile.println(buffer);

    sprintf(buffer, "Time: %d.%d.%d", fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds); //This is *Bad* code, will fix
    Serial.println(buffer);
    logFile.println(buffer);

    Serial.print("Alt (m): ");
    Serial.println(fix.altitude(), 6);
    Serial.print(" | Km/h: ");
    Serial.println(fix.speed_kph(), 6);

    logFile.print("Alt (m): ");
    logFile.println(fix.altitude(), 6);
    logFile.print(" | Km/h: ");
    logFile.println(fix.speed_kph(), 6);

    Serial.print("Sats: ");
    Serial.println(fix.satellites);

    logFile.print("Sats: ");
    logFile.println(fix.satellites);
  } else {
    //Display waiting message until connection is made
    Serial.println("No Valid Location Data Found");
    logFile.println("No Valid Location Data Found");
  }
  tone(22, 4000, 100);
}

void loop()
{
  while (gps.available(Serial1))
    processGPS(gps.read());
}