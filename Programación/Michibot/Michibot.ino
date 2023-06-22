
// Codigo para sumobot

// -------------------------------------------------------------------------- Librerías
#include <NintendoExtensionCtrl.h>
#include <Servo.h> 
#include <Adafruit_NeoPixel.h>
#include <Wire.h>


// ----------------------------Defino pines a usar
#define pinBrazo    26
#define pinBrazo2   17
#define pinNeo      27 // RX 3  //led 23  //16 izq
#define npix        20  //8 y 20
#define inputA1     12
#define inputA2     14
#define inputB1     18
#define inputB2     19

#define pwm_min     128
#define pwm_max     255

#define pos_max     60
#define pos_min     160


// -------------------------------------------------------------------------- Objetos/variables

ClassicController classic;
Servo brazo, brazo2;
int contador;
int pwm;
Adafruit_NeoPixel strip(npix, pinNeo, NEO_RGB + NEO_KHZ800);


// botones del control  
boolean success = classic.update();
boolean padUp = classic.dpadUp();
boolean padDown = classic.dpadDown();
boolean padRight = classic.dpadRight();
boolean padLeft = classic.dpadLeft();
  
boolean aButton = classic.buttonA();
boolean bButton = classic.buttonB();
boolean bSelect = classic.buttonSelect();
boolean bStart = classic.buttonStart();

boolean sinMov = true;

// -------------------------------------------------------------------------- Setup
void setup() 
{
  Serial.begin(115200);
  Serial.println("Iniciando LuchaBot ...");

  //Wire.begin(5,4);
  Wire.begin();
  pinMode(inputA1,OUTPUT);
  pinMode(inputA2,OUTPUT);
  pinMode(inputB1,OUTPUT);
  pinMode(inputB2,OUTPUT);

  Serial.println("Motores Listos...");
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  colorWipe(strip.Color(  255, 0,   0)     , 50); // Green
  strip.show();            // Turn OFF all pixels ASAP
  delay(500);
  

  // pregunta solo al inicio pero sigue de largo si no lo encuentra
  while(!classic.connect()) 
  {
    
    
    classic.begin(); 
    Serial.println("Control no detectado!");
    colorWipe(strip.Color(  0, 255,   0)     , 50); // Green
    //colorWipe(strip.Color(  0,   0, 255)     , 50); // Blue
    //colorWipe(strip.Color(  0,   0,   0, 255), 50); // True white (not RGB white)
    //whiteOverRainbow(75, 5);
    //pulseWhite(50);
    //rainbowFade2White(3, 3, 1);
    delay(100);
  }
  Serial.println("Piloto listo. Arrancando LuchaBot...");

  brazo.attach(pinBrazo);
  brazo2.attach(pinBrazo2);
  rainbowFade2White(1, 1, 1);
  
}


// -------------------------------------------------------------------------- loop
void loop() 
{

 radioControl();
}



// ------------------------------------------------------------------------ RF
void radioControl(void)
{
    
  success = classic.update();
  padUp = classic.dpadUp();
  padDown = classic.dpadDown();
  padRight = classic.dpadRight();
  padLeft = classic.dpadLeft();
  
  
  aButton = classic.buttonA();
  bButton = classic.buttonB();
  bSelect = classic.buttonSelect();
  bStart = classic.buttonStart();

  if(padUp==true || padDown==true || padRight==true || padLeft==true || aButton == true || bButton == true) //No hay movimiento
  {
     sinMov = false;
    
    if(aButton==true) //A
    {
      Serial.println("A");
      if(padUp==true || padDown==true || padRight==true || padLeft==true)
        pwm = pwm_max;
      else
        {
          pwm = pwm_min;
          adelante();
        }
    }
    else
    {
      pwm = pwm_min;
    }
     
     if(padUp==true) //Para adelante
      {
        adelante();
        //display.setCursor (0,10);
        //display.print("W");
        Serial.println("Up");
      }
     
     if(padDown==true) //Para atrás
     {
        atras();
        //display.setCursor (0,10);
        //display.print("S");
        Serial.println("Down");
      }
    
    if(padRight==true) //Para derecha
      {
        derecha();
        //display.setCursor (0,10);
        //display.print(">");
        Serial.println("Rigth");
      }
    
    if(padLeft==true) //Para izquierda
    {
      izquierda();
      //display.setCursor (0,10);
      //display.print("<");
      Serial.println("Left");
    }
    
    }
    
    else
    {
      sinmovi(); 
      sinMov = true;
      //display.setCursor (0,10);
      //display.setTextColor(BLACK); 
      //display.write(127); // Borrar
      //display.setTextColor(WHITE); 
    }

    if(bButton==true) //B
    {
      abajo();
      //display.setCursor (0,10);
      //display.print("A");
      Serial.println("A");
    }
    else
    {
      arriba();
    }
    

    if(bSelect==true) //Select
    {
      Serial.println("Select");
      select();
      //display.setCursor (0,10);
      //display.print("B");
      
    }

    if(bStart==true) //Sstart
    {
      Serial.println("Start");
      bstart();
      //display.setCursor (0,10);
      //display.print("B");
    }
    
}

// -------------------------------------------------------------------------- sinmovi
void sinmovi()
{
  
  analogWrite(inputA1,0);
  analogWrite(inputA2,0);
  analogWrite(inputB1,0);
  analogWrite(inputB2,0);
  
}

// -------------------------------------------------------------------------- adelante
void adelante()
{
  analogWrite(inputA1,pwm);
  analogWrite(inputA2,0);
  analogWrite(inputB1,0);
  analogWrite(inputB2,pwm);
 
}

// -------------------------------------------------------------------------- atras
void atras()
{
  analogWrite(inputA1,0);
  analogWrite(inputA2,pwm);
  analogWrite(inputB1,pwm);
  analogWrite(inputB2,0);
}

// -------------------------------------------------------------------------- derecha
void derecha()
{
 analogWrite(inputA1,pwm);
  analogWrite(inputA2,0);
  analogWrite(inputB1,pwm);
  analogWrite(inputB2,0); 
}

// -------------------------------------------------------------------------- izquiereda
void izquierda()
{

  analogWrite(inputA1,0);
  analogWrite(inputA2,pwm);
  analogWrite(inputB1,0);
  analogWrite(inputB2,pwm);
}

// -------------------------------------------------------------------------- abajo
void abajo()
{
  brazo.write(pos_min);
  brazo2.write(pos_max);
  /*for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,corazon[i]);
  }*/
  //delay(1000);
}

// -------------------------------------------------------------------------- arriba
void arriba()
{
  brazo.write(pos_max);
  brazo2.write(pos_min);
  /*for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,corazon[i]);
  }*/
  //delay(1000);
}


// -------------------------------------------------------------------------- select
void select()
{
  rainbowFade2White(0, 1, 0);
  //pulseWhite(50);
}


// -------------------------------------------------------------------------- start
void bstart()
{
  whiteOverRainbow(75, 5);  
}

//---------------------------------
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<strip.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }

    strip.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }
}

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    strip.show();
    delay(wait);

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } 
    else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  //for(int k=0; k<whiteLoops; k++) {
  //  for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
  //    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
  //    strip.show();
  //  }
    //delay(1000); // Pause 1 second
    //for(int j=255; j>=0; j--) { // Ramp down 255 to 0
    //  strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    //  strip.show();
    //}
  //}

  //delay(500); // Pause 1/2 second
}
