#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RCSwitch.h>
#include <Encoder.h>

/*
 * Pinout
 * -------------------------------------------------------
 * -------------------------------------------------------
 * Receiver     D5
 * Transmitter  D6
 * Jammer       D8
 * 
 * SCL          D3
 * SDA          D4
 * 
 * CLK          D1
 * DT           D2
 * SW           D7
 * 
 */
Adafruit_SSD1306 display(-1);

Encoder encoder(D1, D2);
RCSwitch receiver = RCSwitch();
RCSwitch transmitter = RCSwitch();
RCSwitch jammer = RCSwitch();

long oldPosition = -999;
boolean isButtonPressed = false;
long lastUpdateMillis = 0;

#define menuLength 5
String menuOptions[] = {"||Wmk433||", "Jammer", "Sniffer", "Spoofer", "Hacker", "Transmit"};
int menuPosition = 0;

void handleKey()
{
  isButtonPressed = true;
}

void printLine(String message, int line, bool inverted)
{
  display.setTextSize(2);
  if (!inverted)
    display.setTextColor(WHITE);
  else
    display.setTextColor(BLACK, WHITE);
  display.setCursor(0, 20 * line + 4);
  display.println(message);
}

void displayMenu()
{
  display.clearDisplay();
  display.display();

  if (menuPosition == 0)
  {
    printLine(menuOptions[0], 0, false);
    printLine(menuOptions[1], 1, false);
    printLine(menuOptions[2], 2, false);
  }
  else if (menuPosition == menuLength)
  {
    printLine(menuOptions[menuLength - 2], 0, false);
    printLine(menuOptions[menuLength - 1], 1, false);
    printLine(menuOptions[menuLength], 2, true);
  }
  else
  {
    printLine(menuOptions[menuPosition - 1], 0, false);
    printLine(menuOptions[menuPosition], 1, true);
    printLine(menuOptions[menuPosition + 1], 2, false);
  }
  display.display();
}

//return true if button is pressed and return the main-menu
bool exitOption()
{
  if (isButtonPressed)
  {
    delay(10);
    isButtonPressed = false;
    displayMenu();
    return true;
  }
  return false;
}

void initJammer()
{
  //Init Display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Jammer");

  display.setTextSize(2);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(115, 0);
  display.println("X");

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 25);
  display.println("Jamming ...");
  display.display();

  //Start jamming
  while (!exitOption())
  {
    delay(30);
  }
}

//Display the Sniffer Headline on the Display
void displaySniffer()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Sniffer");

  display.setTextSize(2);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(115, 0);
  display.println("X");
  display.display();
}

void initSniffer()
{
  displaySniffer();

  //Receiving and displaying data in this loop
  while (!exitOption())
  {
    if (receiver.available())
    {
      display.clearDisplay();
      displaySniffer();

      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 22);
      display.println(receiver.getReceivedValue());

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 42);
      display.println(receiver.getReceivedBitlength());
      display.setCursor(12, 42);
      display.println("bit");
      display.setCursor(40, 42);
      display.println(receiver.getReceivedDelay());
      display.setCursor(58, 42);
      display.println("us");
      display.display();
      receiver.resetAvailable();
    }
    delay(5);
  }
}

void spoofer()
{
  //Received Signal is stored here
  long receivedValue = 0;
  int receivedBitlength = 0;
  int receivedDelay = 0;
  int receivedProtocol = 0;

  //Init Display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Spoofer");

  display.setTextSize(2);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(115, 0);
  display.println("X");
  display.display();

  //Listen for signals
  receiver.resetAvailable();
  bool scanning = true;
  bool transmitting = false;
  while (scanning)
  {
    if (receiver.available())
    {
      receivedValue = receiver.getReceivedValue();
      receivedBitlength = receiver.getReceivedBitlength();
      receivedDelay = receiver.getReceivedDelay();
      receivedProtocol = receiver.getReceivedProtocol();
      receiver.resetAvailable();
      scanning = false;
      //Signal is received and stored
    }
    if (isButtonPressed) //Abort Sacanning
    {
      delay(30);
      isButtonPressed = false;
      displayMenu();
      return;
    }
    delay(10);
  }

  //Display Signal
  int selectedOption = 0;
  bool update = true;
  while (!scanning)
  {
    long newPosition = encoder.read(); //Handle menu ooptions
    if (newPosition != oldPosition)
    {
      if ((newPosition % 4 == 0) && ((newPosition - oldPosition) > 0))
      {
        if (selectedOption < 2)
        {
          selectedOption += 1;
          update = true;
        }
        else
          selectedOption = 0;
      }
      else if ((newPosition % 4 == 0) && ((newPosition - oldPosition) < 0))
      {
        if (selectedOption > 0)
        {
          selectedOption -= 1;
          update = true;
        }
        else
          selectedOption = 2;
      }
      oldPosition = newPosition;
    }

    if (update) //Display Signal
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 7);
      display.println(receivedValue);

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 27);
      display.println(receivedBitlength);
      display.setCursor(12, 27);
      display.println("bit");
      display.setCursor(40, 27);
      display.println(receivedDelay);
      display.setCursor(58, 27);
      display.println("us");

      if (selectedOption == 0)
        display.setTextColor(BLACK, WHITE);
      else
        display.setTextColor(WHITE);
      display.setCursor(0, 50);
      display.println("Send");
      if (selectedOption == 1)
        display.setTextColor(BLACK, WHITE);
      else
        display.setTextColor(WHITE);
      display.setCursor(42, 50);
      display.println("Search");
      if (selectedOption == 2)
        display.setTextColor(BLACK, WHITE);
      else
        display.setTextColor(WHITE);
      display.setCursor(95, 50);
      display.println("Exit");
      display.display();
    }
    if (isButtonPressed)
    {
      isButtonPressed = false;
      scanning = true;
      delay(30);
      if (selectedOption == 0)
        transmitting = true;
      if (selectedOption == 1)
        spoofer();
      if (selectedOption == 2)
      {
        displayMenu();
      }
    }
    delay(10);
  }

  //Transnmit signal
  if (transmitting)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 15);
    display.println("Transmitting ...");
    display.display();

    while (!exitOption())
    {
      jammer.setPulseLength(receivedDelay);
      jammer.setProtocol(receivedProtocol);
      delay(50);
    }
  }
  displayMenu();
}

void initHacker() //brute-force attack
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Hacker");

  display.setTextSize(2);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(115, 0);
  display.println("X");

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 25);
  display.println("Hacking...");
  display.display();

  jammer.setRepeatTransmit(2);
  int bitlength = 8;
  int maxlength = 256; //2^8
  for (int i = 0; i <= maxlength; i++)
  {
    delay(10);
    Serial.println(i);
    if (exitOption())
      break;
  }
  displayMenu();
}

int readSerialInt()
{
  int value = 0;
  while (!exitOption())
  {
    while (Serial.available())
    {
      char inChar = (char)Serial.read();
      if ((inChar >= '0') && (inChar <= '9'))
      {
        Serial.print(inChar);
        value = value * 10 + inChar - '0';
      }
      if (inChar == '\n')
      {
        return value;
      }
    }
  }
  return 0;
}

void initTransmit()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Transmit");

  display.setTextSize(2);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(115, 0);
  display.println("X");

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 25);
  display.println("Connect your PC and  open the Serial      Monitor!");
  display.display();

  Serial.println("Welcome to the wmk433");
  Serial.println("Please enter the binary code");

  unsigned long code = 0;
  int pulseLength = 0;
  int protocol = 0;
  int bitlength = 0;
  Serial.println("\nCode in dezimal: ");
  code = readSerialInt();
  if (!(code))
    return;
  Serial.println("\nNumber of bits: ");
  bitlength = readSerialInt();
  if (!(bitlength))
    return;
  Serial.println("\nDelay per bit: ");
  pulseLength = readSerialInt();
  if (!(pulseLength))
    return;
  Serial.println("\nProtocol: ");
  protocol = readSerialInt();
  if (!(protocol))
    return;
  jammer.setProtocol(protocol);
  jammer.setPulseLength(pulseLength);
  jammer.setRepeatTransmit(60);
  Serial.println("\nTransmitting ...");
}

void setup()
{
  Wire.begin(D4, D3); //Init communications standards
  Serial.begin(9600);

  jammer.enableTransmit(D8); //Init 433MHz equipment
  transmitter.enableTransmit(D6);
  receiver.enableReceive(D5);

  pinMode(D7, INPUT_PULLUP); //Init Encoder
  attachInterrupt(D7, handleKey, RISING);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Init Display with the I2C addr 0x3C (128x64) //SSD1306_SWITCHCAPVCC
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("@code_byter");
  display.display();
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0, 8);
  display.println("@code_byter");
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 24);
  display.println("@code_byter");
  display.display();

  Serial.println("RF-Hacker connected ...");
  displayMenu();
}

void loop()
{
  long newPosition = encoder.read();

  if (newPosition != oldPosition)
  {
    if ((newPosition % 4 == 0) && ((newPosition - oldPosition) > 0))
    {
      if (menuPosition < menuLength)
        menuPosition += 1;
      displayMenu();
    }
    else if ((newPosition % 4 == 0) && ((newPosition - oldPosition) < 0))
    {
      if (menuPosition > 0)
        menuPosition -= 1;
      displayMenu();
    }
    oldPosition = newPosition;
  }

  if (isButtonPressed)
  {
    isButtonPressed = false;
    delay(30);
    if (menuPosition == 2)
    {
      initSniffer();
    }
    else if (menuPosition == 1)
    {
      initJammer();
    }
    else if (menuPosition == 3)
    {
      spoofer();
    }
    else if (menuPosition == 4)
    {
      initHacker();
    }
    else if (menuPosition == 5)
    {
      initTransmit();
    }
  }
}
