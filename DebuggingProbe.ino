// Program that reads strings line by line from serial port
// and prints it to an OLED display. 

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
// The display can show 4 lines at a time, so the circular buffer is hardcoded to four items.

unsigned long time = 0;

#define SAMPLE_PIN A0

char serialbuf[21];

void setup() {

  Serial.begin(115200);      // Set Serial data rate to 115200 bps

  Serial.setTimeout(50);     // Time out in 50 ms
 
  pinMode(SAMPLE_PIN, INPUT);
  time = millis();           // Time is in milliseconds

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();

  // Clear the display buffer
  display.clearDisplay();

  // Draw a single pixel in white
  // TO BE DELETED: 
  // I haven't yet because I don't want touch the code before finding why it's not displaying again.
  // But eventually I want to get rid of all the startup code.
  display.drawPixel(50, 50, SSD1306_WHITE);
  display.drawPixel(10, 10, SSD1306_WHITE);

  display.display();

  display.println(F("Hello, world!"));

  display.clearDisplay();

  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.cp437(true);                 // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for (int16_t i = 0; i < 256; i++) {
    if (i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();

  display.clearDisplay();

  display.println(F("Hello, world again!"));

  display.display();

  delay(1000); // Pause for 2 seconds

  display.clearDisplay();


}
// Can print four rows of 21 characters each
// Buffer can contain four strings of 21 character each?


// Initialize String for incoming data from Serial stream
String incomingByte = "";

void loop() {
  // Main code here, to run repeatedly:

  incomingByte = "";
  display.setCursor(0, 0);     // Start at top-left corner

  // If there's information on the serial, commence reading and writing logic
  if (Serial.available() > 0) {
    // Read the incoming byte until the new line:
    incomingByte = Serial.readStringUntil('\n');

    // There are 21 normal characters per line, anything past it cannot be read.
    // Cut the string into the first 21 characters.
    incomingByte = incomingByte.substring(0, 21);

    // Say what you got and push string read into circular buffer:
    buffer.push(incomingByte);

    // Because the display can only print four lines, the buffer has a length of four.
    // When the line is at the character limit, 21, the newline is usually truncated, hence println.
    // Otherwise the string contains the newline character.
    for (int i = 3; i > -1; i--) {
      if (incomingByte.length() == 21)
      {
        display.println(buffer[i]);
      }
      else
      {
        display.print(buffer[i]);
      }
      Serial.println(buffer[i]);
      // Print to serial for debugging purposes

    }

    delay(1);
    display.display();
    // Display the buffer.
    
    display.clearDisplay();
    // Clear display so there's no overlay.
  }



}

int readline(char *sbuffer, int len) {
    static int pos = 0;
    int rpos;
    int readch;

    if (readch > 0) {
        switch (readch) {
            case '\r': // Ignore CR
                break;
            case '\n': // Return on new line
                rpos = pos;
                pos = 0;  // Reset position index ready for next time
                return rpos;
            case '\0': // Return on EOS
                rpos = pos;
                pos = 0;  // Reset position index ready for next time
                return rpos;
            default:
                if (pos < len-1) {
                    sbuffer[pos++] = readch;
                    sbuffer[pos] = 0;
                }
        }
    }
    return 0;
}
