
#include <CircularBuffer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define CHAR_LIM 21      // Max number of characaters per line

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


CircularBuffer<String, 4> buffer;

unsigned long time = 0;

#define SAMPLE_PIN A0

void setup() {

  Serial.begin(115200);
  pinMode(SAMPLE_PIN, INPUT);
  time = millis();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(50, 50, SSD1306_WHITE);
  display.drawPixel(10, 10, SSD1306_WHITE);

  display.display();

  display.println(F("Hello, world!"));

  //display.display();

  //delay(2000); // Pause for 2 seconds

  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  //delay(2000);

  display.invertDisplay(true);
  //delay(1000);
  display.invertDisplay(false);
  //delay(1000);
  display.clearDisplay();

  display.println(F("Hello, world again!"));

  display.display();

  delay(1000); // Pause for 2 seconds

  display.clearDisplay();


}
// Can print four rows of 21 characters each
// Buffer can contain four strings of 21 character each?

String incomingByte = "";

void loop() {
  // put your main code here, to run repeatedly:

  incomingByte = "";
  //display.setTextSize(1);      // Normal 1:1 pixel scale
  //display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  //display.cp437(true);         // Use full 256 char 'Code Page 437' font

  /*buffer.push(1);
  buffer.push(2);
  buffer.push(3);
  buffer.push(4);
  */
  while (Serial.available() > 0) {
    // read the incoming byte:
    //21 normal characters per line
    //char incomingByte[21];
    String incomingByte = Serial.readString();
    incomingByte =  incomingByte.substring(0, 21);
//    incomingByte[strlen(incomingByte) - (strlen(incomingByte)- CHAR_LIM)]  = '\0';
    
    // say what you got:
    //Serial.print("I received: ");
    //Serial.println(incomingByte);
    buffer.push(incomingByte);
    for (int i = 3; i > -1; i--) {
      if (incomingByte.length() == 21)
      {
        display.println(buffer[i]);
      }
      else
      {
        display.print(buffer[i]);
      }
      //Serial.println(buffer[i]);

    }
    
    delay(1);
    display.display();
    display.clearDisplay();
    //display.setCursor(0, 0);     // Start at top-left corner

  }


  /*for (int i = 3; i > -1; i--) {
      display.println(buffer[i]);
      Serial.print(buffer[i]);

    }*/


/*
  buffer.push(5);
  buffer.push(6);
  buffer.push(7);
  buffer.push(8);
*/
/*
  for (int i = 3; i > -1; i--) {
      display.println(buffer[i]);
      Serial.print(buffer[i]);

    }*/

/*
 delay(3000);
 display.display();
 display.clearDisplay();
*/

}
