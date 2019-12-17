#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

//***************************************************************************

//Define data pin and LED intensity
#define PIN 9
#define BRIGHTNESS 32

// Define matrix width and height.
#define mw 32
#define mh 8

//Initialize the LED array:
//Defines the dimensions of the array, the pin used to transmit data
//Defines the first LED as the top left corner
//Defines the LEDs as grouped in columns wired in a zig-zag pattern
//Defines the LEDs as high density (800 kHz) GRB bitstream
Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(mw, mh, PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

//Define LED colors
#define RED      0xF800
#define GREEN    0x07E0
#define YELLOW   0xFFE0 

//***************************************************************************

//Choose the computer's number
int comp_num;
int guess;

//Scrolling text function
void scrollText(String text, int c) {
  matrix->clear();
  for (int x=30; x>=-150; x--){
  	matrix->clear();
  	matrix->setCursor(x,0);
  	matrix->setTextColor(c);
  	matrix->print(text);
  	matrix->show();
    delay(50);
  }
}

//Game function
void guessing_game(){
  for (int i = 1; i <= 1000; i++) {

    if(Serial.available() > 0){
      guess = Serial.parseInt(); 
    }
    
    Serial.print("You guessed: ");
    Serial.println(guess);
    
    if (guess == comp_num){
      scrollText("You guessed it!", GREEN);
      scrollText("Total Guesses: " + String(i), GREEN);
      break;
    }
    else if (guess > comp_num + 10){
      scrollText(String(guess) + " is way too high", RED);
    }
    else if (guess > comp_num && guess < comp_num + 10){
      scrollText(String(guess) + " is too high", YELLOW);
    }
    else if (guess < comp_num-10){
      scrollText(String(guess) + " is way too low", RED);
    }
    else if (guess < comp_num && guess > comp_num-10){
      scrollText(String(guess) + " is too low", YELLOW);
    }
    delay(3000);
  }
}

void stop(){
  while(1);
}

//***************************************************************************

void setup() {
  randomSeed(analogRead(0)); //Random noise seeds the generator
   
  Serial.begin(115200);

  //Initialize the matrix using earlier parameters
  matrix->begin();
  matrix->clear();
  matrix->setTextWrap(false);
  matrix->setBrightness(BRIGHTNESS);
}

void loop() {
  comp_num = random(1, 101);

  Serial.println("Guess a number beween 1 and 100.");
  delay(3000);
  guessing_game();
  
  Serial.println("Would you like to play again? (y/n)");
  delay(5000);
  if (Serial.available() > 0) {
    char repeat = Serial.read();
    if (repeat != 'y'){
      stop();
    }
  }
}
