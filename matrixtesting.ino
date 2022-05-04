//www.github.com/aimarket
//used on a LED Matrix with a total of 256 LEDS

#include <Adafruit_NeoPixel.h>
#include <IRremote.h>

#define LED_PIN 5
#define LED_COUNT 256
int RECV_PIN = 4; // the pin where you connect the output pin of IR sensor 

SemaphoreHandle_t Semaphore; //Flag signal
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
IRrecv irrecv(RECV_PIN); //recive IR signals
decode_results results; //Decoded IR signals

uint32_t colors[10]={
      strip.Color(255,255,255), //white
      strip.Color(255,0,0), // red
      strip.Color(0,255,0), // green
      strip.Color(0,0,255), // blue
      strip.Color(0,255,255), // cyan
      strip.Color(255,0,255), // pink
      strip.Color(255,255,0), // yellow
      strip.Color(255,150,0), // orange
      strip.Color(175,255,0), // yellow green
      strip.Color(100,0,255), // violet
      
    };
bool loopit = true;
static volatile unsigned long code = 4; //global variable
static volatile unsigned long rate = 20; //global variable
static volatile unsigned long colorPick = 3; //global variable

void setup() {
  Semaphore = xSemaphoreCreateMutex();
  irrecv.enableIRIn();
  pinMode(21, OUTPUT);
  digitalWrite(21,LOW);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(5); // Set BRIGHTNESS to about 1/5 (max = 255)

  xTaskCreate(
                    producer,            /* Task function. */
                    "Producer",       /* String with name of task. */
                    10000,            /* Stack size in words. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */
 
     xTaskCreate(
                    consumer,            /* Task function. */
                    "Consumer",       /* String with name of task. */
                    10000,            /* Stack size in words. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */
}

void producer(void * parameter) {
  while(true){
  if (irrecv.decode(&results))// Returns 0 if no data ready, 1 if data ready.     
{     
  

 int oldCode;
 if(results.value != -1){
  oldCode=results.value;
 }
 switch (oldCode){
    case 16753245:// 1
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=1;
    xSemaphoreGive(Semaphore);
    break;
    case 16736925:// 2
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=2;
    xSemaphoreGive(Semaphore);
    break;
    case 16769565:// 3
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=3;
    xSemaphoreGive(Semaphore);
    break;
    case 16720605: // 4
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=4;
    xSemaphoreGive(Semaphore);
    break;
    case 16712445: // 5
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=5;
    xSemaphoreGive(Semaphore);
    break;
    case 16761405: // 6
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=6;
    xSemaphoreGive(Semaphore);
    break;
    case 16769055: // 7
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=7;
    xSemaphoreGive(Semaphore);
    break;
    case 16754775: // 8
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=8;
    xSemaphoreGive(Semaphore);
    break;
    case 16748655: // 9
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=9;
    xSemaphoreGive(Semaphore);
    break;
    case 16750695: // 0
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=0;
    xSemaphoreGive(Semaphore);
    break;
    case 16738455:// *
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=10;
    xSemaphoreGive(Semaphore);
    break;
    case 16716015:// <
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    if(colorPick-1==-1){
      colorPick=9;
    }else{
      colorPick--;
    }
    xSemaphoreGive(Semaphore);
    break;
    case 16734885:// >
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    if(colorPick+1==10){
      colorPick=0;
    }else{
      colorPick++;
    }
    xSemaphoreGive(Semaphore);
    break;
    case 16718055: //^
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    if(rate>1){
      rate--;
    }
    xSemaphoreGive(Semaphore);
    break;     
    case 16730805: //v
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    if(rate<100){
      rate++;
    }
    xSemaphoreGive(Semaphore);
    break;
    case 16756815://#
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=11;
    xSemaphoreGive(Semaphore);
    break;
    case 16726215://ok
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    code=12;
    xSemaphoreGive(Semaphore);
    break;
 }
 irrecv.resume(); // Restart the ISR state machine and Receive the next value 
}
  delay(5);
  }
}

void consumer(void * parameter) {
  while(true){
   // xSemaphoreTake(Semaphore, portMAX_DELAY);  
    long tempCode = code;
  switch(tempCode){
    
    case 1:
      clearLED();
      upStrob(colors[colorPick],tempCode);
      if(loopit){
        
      }
      break;
    case 2:
      clearLED();
      colorWipe(colors[colorPick],tempCode);
      break;
    case 3:
      clearLED();
      lineChase(colors[colorPick],tempCode);
      break;
    case 4: 
      clearLED();
      colorvsColor(tempCode);
      break;
    case 5:
      clearLED();
      stars(tempCode);
      break;
    case 6:
      clearLED();
      rainbow(tempCode);
      break;
    case 7:
      clearLED();
      gfx(tempCode);
      break;
    case 8:
      clearLED();
      ballGFX();
      break;
    case 9:
      clearLED();
      meteorRain(colors[colorPick],20, 20, true,tempCode);
      break;
    case 0:
      clearLED();
      pacman();
      break;
    case 10:
      clearLED();
      digitalWrite(21, LOW);
      break;
    case 11:
      digitalWrite(21, HIGH);
      break;
    case 12:
      clearLED();
      solid(colors[colorPick],tempCode);
      break;
  }
 // xSemaphoreGive(Semaphore);
  delay(10);
  }
}

void loop()
{
    vTaskDelete (NULL);
}
void loopIt() {
  functions[]
}
void ballPolice(){
  ballMapBlue(0,2,0,0,10);ballMapBlue(0,3,0,0,10);ballMapBlue(0,11,0,0,10);ballMapBlue(0,12,0,0,10);ballMapBlue(7,7,0,0,10);ballMapBlue(7,8,0,0,10);ballMapBlue(0,23,0,0,10);ballMapBlue(0,24,0,0,10);ballMapBlue(7,18,0,0,10);ballMapBlue(7,19,0,0,10);ballMapBlue(7,27,0,0,10);ballMapBlue(7,28,0,0,10);
ballColorStep();
ballMapRed(0,2,0,0,10);ballMapRed(0,3,0,0,10);ballMapRed(0,11,0,0,10);ballMapRed(0,12,0,0,10);ballMapRed(7,7,0,0,10);ballMapRed(7,8,0,0,10);ballMapRed(0,23,0,0,10);ballMapRed(0,24,0,0,10);ballMapRed(7,18,0,0,10);ballMapRed(7,19,0,0,10);ballMapRed(7,27,0,0,10);ballMapRed(7,28,0,0,10);
ballStep();
ballMapBlue(0,1,0,0,10);ballMapBlue(0,2,0,0,10);ballMapBlue(0,3,0,0,10);ballMapBlue(0,4,0,0,10);ballMapBlue(0,10,0,0,10);ballMapBlue(0,11,0,0,10);ballMapBlue(0,12,0,0,10);ballMapBlue(0,13,0,0,10);ballMapBlue(1,2,0,0,10);ballMapBlue(1,3,0,0,10);ballMapBlue(1,11,0,0,10);ballMapBlue(1,12,0,0,10);ballMapBlue(6,7,0,0,10);ballMapBlue(6,8,0,0,10);ballMapBlue(7,6,0,0,10);ballMapBlue(7,7,0,0,10);ballMapBlue(7,8,0,0,10);ballMapBlue(7,9,0,0,10);ballMapBlue(0,22,0,0,10);ballMapBlue(0,23,0,0,10);ballMapBlue(0,24,0,0,10);ballMapBlue(0,25,0,0,10);ballMapBlue(1,23,0,0,10);ballMapBlue(1,24,0,0,10);ballMapBlue(6,18,0,0,10);ballMapBlue(6,19,0,0,10);ballMapBlue(6,27,0,0,10);ballMapBlue(6,28,0,0,10);ballMapBlue(7,17,0,0,10);ballMapBlue(7,18,0,0,10);ballMapBlue(7,19,0,0,10);ballMapBlue(7,20,0,0,10);ballMapBlue(7,26,0,0,10);ballMapBlue(7,27,0,0,10);ballMapBlue(7,28,0,0,10);ballMapBlue(7,29,0,0,10);
ballColorStep();
ballMapRed(0,1,0,0,10);ballMapRed(0,2,0,0,10);ballMapRed(0,3,0,0,10);ballMapRed(0,4,0,0,10);ballMapRed(0,10,0,0,10);ballMapRed(0,11,0,0,10);ballMapRed(0,12,0,0,10);ballMapRed(0,13,0,0,10);ballMapRed(1,2,0,0,10);ballMapRed(1,3,0,0,10);ballMapRed(1,11,0,0,10);ballMapRed(1,12,0,0,10);ballMapRed(6,7,0,0,10);ballMapRed(6,8,0,0,10);ballMapRed(7,6,0,0,10);ballMapRed(7,7,0,0,10);ballMapRed(7,8,0,0,10);ballMapRed(7,9,0,0,10);ballMapRed(0,22,0,0,10);ballMapRed(0,23,0,0,10);ballMapRed(0,24,0,0,10);ballMapRed(0,25,0,0,10);ballMapRed(1,23,0,0,10);ballMapRed(1,24,0,0,10);ballMapRed(6,18,0,0,10);ballMapRed(6,19,0,0,10);ballMapRed(6,27,0,0,10);ballMapRed(6,28,0,0,10);ballMapRed(7,17,0,0,10);ballMapRed(7,18,0,0,10);ballMapRed(7,19,0,0,10);ballMapRed(7,20,0,0,10);ballMapRed(7,26,0,0,10);ballMapRed(7,27,0,0,10);ballMapRed(7,28,0,0,10);ballMapRed(7,29,0,0,10);
ballStep();
ballMapBlue(0,1,0,0,10);ballMapBlue(0,2,0,0,10);ballMapBlue(0,3,0,0,10);ballMapBlue(0,4,0,0,10);ballMapBlue(0,10,0,0,10);ballMapBlue(0,11,0,0,10);ballMapBlue(0,12,0,0,10);ballMapBlue(0,13,0,0,10);ballMapBlue(1,1,0,0,10);ballMapBlue(1,2,0,0,10);ballMapBlue(1,3,0,0,10);ballMapBlue(1,4,0,0,10);ballMapBlue(1,10,0,0,10);ballMapBlue(1,11,0,0,10);ballMapBlue(1,12,0,0,10);ballMapBlue(1,13,0,0,10);ballMapBlue(2,2,0,0,10);ballMapBlue(2,3,0,0,10);ballMapBlue(2,11,0,0,10);ballMapBlue(2,12,0,0,10);ballMapBlue(5,7,0,0,10);ballMapBlue(5,8,0,0,10);ballMapBlue(6,6,0,0,10);ballMapBlue(6,7,0,0,10);ballMapBlue(6,8,0,0,10);ballMapBlue(6,9,0,0,10);ballMapBlue(7,6,0,0,10);ballMapBlue(7,7,0,0,10);ballMapBlue(7,8,0,0,10);ballMapBlue(7,9,0,0,10);ballMapBlue(0,22,0,0,10);ballMapBlue(0,23,0,0,10);ballMapBlue(0,24,0,0,10);ballMapBlue(0,25,0,0,10);ballMapBlue(1,22,0,0,10);ballMapBlue(1,23,0,0,10);ballMapBlue(1,24,0,0,10);ballMapBlue(1,25,0,0,10);ballMapBlue(2,23,0,0,10);ballMapBlue(2,24,0,0,10);ballMapBlue(5,18,0,0,10);ballMapBlue(5,19,0,0,10);ballMapBlue(5,27,0,0,10);ballMapBlue(5,28,0,0,10);ballMapBlue(6,17,0,0,10);ballMapBlue(6,18,0,0,10);ballMapBlue(6,19,0,0,10);ballMapBlue(6,20,0,0,10);ballMapBlue(6,26,0,0,10);ballMapBlue(6,27,0,0,10);ballMapBlue(6,28,0,0,10);ballMapBlue(6,29,0,0,10);ballMapBlue(7,17,0,0,10);ballMapBlue(7,18,0,0,10);ballMapBlue(7,19,0,0,10);ballMapBlue(7,20,0,0,10);ballMapBlue(7,26,0,0,10);ballMapBlue(7,27,0,0,10);ballMapBlue(7,28,0,0,10);ballMapBlue(7,29,0,0,10);
ballColorStep();
ballMapRed(0,1,0,0,10);ballMapRed(0,2,0,0,10);ballMapRed(0,3,0,0,10);ballMapRed(0,4,0,0,10);ballMapRed(0,10,0,0,10);ballMapRed(0,11,0,0,10);ballMapRed(0,12,0,0,10);ballMapRed(0,13,0,0,10);ballMapRed(1,1,0,0,10);ballMapRed(1,2,0,0,10);ballMapRed(1,3,0,0,10);ballMapRed(1,4,0,0,10);ballMapRed(1,10,0,0,10);ballMapRed(1,11,0,0,10);ballMapRed(1,12,0,0,10);ballMapRed(1,13,0,0,10);ballMapRed(2,2,0,0,10);ballMapRed(2,3,0,0,10);ballMapRed(2,11,0,0,10);ballMapRed(2,12,0,0,10);ballMapRed(5,7,0,0,10);ballMapRed(5,8,0,0,10);ballMapRed(6,6,0,0,10);ballMapRed(6,7,0,0,10);ballMapRed(6,8,0,0,10);ballMapRed(6,9,0,0,10);ballMapRed(7,6,0,0,10);ballMapRed(7,7,0,0,10);ballMapRed(7,8,0,0,10);ballMapRed(7,9,0,0,10);ballMapRed(0,22,0,0,10);ballMapRed(0,23,0,0,10);ballMapRed(0,24,0,0,10);ballMapRed(0,25,0,0,10);ballMapRed(1,22,0,0,10);ballMapRed(1,23,0,0,10);ballMapRed(1,24,0,0,10);ballMapRed(1,25,0,0,10);ballMapRed(2,23,0,0,10);ballMapRed(2,24,0,0,10);ballMapRed(5,18,0,0,10);ballMapRed(5,19,0,0,10);ballMapRed(5,27,0,0,10);ballMapRed(5,28,0,0,10);ballMapRed(6,17,0,0,10);ballMapRed(6,18,0,0,10);ballMapRed(6,19,0,0,10);ballMapRed(6,20,0,0,10);ballMapRed(6,26,0,0,10);ballMapRed(6,27,0,0,10);ballMapRed(6,28,0,0,10);ballMapRed(6,29,0,0,10);ballMapRed(7,17,0,0,10);ballMapRed(7,18,0,0,10);ballMapRed(7,19,0,0,10);ballMapRed(7,20,0,0,10);ballMapRed(7,26,0,0,10);ballMapRed(7,27,0,0,10);ballMapRed(7,28,0,0,10);ballMapRed(7,29,0,0,10);
ballStep();
ballMapBlue(0,2,0,0,10);ballMapBlue(0,3,0,0,10);ballMapBlue(0,11,0,0,10);ballMapBlue(0,12,0,0,10);ballMapBlue(1,1,0,0,10);ballMapBlue(1,2,0,0,10);ballMapBlue(1,3,0,0,10);ballMapBlue(1,4,0,0,10);ballMapBlue(1,10,0,0,10);ballMapBlue(1,11,0,0,10);ballMapBlue(1,12,0,0,10);ballMapBlue(1,13,0,0,10);ballMapBlue(2,1,0,0,10);ballMapBlue(2,2,0,0,10);ballMapBlue(2,3,0,0,10);ballMapBlue(2,4,0,0,10);ballMapBlue(2,10,0,0,10);ballMapBlue(2,11,0,0,10);ballMapBlue(2,12,0,0,10);ballMapBlue(2,13,0,0,10);ballMapBlue(3,2,0,0,10);ballMapBlue(3,3,0,0,10);ballMapBlue(3,11,0,0,10);ballMapBlue(3,12,0,0,10);ballMapBlue(4,7,0,0,10);ballMapBlue(4,8,0,0,10);ballMapBlue(5,6,0,0,10);ballMapBlue(5,7,0,0,10);ballMapBlue(5,8,0,0,10);ballMapBlue(5,9,0,0,10);ballMapBlue(6,6,0,0,10);ballMapBlue(6,7,0,0,10);ballMapBlue(6,8,0,0,10);ballMapBlue(6,9,0,0,10);ballMapBlue(7,7,0,0,10);ballMapBlue(7,8,0,0,10);ballMapBlue(0,23,0,0,10);ballMapBlue(0,24,0,0,10);ballMapBlue(1,22,0,0,10);ballMapBlue(1,23,0,0,10);ballMapBlue(1,24,0,0,10);ballMapBlue(1,25,0,0,10);ballMapBlue(2,22,0,0,10);ballMapBlue(2,23,0,0,10);ballMapBlue(2,24,0,0,10);ballMapBlue(2,25,0,0,10);ballMapBlue(3,23,0,0,10);ballMapBlue(3,24,0,0,10);ballMapBlue(4,18,0,0,10);ballMapBlue(4,19,0,0,10);ballMapBlue(4,27,0,0,10);ballMapBlue(4,28,0,0,10);ballMapBlue(5,17,0,0,10);ballMapBlue(5,18,0,0,10);ballMapBlue(5,19,0,0,10);ballMapBlue(5,20,0,0,10);ballMapBlue(5,26,0,0,10);ballMapBlue(5,27,0,0,10);ballMapBlue(5,28,0,0,10);ballMapBlue(5,29,0,0,10);ballMapBlue(6,17,0,0,10);ballMapBlue(6,18,0,0,10);ballMapBlue(6,19,0,0,10);ballMapBlue(6,20,0,0,10);ballMapBlue(6,26,0,0,10);ballMapBlue(6,27,0,0,10);ballMapBlue(6,28,0,0,10);ballMapBlue(6,29,0,0,10);ballMapBlue(7,18,0,0,10);ballMapBlue(7,19,0,0,10);ballMapBlue(7,27,0,0,10);ballMapBlue(7,28,0,0,10);
ballColorStep();
ballMapRed(0,2,0,0,10);ballMapRed(0,3,0,0,10);ballMapRed(0,11,0,0,10);ballMapRed(0,12,0,0,10);ballMapRed(1,1,0,0,10);ballMapRed(1,2,0,0,10);ballMapRed(1,3,0,0,10);ballMapRed(1,4,0,0,10);ballMapRed(1,10,0,0,10);ballMapRed(1,11,0,0,10);ballMapRed(1,12,0,0,10);ballMapRed(1,13,0,0,10);ballMapRed(2,1,0,0,10);ballMapRed(2,2,0,0,10);ballMapRed(2,3,0,0,10);ballMapRed(2,4,0,0,10);ballMapRed(2,10,0,0,10);ballMapRed(2,11,0,0,10);ballMapRed(2,12,0,0,10);ballMapRed(2,13,0,0,10);ballMapRed(3,2,0,0,10);ballMapRed(3,3,0,0,10);ballMapRed(3,11,0,0,10);ballMapRed(3,12,0,0,10);ballMapRed(4,7,0,0,10);ballMapRed(4,8,0,0,10);ballMapRed(5,6,0,0,10);ballMapRed(5,7,0,0,10);ballMapRed(5,8,0,0,10);ballMapRed(5,9,0,0,10);ballMapRed(6,6,0,0,10);ballMapRed(6,7,0,0,10);ballMapRed(6,8,0,0,10);ballMapRed(6,9,0,0,10);ballMapRed(7,7,0,0,10);ballMapRed(7,8,0,0,10);ballMapRed(0,23,0,0,10);ballMapRed(0,24,0,0,10);ballMapRed(1,22,0,0,10);ballMapRed(1,23,0,0,10);ballMapRed(1,24,0,0,10);ballMapRed(1,25,0,0,10);ballMapRed(2,22,0,0,10);ballMapRed(2,23,0,0,10);ballMapRed(2,24,0,0,10);ballMapRed(2,25,0,0,10);ballMapRed(3,23,0,0,10);ballMapRed(3,24,0,0,10);ballMapRed(4,18,0,0,10);ballMapRed(4,19,0,0,10);ballMapRed(4,27,0,0,10);ballMapRed(4,28,0,0,10);ballMapRed(5,17,0,0,10);ballMapRed(5,18,0,0,10);ballMapRed(5,19,0,0,10);ballMapRed(5,20,0,0,10);ballMapRed(5,26,0,0,10);ballMapRed(5,27,0,0,10);ballMapRed(5,28,0,0,10);ballMapRed(5,29,0,0,10);ballMapRed(6,17,0,0,10);ballMapRed(6,18,0,0,10);ballMapRed(6,19,0,0,10);ballMapRed(6,20,0,0,10);ballMapRed(6,26,0,0,10);ballMapRed(6,27,0,0,10);ballMapRed(6,28,0,0,10);ballMapRed(6,29,0,0,10);ballMapRed(7,18,0,0,10);ballMapRed(7,19,0,0,10);ballMapRed(7,27,0,0,10);ballMapRed(7,28,0,0,10);
ballStep();
ballMapBlue(1,2,0,0,10);ballMapBlue(1,3,0,0,10);ballMapBlue(1,11,0,0,10);ballMapBlue(1,12,0,0,10);ballMapBlue(2,1,0,0,10);ballMapBlue(2,2,0,0,10);ballMapBlue(2,3,0,0,10);ballMapBlue(2,4,0,0,10);ballMapBlue(2,10,0,0,10);ballMapBlue(2,11,0,0,10);ballMapBlue(2,12,0,0,10);ballMapBlue(2,13,0,0,10);ballMapBlue(3,1,0,0,10);ballMapBlue(3,2,0,0,10);ballMapBlue(3,3,0,0,10);ballMapBlue(3,4,0,0,10);ballMapBlue(3,7,0,0,10);ballMapBlue(3,8,0,0,10);ballMapBlue(3,10,0,0,10);ballMapBlue(3,11,0,0,10);ballMapBlue(3,12,0,0,10);ballMapBlue(3,13,0,0,10);ballMapBlue(4,2,0,0,10);ballMapBlue(4,3,0,0,10);ballMapBlue(4,6,0,0,10);ballMapBlue(4,7,0,0,10);ballMapBlue(4,8,0,0,10);ballMapBlue(4,9,0,0,10);ballMapBlue(4,11,0,0,10);ballMapBlue(4,12,0,0,10);ballMapBlue(5,6,0,0,10);ballMapBlue(5,7,0,0,10);ballMapBlue(5,8,0,0,10);ballMapBlue(5,9,0,0,10);ballMapBlue(6,7,0,0,10);ballMapBlue(6,8,0,0,10);ballMapBlue(1,23,0,0,10);ballMapBlue(1,24,0,0,10);ballMapBlue(2,22,0,0,10);ballMapBlue(2,23,0,0,10);ballMapBlue(2,24,0,0,10);ballMapBlue(2,25,0,0,10);ballMapBlue(3,18,0,0,10);ballMapBlue(3,19,0,0,10);ballMapBlue(3,22,0,0,10);ballMapBlue(3,23,0,0,10);ballMapBlue(3,24,0,0,10);ballMapBlue(3,25,0,0,10);ballMapBlue(3,27,0,0,10);ballMapBlue(3,28,0,0,10);ballMapBlue(4,17,0,0,10);ballMapBlue(4,18,0,0,10);ballMapBlue(4,19,0,0,10);ballMapBlue(4,20,0,0,10);ballMapBlue(4,23,0,0,10);ballMapBlue(4,24,0,0,10);ballMapBlue(4,26,0,0,10);ballMapBlue(4,27,0,0,10);ballMapBlue(4,28,0,0,10);ballMapBlue(4,29,0,0,10);ballMapBlue(5,17,0,0,10);ballMapBlue(5,18,0,0,10);ballMapBlue(5,19,0,0,10);ballMapBlue(5,20,0,0,10);ballMapBlue(5,26,0,0,10);ballMapBlue(5,27,0,0,10);ballMapBlue(5,28,0,0,10);ballMapBlue(5,29,0,0,10);ballMapBlue(6,18,0,0,10);ballMapBlue(6,19,0,0,10);ballMapBlue(6,27,0,0,10);ballMapBlue(6,28,0,0,10);
ballColorStep();
ballMapRed(1,2,0,0,10);ballMapRed(1,3,0,0,10);ballMapRed(1,11,0,0,10);ballMapRed(1,12,0,0,10);ballMapRed(2,1,0,0,10);ballMapRed(2,2,0,0,10);ballMapRed(2,3,0,0,10);ballMapRed(2,4,0,0,10);ballMapRed(2,10,0,0,10);ballMapRed(2,11,0,0,10);ballMapRed(2,12,0,0,10);ballMapRed(2,13,0,0,10);ballMapRed(3,1,0,0,10);ballMapRed(3,2,0,0,10);ballMapRed(3,3,0,0,10);ballMapRed(3,4,0,0,10);ballMapRed(3,7,0,0,10);ballMapRed(3,8,0,0,10);ballMapRed(3,10,0,0,10);ballMapRed(3,11,0,0,10);ballMapRed(3,12,0,0,10);ballMapRed(3,13,0,0,10);ballMapRed(4,2,0,0,10);ballMapRed(4,3,0,0,10);ballMapRed(4,6,0,0,10);ballMapRed(4,7,0,0,10);ballMapRed(4,8,0,0,10);ballMapRed(4,9,0,0,10);ballMapRed(4,11,0,0,10);ballMapRed(4,12,0,0,10);ballMapRed(5,6,0,0,10);ballMapRed(5,7,0,0,10);ballMapRed(5,8,0,0,10);ballMapRed(5,9,0,0,10);ballMapRed(6,7,0,0,10);ballMapRed(6,8,0,0,10);ballMapRed(1,23,0,0,10);ballMapRed(1,24,0,0,10);ballMapRed(2,22,0,0,10);ballMapRed(2,23,0,0,10);ballMapRed(2,24,0,0,10);ballMapRed(2,25,0,0,10);ballMapRed(3,18,0,0,10);ballMapRed(3,19,0,0,10);ballMapRed(3,22,0,0,10);ballMapRed(3,23,0,0,10);ballMapRed(3,24,0,0,10);ballMapRed(3,25,0,0,10);ballMapRed(3,27,0,0,10);ballMapRed(3,28,0,0,10);ballMapRed(4,17,0,0,10);ballMapRed(4,18,0,0,10);ballMapRed(4,19,0,0,10);ballMapRed(4,20,0,0,10);ballMapRed(4,23,0,0,10);ballMapRed(4,24,0,0,10);ballMapRed(4,26,0,0,10);ballMapRed(4,27,0,0,10);ballMapRed(4,28,0,0,10);ballMapRed(4,29,0,0,10);ballMapRed(5,17,0,0,10);ballMapRed(5,18,0,0,10);ballMapRed(5,19,0,0,10);ballMapRed(5,20,0,0,10);ballMapRed(5,26,0,0,10);ballMapRed(5,27,0,0,10);ballMapRed(5,28,0,0,10);ballMapRed(5,29,0,0,10);ballMapRed(6,18,0,0,10);ballMapRed(6,19,0,0,10);ballMapRed(6,27,0,0,10);ballMapRed(6,28,0,0,10);
ballStep();
ballMapBlue(2,2,0,0,10);ballMapBlue(2,3,0,0,10);ballMapBlue(2,7,0,0,10);ballMapBlue(2,8,0,0,10);ballMapBlue(2,11,0,0,10);ballMapBlue(2,12,0,0,10);ballMapBlue(3,1,0,0,10);ballMapBlue(3,2,0,0,10);ballMapBlue(3,3,0,0,10);ballMapBlue(3,4,0,0,10);ballMapBlue(3,6,0,0,10);ballMapBlue(3,7,0,0,10);ballMapBlue(3,8,0,0,10);ballMapBlue(3,9,0,0,10);ballMapBlue(3,10,0,0,10);ballMapBlue(3,11,0,0,10);ballMapBlue(3,12,0,0,10);ballMapBlue(3,13,0,0,10);ballMapBlue(4,1,0,0,10);ballMapBlue(4,2,0,0,10);ballMapBlue(4,3,0,0,10);ballMapBlue(4,4,0,0,10);ballMapBlue(4,6,0,0,10);ballMapBlue(4,7,0,0,10);ballMapBlue(4,8,0,0,10);ballMapBlue(4,9,0,0,10);ballMapBlue(4,10,0,0,10);ballMapBlue(4,11,0,0,10);ballMapBlue(4,12,0,0,10);ballMapBlue(4,13,0,0,10);ballMapBlue(5,2,0,0,10);ballMapBlue(5,3,0,0,10);ballMapBlue(5,7,0,0,10);ballMapBlue(5,8,0,0,10);ballMapBlue(5,11,0,0,10);ballMapBlue(5,12,0,0,10);ballMapBlue(2,18,0,0,10);ballMapBlue(2,19,0,0,10);ballMapBlue(2,23,0,0,10);ballMapBlue(2,24,0,0,10);ballMapBlue(2,27,0,0,10);ballMapBlue(2,28,0,0,10);ballMapBlue(3,17,0,0,10);ballMapBlue(3,18,0,0,10);ballMapBlue(3,19,0,0,10);ballMapBlue(3,20,0,0,10);ballMapBlue(3,22,0,0,10);ballMapBlue(3,23,0,0,10);ballMapBlue(3,24,0,0,10);ballMapBlue(3,25,0,0,10);ballMapBlue(3,26,0,0,10);ballMapBlue(3,27,0,0,10);ballMapBlue(3,28,0,0,10);ballMapBlue(3,29,0,0,10);ballMapBlue(4,17,0,0,10);ballMapBlue(4,18,0,0,10);ballMapBlue(4,19,0,0,10);ballMapBlue(4,20,0,0,10);ballMapBlue(4,22,0,0,10);ballMapBlue(4,23,0,0,10);ballMapBlue(4,24,0,0,10);ballMapBlue(4,25,0,0,10);ballMapBlue(4,26,0,0,10);ballMapBlue(4,27,0,0,10);ballMapBlue(4,28,0,0,10);ballMapBlue(4,29,0,0,10);ballMapBlue(5,18,0,0,10);ballMapBlue(5,19,0,0,10);ballMapBlue(5,23,0,0,10);ballMapBlue(5,24,0,0,10);ballMapBlue(5,27,0,0,10);ballMapBlue(5,28,0,0,10);
ballColorStep();
ballMapRed(2,2,0,0,10);ballMapRed(2,3,0,0,10);ballMapRed(2,7,0,0,10);ballMapRed(2,8,0,0,10);ballMapRed(2,11,0,0,10);ballMapRed(2,12,0,0,10);ballMapRed(3,1,0,0,10);ballMapRed(3,2,0,0,10);ballMapRed(3,3,0,0,10);ballMapRed(3,4,0,0,10);ballMapRed(3,6,0,0,10);ballMapRed(3,7,0,0,10);ballMapRed(3,8,0,0,10);ballMapRed(3,9,0,0,10);ballMapRed(3,10,0,0,10);ballMapRed(3,11,0,0,10);ballMapRed(3,12,0,0,10);ballMapRed(3,13,0,0,10);ballMapRed(4,1,0,0,10);ballMapRed(4,2,0,0,10);ballMapRed(4,3,0,0,10);ballMapRed(4,4,0,0,10);ballMapRed(4,6,0,0,10);ballMapRed(4,7,0,0,10);ballMapRed(4,8,0,0,10);ballMapRed(4,9,0,0,10);ballMapRed(4,10,0,0,10);ballMapRed(4,11,0,0,10);ballMapRed(4,12,0,0,10);ballMapRed(4,13,0,0,10);ballMapRed(5,2,0,0,10);ballMapRed(5,3,0,0,10);ballMapRed(5,7,0,0,10);ballMapRed(5,8,0,0,10);ballMapRed(5,11,0,0,10);ballMapRed(5,12,0,0,10);ballMapRed(2,18,0,0,10);ballMapRed(2,19,0,0,10);ballMapRed(2,23,0,0,10);ballMapRed(2,24,0,0,10);ballMapRed(2,27,0,0,10);ballMapRed(2,28,0,0,10);ballMapRed(3,17,0,0,10);ballMapRed(3,18,0,0,10);ballMapRed(3,19,0,0,10);ballMapRed(3,20,0,0,10);ballMapRed(3,22,0,0,10);ballMapRed(3,23,0,0,10);ballMapRed(3,24,0,0,10);ballMapRed(3,25,0,0,10);ballMapRed(3,26,0,0,10);ballMapRed(3,27,0,0,10);ballMapRed(3,28,0,0,10);ballMapRed(3,29,0,0,10);ballMapRed(4,17,0,0,10);ballMapRed(4,18,0,0,10);ballMapRed(4,19,0,0,10);ballMapRed(4,20,0,0,10);ballMapRed(4,22,0,0,10);ballMapRed(4,23,0,0,10);ballMapRed(4,24,0,0,10);ballMapRed(4,25,0,0,10);ballMapRed(4,26,0,0,10);ballMapRed(4,27,0,0,10);ballMapRed(4,28,0,0,10);ballMapRed(4,29,0,0,10);ballMapRed(5,18,0,0,10);ballMapRed(5,19,0,0,10);ballMapRed(5,23,0,0,10);ballMapRed(5,24,0,0,10);ballMapRed(5,27,0,0,10);ballMapRed(5,28,0,0,10);
ballStep();
ballMapBlue(1,7,0,0,10);ballMapBlue(1,8,0,0,10);ballMapBlue(2,6,0,0,10);ballMapBlue(2,7,0,0,10);ballMapBlue(2,8,0,0,10);ballMapBlue(2,9,0,0,10);ballMapBlue(3,2,0,0,10);ballMapBlue(3,3,0,0,10);ballMapBlue(3,6,0,0,10);ballMapBlue(3,7,0,0,10);ballMapBlue(3,8,0,0,10);ballMapBlue(3,9,0,0,10);ballMapBlue(3,11,0,0,10);ballMapBlue(3,12,0,0,10);ballMapBlue(4,1,0,0,10);ballMapBlue(4,2,0,0,10);ballMapBlue(4,3,0,0,10);ballMapBlue(4,4,0,0,10);ballMapBlue(4,7,0,0,10);ballMapBlue(4,8,0,0,10);ballMapBlue(4,10,0,0,10);ballMapBlue(4,11,0,0,10);ballMapBlue(4,12,0,0,10);ballMapBlue(4,13,0,0,10);ballMapBlue(5,1,0,0,10);ballMapBlue(5,2,0,0,10);ballMapBlue(5,3,0,0,10);ballMapBlue(5,4,0,0,10);ballMapBlue(5,10,0,0,10);ballMapBlue(5,11,0,0,10);ballMapBlue(5,12,0,0,10);ballMapBlue(5,13,0,0,10);ballMapBlue(6,2,0,0,10);ballMapBlue(6,3,0,0,10);ballMapBlue(6,11,0,0,10);ballMapBlue(6,12,0,0,10);ballMapBlue(1,18,0,0,10);ballMapBlue(1,19,0,0,10);ballMapBlue(1,27,0,0,10);ballMapBlue(1,28,0,0,10);ballMapBlue(2,17,0,0,10);ballMapBlue(2,18,0,0,10);ballMapBlue(2,19,0,0,10);ballMapBlue(2,20,0,0,10);ballMapBlue(2,26,0,0,10);ballMapBlue(2,27,0,0,10);ballMapBlue(2,28,0,0,10);ballMapBlue(2,29,0,0,10);ballMapBlue(3,17,0,0,10);ballMapBlue(3,18,0,0,10);ballMapBlue(3,19,0,0,10);ballMapBlue(3,20,0,0,10);ballMapBlue(3,23,0,0,10);ballMapBlue(3,24,0,0,10);ballMapBlue(3,26,0,0,10);ballMapBlue(3,27,0,0,10);ballMapBlue(3,28,0,0,10);ballMapBlue(3,29,0,0,10);ballMapBlue(4,18,0,0,10);ballMapBlue(4,19,0,0,10);ballMapBlue(4,22,0,0,10);ballMapBlue(4,23,0,0,10);ballMapBlue(4,24,0,0,10);ballMapBlue(4,25,0,0,10);ballMapBlue(4,27,0,0,10);ballMapBlue(4,28,0,0,10);ballMapBlue(5,22,0,0,10);ballMapBlue(5,23,0,0,10);ballMapBlue(5,24,0,0,10);ballMapBlue(5,25,0,0,10);ballMapBlue(6,23,0,0,10);ballMapBlue(6,24,0,0,10);
ballColorStep();
ballMapRed(1,7,0,0,10);ballMapRed(1,8,0,0,10);ballMapRed(2,6,0,0,10);ballMapRed(2,7,0,0,10);ballMapRed(2,8,0,0,10);ballMapRed(2,9,0,0,10);ballMapRed(3,2,0,0,10);ballMapRed(3,3,0,0,10);ballMapRed(3,6,0,0,10);ballMapRed(3,7,0,0,10);ballMapRed(3,8,0,0,10);ballMapRed(3,9,0,0,10);ballMapRed(3,11,0,0,10);ballMapRed(3,12,0,0,10);ballMapRed(4,1,0,0,10);ballMapRed(4,2,0,0,10);ballMapRed(4,3,0,0,10);ballMapRed(4,4,0,0,10);ballMapRed(4,7,0,0,10);ballMapRed(4,8,0,0,10);ballMapRed(4,10,0,0,10);ballMapRed(4,11,0,0,10);ballMapRed(4,12,0,0,10);ballMapRed(4,13,0,0,10);ballMapRed(5,1,0,0,10);ballMapRed(5,2,0,0,10);ballMapRed(5,3,0,0,10);ballMapRed(5,4,0,0,10);ballMapRed(5,10,0,0,10);ballMapRed(5,11,0,0,10);ballMapRed(5,12,0,0,10);ballMapRed(5,13,0,0,10);ballMapRed(6,2,0,0,10);ballMapRed(6,3,0,0,10);ballMapRed(6,11,0,0,10);ballMapRed(6,12,0,0,10);ballMapRed(1,18,0,0,10);ballMapRed(1,19,0,0,10);ballMapRed(1,27,0,0,10);ballMapRed(1,28,0,0,10);ballMapRed(2,17,0,0,10);ballMapRed(2,18,0,0,10);ballMapRed(2,19,0,0,10);ballMapRed(2,20,0,0,10);ballMapRed(2,26,0,0,10);ballMapRed(2,27,0,0,10);ballMapRed(2,28,0,0,10);ballMapRed(2,29,0,0,10);ballMapRed(3,17,0,0,10);ballMapRed(3,18,0,0,10);ballMapRed(3,19,0,0,10);ballMapRed(3,20,0,0,10);ballMapRed(3,23,0,0,10);ballMapRed(3,24,0,0,10);ballMapRed(3,26,0,0,10);ballMapRed(3,27,0,0,10);ballMapRed(3,28,0,0,10);ballMapRed(3,29,0,0,10);ballMapRed(4,18,0,0,10);ballMapRed(4,19,0,0,10);ballMapRed(4,22,0,0,10);ballMapRed(4,23,0,0,10);ballMapRed(4,24,0,0,10);ballMapRed(4,25,0,0,10);ballMapRed(4,27,0,0,10);ballMapRed(4,28,0,0,10);ballMapRed(5,22,0,0,10);ballMapRed(5,23,0,0,10);ballMapRed(5,24,0,0,10);ballMapRed(5,25,0,0,10);ballMapRed(6,23,0,0,10);ballMapRed(6,24,0,0,10);
ballStep();
ballMapBlue(0,7,0,0,10);ballMapBlue(0,8,0,0,10);ballMapBlue(1,6,0,0,10);ballMapBlue(1,7,0,0,10);ballMapBlue(1,8,0,0,10);ballMapBlue(1,9,0,0,10);ballMapBlue(2,6,0,0,10);ballMapBlue(2,7,0,0,10);ballMapBlue(2,8,0,0,10);ballMapBlue(2,9,0,0,10);ballMapBlue(3,7,0,0,10);ballMapBlue(3,8,0,0,10);ballMapBlue(4,2,0,0,10);ballMapBlue(4,3,0,0,10);ballMapBlue(4,11,0,0,10);ballMapBlue(4,12,0,0,10);ballMapBlue(5,1,0,0,10);ballMapBlue(5,2,0,0,10);ballMapBlue(5,3,0,0,10);ballMapBlue(5,4,0,0,10);ballMapBlue(5,10,0,0,10);ballMapBlue(5,11,0,0,10);ballMapBlue(5,12,0,0,10);ballMapBlue(5,13,0,0,10);ballMapBlue(6,1,0,0,10);ballMapBlue(6,2,0,0,10);ballMapBlue(6,3,0,0,10);ballMapBlue(6,4,0,0,10);ballMapBlue(6,10,0,0,10);ballMapBlue(6,11,0,0,10);ballMapBlue(6,12,0,0,10);ballMapBlue(6,13,0,0,10);ballMapBlue(7,2,0,0,10);ballMapBlue(7,3,0,0,10);ballMapBlue(7,11,0,0,10);ballMapBlue(7,12,0,0,10);ballMapBlue(0,18,0,0,10);ballMapBlue(0,19,0,0,10);ballMapBlue(0,27,0,0,10);ballMapBlue(0,28,0,0,10);ballMapBlue(1,17,0,0,10);ballMapBlue(1,18,0,0,10);ballMapBlue(1,19,0,0,10);ballMapBlue(1,20,0,0,10);ballMapBlue(1,26,0,0,10);ballMapBlue(1,27,0,0,10);ballMapBlue(1,28,0,0,10);ballMapBlue(1,29,0,0,10);ballMapBlue(2,17,0,0,10);ballMapBlue(2,18,0,0,10);ballMapBlue(2,19,0,0,10);ballMapBlue(2,20,0,0,10);ballMapBlue(2,26,0,0,10);ballMapBlue(2,27,0,0,10);ballMapBlue(2,28,0,0,10);ballMapBlue(2,29,0,0,10);ballMapBlue(3,18,0,0,10);ballMapBlue(3,19,0,0,10);ballMapBlue(3,27,0,0,10);ballMapBlue(3,28,0,0,10);ballMapBlue(4,23,0,0,10);ballMapBlue(4,24,0,0,10);ballMapBlue(5,22,0,0,10);ballMapBlue(5,23,0,0,10);ballMapBlue(5,24,0,0,10);ballMapBlue(5,25,0,0,10);ballMapBlue(6,22,0,0,10);ballMapBlue(6,23,0,0,10);ballMapBlue(6,24,0,0,10);ballMapBlue(6,25,0,0,10);ballMapBlue(7,23,0,0,10);ballMapBlue(7,24,0,0,10);
ballColorStep();
ballMapRed(0,7,0,0,10);ballMapRed(0,8,0,0,10);ballMapRed(1,6,0,0,10);ballMapRed(1,7,0,0,10);ballMapRed(1,8,0,0,10);ballMapRed(1,9,0,0,10);ballMapRed(2,6,0,0,10);ballMapRed(2,7,0,0,10);ballMapRed(2,8,0,0,10);ballMapRed(2,9,0,0,10);ballMapRed(3,7,0,0,10);ballMapRed(3,8,0,0,10);ballMapRed(4,2,0,0,10);ballMapRed(4,3,0,0,10);ballMapRed(4,11,0,0,10);ballMapRed(4,12,0,0,10);ballMapRed(5,1,0,0,10);ballMapRed(5,2,0,0,10);ballMapRed(5,3,0,0,10);ballMapRed(5,4,0,0,10);ballMapRed(5,10,0,0,10);ballMapRed(5,11,0,0,10);ballMapRed(5,12,0,0,10);ballMapRed(5,13,0,0,10);ballMapRed(6,1,0,0,10);ballMapRed(6,2,0,0,10);ballMapRed(6,3,0,0,10);ballMapRed(6,4,0,0,10);ballMapRed(6,10,0,0,10);ballMapRed(6,11,0,0,10);ballMapRed(6,12,0,0,10);ballMapRed(6,13,0,0,10);ballMapRed(7,2,0,0,10);ballMapRed(7,3,0,0,10);ballMapRed(7,11,0,0,10);ballMapRed(7,12,0,0,10);ballMapRed(0,18,0,0,10);ballMapRed(0,19,0,0,10);ballMapRed(0,27,0,0,10);ballMapRed(0,28,0,0,10);ballMapRed(1,17,0,0,10);ballMapRed(1,18,0,0,10);ballMapRed(1,19,0,0,10);ballMapRed(1,20,0,0,10);ballMapRed(1,26,0,0,10);ballMapRed(1,27,0,0,10);ballMapRed(1,28,0,0,10);ballMapRed(1,29,0,0,10);ballMapRed(2,17,0,0,10);ballMapRed(2,18,0,0,10);ballMapRed(2,19,0,0,10);ballMapRed(2,20,0,0,10);ballMapRed(2,26,0,0,10);ballMapRed(2,27,0,0,10);ballMapRed(2,28,0,0,10);ballMapRed(2,29,0,0,10);ballMapRed(3,18,0,0,10);ballMapRed(3,19,0,0,10);ballMapRed(3,27,0,0,10);ballMapRed(3,28,0,0,10);ballMapRed(4,23,0,0,10);ballMapRed(4,24,0,0,10);ballMapRed(5,22,0,0,10);ballMapRed(5,23,0,0,10);ballMapRed(5,24,0,0,10);ballMapRed(5,25,0,0,10);ballMapRed(6,22,0,0,10);ballMapRed(6,23,0,0,10);ballMapRed(6,24,0,0,10);ballMapRed(6,25,0,0,10);ballMapRed(7,23,0,0,10);ballMapRed(7,24,0,0,10);
ballStep();
ballMapBlue(0,6,0,0,10);ballMapBlue(0,7,0,0,10);ballMapBlue(0,8,0,0,10);ballMapBlue(0,9,0,0,10);ballMapBlue(1,6,0,0,10);ballMapBlue(1,7,0,0,10);ballMapBlue(1,8,0,0,10);ballMapBlue(1,9,0,0,10);ballMapBlue(2,7,0,0,10);ballMapBlue(2,8,0,0,10);ballMapBlue(5,2,0,0,10);ballMapBlue(5,3,0,0,10);ballMapBlue(5,11,0,0,10);ballMapBlue(5,12,0,0,10);ballMapBlue(6,1,0,0,10);ballMapBlue(6,2,0,0,10);ballMapBlue(6,3,0,0,10);ballMapBlue(6,4,0,0,10);ballMapBlue(6,10,0,0,10);ballMapBlue(6,11,0,0,10);ballMapBlue(6,12,0,0,10);ballMapBlue(6,13,0,0,10);ballMapBlue(7,1,0,0,10);ballMapBlue(7,2,0,0,10);ballMapBlue(7,3,0,0,10);ballMapBlue(7,4,0,0,10);ballMapBlue(7,10,0,0,10);ballMapBlue(7,11,0,0,10);ballMapBlue(7,12,0,0,10);ballMapBlue(7,13,0,0,10);ballMapBlue(0,17,0,0,10);ballMapBlue(0,18,0,0,10);ballMapBlue(0,19,0,0,10);ballMapBlue(0,20,0,0,10);ballMapBlue(0,26,0,0,10);ballMapBlue(0,27,0,0,10);ballMapBlue(0,28,0,0,10);ballMapBlue(0,29,0,0,10);ballMapBlue(1,17,0,0,10);ballMapBlue(1,18,0,0,10);ballMapBlue(1,19,0,0,10);ballMapBlue(1,20,0,0,10);ballMapBlue(1,26,0,0,10);ballMapBlue(1,27,0,0,10);ballMapBlue(1,28,0,0,10);ballMapBlue(1,29,0,0,10);ballMapBlue(2,18,0,0,10);ballMapBlue(2,19,0,0,10);ballMapBlue(2,27,0,0,10);ballMapBlue(2,28,0,0,10);ballMapBlue(5,23,0,0,10);ballMapBlue(5,24,0,0,10);ballMapBlue(6,22,0,0,10);ballMapBlue(6,23,0,0,10);ballMapBlue(6,24,0,0,10);ballMapBlue(6,25,0,0,10);ballMapBlue(7,22,0,0,10);ballMapBlue(7,23,0,0,10);ballMapBlue(7,24,0,0,10);ballMapBlue(7,25,0,0,10);
ballColorStep();
ballMapRed(0,6,0,0,10);ballMapRed(0,7,0,0,10);ballMapRed(0,8,0,0,10);ballMapRed(0,9,0,0,10);ballMapRed(1,6,0,0,10);ballMapRed(1,7,0,0,10);ballMapRed(1,8,0,0,10);ballMapRed(1,9,0,0,10);ballMapRed(2,7,0,0,10);ballMapRed(2,8,0,0,10);ballMapRed(5,2,0,0,10);ballMapRed(5,3,0,0,10);ballMapRed(5,11,0,0,10);ballMapRed(5,12,0,0,10);ballMapRed(6,1,0,0,10);ballMapRed(6,2,0,0,10);ballMapRed(6,3,0,0,10);ballMapRed(6,4,0,0,10);ballMapRed(6,10,0,0,10);ballMapRed(6,11,0,0,10);ballMapRed(6,12,0,0,10);ballMapRed(6,13,0,0,10);ballMapRed(7,1,0,0,10);ballMapRed(7,2,0,0,10);ballMapRed(7,3,0,0,10);ballMapRed(7,4,0,0,10);ballMapRed(7,10,0,0,10);ballMapRed(7,11,0,0,10);ballMapRed(7,12,0,0,10);ballMapRed(7,13,0,0,10);ballMapRed(0,17,0,0,10);ballMapRed(0,18,0,0,10);ballMapRed(0,19,0,0,10);ballMapRed(0,20,0,0,10);ballMapRed(0,26,0,0,10);ballMapRed(0,27,0,0,10);ballMapRed(0,28,0,0,10);ballMapRed(0,29,0,0,10);ballMapRed(1,17,0,0,10);ballMapRed(1,18,0,0,10);ballMapRed(1,19,0,0,10);ballMapRed(1,20,0,0,10);ballMapRed(1,26,0,0,10);ballMapRed(1,27,0,0,10);ballMapRed(1,28,0,0,10);ballMapRed(1,29,0,0,10);ballMapRed(2,18,0,0,10);ballMapRed(2,19,0,0,10);ballMapRed(2,27,0,0,10);ballMapRed(2,28,0,0,10);ballMapRed(5,23,0,0,10);ballMapRed(5,24,0,0,10);ballMapRed(6,22,0,0,10);ballMapRed(6,23,0,0,10);ballMapRed(6,24,0,0,10);ballMapRed(6,25,0,0,10);ballMapRed(7,22,0,0,10);ballMapRed(7,23,0,0,10);ballMapRed(7,24,0,0,10);ballMapRed(7,25,0,0,10);
ballStep();
ballMapBlue(0,6,0,0,10);ballMapBlue(0,7,0,0,10);ballMapBlue(0,8,0,0,10);ballMapBlue(0,9,0,0,10);ballMapBlue(1,7,0,0,10);ballMapBlue(1,8,0,0,10);ballMapBlue(6,2,0,0,10);ballMapBlue(6,3,0,0,10);ballMapBlue(6,11,0,0,10);ballMapBlue(6,12,0,0,10);ballMapBlue(7,1,0,0,10);ballMapBlue(7,2,0,0,10);ballMapBlue(7,3,0,0,10);ballMapBlue(7,4,0,0,10);ballMapBlue(7,10,0,0,10);ballMapBlue(7,11,0,0,10);ballMapBlue(7,12,0,0,10);ballMapBlue(7,13,0,0,10);ballMapBlue(0,17,0,0,10);ballMapBlue(0,18,0,0,10);ballMapBlue(0,19,0,0,10);ballMapBlue(0,20,0,0,10);ballMapBlue(0,26,0,0,10);ballMapBlue(0,27,0,0,10);ballMapBlue(0,28,0,0,10);ballMapBlue(0,29,0,0,10);ballMapBlue(1,18,0,0,10);ballMapBlue(1,19,0,0,10);ballMapBlue(1,27,0,0,10);ballMapBlue(1,28,0,0,10);ballMapBlue(6,23,0,0,10);ballMapBlue(6,24,0,0,10);ballMapBlue(7,22,0,0,10);ballMapBlue(7,23,0,0,10);ballMapBlue(7,24,0,0,10);ballMapBlue(7,25,0,0,10);
ballColorStep();
ballMapRed(0,6,0,0,10);ballMapRed(0,7,0,0,10);ballMapRed(0,8,0,0,10);ballMapRed(0,9,0,0,10);ballMapRed(1,7,0,0,10);ballMapRed(1,8,0,0,10);ballMapRed(6,2,0,0,10);ballMapRed(6,3,0,0,10);ballMapRed(6,11,0,0,10);ballMapRed(6,12,0,0,10);ballMapRed(7,1,0,0,10);ballMapRed(7,2,0,0,10);ballMapRed(7,3,0,0,10);ballMapRed(7,4,0,0,10);ballMapRed(7,10,0,0,10);ballMapRed(7,11,0,0,10);ballMapRed(7,12,0,0,10);ballMapRed(7,13,0,0,10);ballMapRed(0,17,0,0,10);ballMapRed(0,18,0,0,10);ballMapRed(0,19,0,0,10);ballMapRed(0,20,0,0,10);ballMapRed(0,26,0,0,10);ballMapRed(0,27,0,0,10);ballMapRed(0,28,0,0,10);ballMapRed(0,29,0,0,10);ballMapRed(1,18,0,0,10);ballMapRed(1,19,0,0,10);ballMapRed(1,27,0,0,10);ballMapRed(1,28,0,0,10);ballMapRed(6,23,0,0,10);ballMapRed(6,24,0,0,10);ballMapRed(7,22,0,0,10);ballMapRed(7,23,0,0,10);ballMapRed(7,24,0,0,10);ballMapRed(7,25,0,0,10);
ballStep();
ballMapBlue(0,7,0,0,10);ballMapBlue(0,8,0,0,10);ballMapBlue(7,2,0,0,10);ballMapBlue(7,3,0,0,10);ballMapBlue(7,11,0,0,10);ballMapBlue(7,12,0,0,10);ballMapBlue(0,18,0,0,10);ballMapBlue(0,19,0,0,10);ballMapBlue(0,27,0,0,10);ballMapBlue(0,28,0,0,10);ballMapBlue(7,23,0,0,10);ballMapBlue(7,24,0,0,10);
ballColorStep();
ballMapRed(0,7,0,0,10);ballMapRed(0,8,0,0,10);ballMapRed(7,2,0,0,10);ballMapRed(7,3,0,0,10);ballMapRed(7,11,0,0,10);ballMapRed(7,12,0,0,10);ballMapRed(0,18,0,0,10);ballMapRed(0,19,0,0,10);ballMapRed(0,27,0,0,10);ballMapRed(0,28,0,0,10);ballMapRed(7,23,0,0,10);ballMapRed(7,24,0,0,10);
ballStep();
}
void ballGFX(){
ballMapBlue(0,2,0,0,10);ballMapBlue(0,3,0,0,10);ballMapBlue(0,11,0,0,10);ballMapBlue(0,12,0,0,10);ballMapBlue(7,7,0,0,10);ballMapBlue(7,8,0,0,10);ballMapBlue(0,23,0,0,10);ballMapBlue(0,24,0,0,10);ballMapBlue(7,18,0,0,10);ballMapBlue(7,19,0,0,10);ballMapBlue(7,27,0,0,10);ballMapBlue(7,28,0,0,10);
ballColorStep();
ballMapRed(0,2,0,0,10);ballMapRed(0,3,0,0,10);ballMapRed(0,11,0,0,10);ballMapRed(0,12,0,0,10);ballMapRed(7,7,0,0,10);ballMapRed(7,8,0,0,10);ballMapRed(0,23,0,0,10);ballMapRed(0,24,0,0,10);ballMapRed(7,18,0,0,10);ballMapRed(7,19,0,0,10);ballMapRed(7,27,0,0,10);ballMapRed(7,28,0,0,10);
ballColorStep();
ballMapGreen(0,2,0,0,10);ballMapGreen(0,3,0,0,10);ballMapGreen(0,11,0,0,10);ballMapGreen(0,12,0,0,10);ballMapGreen(7,7,0,0,10);ballMapGreen(7,8,0,0,10);ballMapGreen(0,23,0,0,10);ballMapGreen(0,24,0,0,10);ballMapGreen(7,18,0,0,10);ballMapGreen(7,19,0,0,10);ballMapGreen(7,27,0,0,10);ballMapGreen(7,28,0,0,10);

ballStep();

ballMapBlue(0,1,0,0,10);ballMapBlue(0,2,0,0,10);ballMapBlue(0,3,0,0,10);ballMapBlue(0,4,0,0,10);ballMapBlue(0,10,0,0,10);ballMapBlue(0,11,0,0,10);ballMapBlue(0,12,0,0,10);ballMapBlue(0,13,0,0,10);ballMapBlue(1,2,0,0,10);ballMapBlue(1,3,0,0,10);ballMapBlue(1,11,0,0,10);ballMapBlue(1,12,0,0,10);ballMapBlue(6,7,0,0,10);ballMapBlue(6,8,0,0,10);ballMapBlue(7,6,0,0,10);ballMapBlue(7,7,0,0,10);ballMapBlue(7,8,0,0,10);ballMapBlue(7,9,0,0,10);ballMapBlue(0,22,0,0,10);ballMapBlue(0,23,0,0,10);ballMapBlue(0,24,0,0,10);ballMapBlue(0,25,0,0,10);ballMapBlue(1,23,0,0,10);ballMapBlue(1,24,0,0,10);ballMapBlue(6,18,0,0,10);ballMapBlue(6,19,0,0,10);ballMapBlue(6,27,0,0,10);ballMapBlue(6,28,0,0,10);ballMapBlue(7,17,0,0,10);ballMapBlue(7,18,0,0,10);ballMapBlue(7,19,0,0,10);ballMapBlue(7,20,0,0,10);ballMapBlue(7,26,0,0,10);ballMapBlue(7,27,0,0,10);ballMapBlue(7,28,0,0,10);ballMapBlue(7,29,0,0,10);
ballColorStep();
ballMapRed(0,1,0,0,10);ballMapRed(0,2,0,0,10);ballMapRed(0,3,0,0,10);ballMapRed(0,4,0,0,10);ballMapRed(0,10,0,0,10);ballMapRed(0,11,0,0,10);ballMapRed(0,12,0,0,10);ballMapRed(0,13,0,0,10);ballMapRed(1,2,0,0,10);ballMapRed(1,3,0,0,10);ballMapRed(1,11,0,0,10);ballMapRed(1,12,0,0,10);ballMapRed(6,7,0,0,10);ballMapRed(6,8,0,0,10);ballMapRed(7,6,0,0,10);ballMapRed(7,7,0,0,10);ballMapRed(7,8,0,0,10);ballMapRed(7,9,0,0,10);ballMapRed(0,22,0,0,10);ballMapRed(0,23,0,0,10);ballMapRed(0,24,0,0,10);ballMapRed(0,25,0,0,10);ballMapRed(1,23,0,0,10);ballMapRed(1,24,0,0,10);ballMapRed(6,18,0,0,10);ballMapRed(6,19,0,0,10);ballMapRed(6,27,0,0,10);ballMapRed(6,28,0,0,10);ballMapRed(7,17,0,0,10);ballMapRed(7,18,0,0,10);ballMapRed(7,19,0,0,10);ballMapRed(7,20,0,0,10);ballMapRed(7,26,0,0,10);ballMapRed(7,27,0,0,10);ballMapRed(7,28,0,0,10);ballMapRed(7,29,0,0,10);
ballColorStep();
ballMapGreen(0,1,0,0,10);ballMapGreen(0,2,0,0,10);ballMapGreen(0,3,0,0,10);ballMapGreen(0,4,0,0,10);ballMapGreen(0,10,0,0,10);ballMapGreen(0,11,0,0,10);ballMapGreen(0,12,0,0,10);ballMapGreen(0,13,0,0,10);ballMapGreen(1,2,0,0,10);ballMapGreen(1,3,0,0,10);ballMapGreen(1,11,0,0,10);ballMapGreen(1,12,0,0,10);ballMapGreen(6,7,0,0,10);ballMapGreen(6,8,0,0,10);ballMapGreen(7,6,0,0,10);ballMapGreen(7,7,0,0,10);ballMapGreen(7,8,0,0,10);ballMapGreen(7,9,0,0,10);ballMapGreen(0,22,0,0,10);ballMapGreen(0,23,0,0,10);ballMapGreen(0,24,0,0,10);ballMapGreen(0,25,0,0,10);ballMapGreen(1,23,0,0,10);ballMapGreen(1,24,0,0,10);ballMapGreen(6,18,0,0,10);ballMapGreen(6,19,0,0,10);ballMapGreen(6,27,0,0,10);ballMapGreen(6,28,0,0,10);ballMapGreen(7,17,0,0,10);ballMapGreen(7,18,0,0,10);ballMapGreen(7,19,0,0,10);ballMapGreen(7,20,0,0,10);ballMapGreen(7,26,0,0,10);ballMapGreen(7,27,0,0,10);ballMapGreen(7,28,0,0,10);ballMapGreen(7,29,0,0,10);

ballStep();

ballMapBlue(0,1,0,0,10);ballMapBlue(0,2,0,0,10);ballMapBlue(0,3,0,0,10);ballMapBlue(0,4,0,0,10);ballMapBlue(0,10,0,0,10);ballMapBlue(0,11,0,0,10);ballMapBlue(0,12,0,0,10);ballMapBlue(0,13,0,0,10);ballMapBlue(1,1,0,0,10);ballMapBlue(1,2,0,0,10);ballMapBlue(1,3,0,0,10);ballMapBlue(1,4,0,0,10);ballMapBlue(1,10,0,0,10);ballMapBlue(1,11,0,0,10);ballMapBlue(1,12,0,0,10);ballMapBlue(1,13,0,0,10);ballMapBlue(2,2,0,0,10);ballMapBlue(2,3,0,0,10);ballMapBlue(2,11,0,0,10);ballMapBlue(2,12,0,0,10);ballMapBlue(5,7,0,0,10);ballMapBlue(5,8,0,0,10);ballMapBlue(6,6,0,0,10);ballMapBlue(6,7,0,0,10);ballMapBlue(6,8,0,0,10);ballMapBlue(6,9,0,0,10);ballMapBlue(7,6,0,0,10);ballMapBlue(7,7,0,0,10);ballMapBlue(7,8,0,0,10);ballMapBlue(7,9,0,0,10);ballMapBlue(0,22,0,0,10);ballMapBlue(0,23,0,0,10);ballMapBlue(0,24,0,0,10);ballMapBlue(0,25,0,0,10);ballMapBlue(1,22,0,0,10);ballMapBlue(1,23,0,0,10);ballMapBlue(1,24,0,0,10);ballMapBlue(1,25,0,0,10);ballMapBlue(2,23,0,0,10);ballMapBlue(2,24,0,0,10);ballMapBlue(5,18,0,0,10);ballMapBlue(5,19,0,0,10);ballMapBlue(5,27,0,0,10);ballMapBlue(5,28,0,0,10);ballMapBlue(6,17,0,0,10);ballMapBlue(6,18,0,0,10);ballMapBlue(6,19,0,0,10);ballMapBlue(6,20,0,0,10);ballMapBlue(6,26,0,0,10);ballMapBlue(6,27,0,0,10);ballMapBlue(6,28,0,0,10);ballMapBlue(6,29,0,0,10);ballMapBlue(7,17,0,0,10);ballMapBlue(7,18,0,0,10);ballMapBlue(7,19,0,0,10);ballMapBlue(7,20,0,0,10);ballMapBlue(7,26,0,0,10);ballMapBlue(7,27,0,0,10);ballMapBlue(7,28,0,0,10);ballMapBlue(7,29,0,0,10);
ballColorStep();
ballMapRed(0,1,0,0,10);ballMapRed(0,2,0,0,10);ballMapRed(0,3,0,0,10);ballMapRed(0,4,0,0,10);ballMapRed(0,10,0,0,10);ballMapRed(0,11,0,0,10);ballMapRed(0,12,0,0,10);ballMapRed(0,13,0,0,10);ballMapRed(1,1,0,0,10);ballMapRed(1,2,0,0,10);ballMapRed(1,3,0,0,10);ballMapRed(1,4,0,0,10);ballMapRed(1,10,0,0,10);ballMapRed(1,11,0,0,10);ballMapRed(1,12,0,0,10);ballMapRed(1,13,0,0,10);ballMapRed(2,2,0,0,10);ballMapRed(2,3,0,0,10);ballMapRed(2,11,0,0,10);ballMapRed(2,12,0,0,10);ballMapRed(5,7,0,0,10);ballMapRed(5,8,0,0,10);ballMapRed(6,6,0,0,10);ballMapRed(6,7,0,0,10);ballMapRed(6,8,0,0,10);ballMapRed(6,9,0,0,10);ballMapRed(7,6,0,0,10);ballMapRed(7,7,0,0,10);ballMapRed(7,8,0,0,10);ballMapRed(7,9,0,0,10);ballMapRed(0,22,0,0,10);ballMapRed(0,23,0,0,10);ballMapRed(0,24,0,0,10);ballMapRed(0,25,0,0,10);ballMapRed(1,22,0,0,10);ballMapRed(1,23,0,0,10);ballMapRed(1,24,0,0,10);ballMapRed(1,25,0,0,10);ballMapRed(2,23,0,0,10);ballMapRed(2,24,0,0,10);ballMapRed(5,18,0,0,10);ballMapRed(5,19,0,0,10);ballMapRed(5,27,0,0,10);ballMapRed(5,28,0,0,10);ballMapRed(6,17,0,0,10);ballMapRed(6,18,0,0,10);ballMapRed(6,19,0,0,10);ballMapRed(6,20,0,0,10);ballMapRed(6,26,0,0,10);ballMapRed(6,27,0,0,10);ballMapRed(6,28,0,0,10);ballMapRed(6,29,0,0,10);ballMapRed(7,17,0,0,10);ballMapRed(7,18,0,0,10);ballMapRed(7,19,0,0,10);ballMapRed(7,20,0,0,10);ballMapRed(7,26,0,0,10);ballMapRed(7,27,0,0,10);ballMapRed(7,28,0,0,10);ballMapRed(7,29,0,0,10);
ballColorStep();
ballMapGreen(0,1,0,0,10);ballMapGreen(0,2,0,0,10);ballMapGreen(0,3,0,0,10);ballMapGreen(0,4,0,0,10);ballMapGreen(0,10,0,0,10);ballMapGreen(0,11,0,0,10);ballMapGreen(0,12,0,0,10);ballMapGreen(0,13,0,0,10);ballMapGreen(1,1,0,0,10);ballMapGreen(1,2,0,0,10);ballMapGreen(1,3,0,0,10);ballMapGreen(1,4,0,0,10);ballMapGreen(1,10,0,0,10);ballMapGreen(1,11,0,0,10);ballMapGreen(1,12,0,0,10);ballMapGreen(1,13,0,0,10);ballMapGreen(2,2,0,0,10);ballMapGreen(2,3,0,0,10);ballMapGreen(2,11,0,0,10);ballMapGreen(2,12,0,0,10);ballMapGreen(5,7,0,0,10);ballMapGreen(5,8,0,0,10);ballMapGreen(6,6,0,0,10);ballMapGreen(6,7,0,0,10);ballMapGreen(6,8,0,0,10);ballMapGreen(6,9,0,0,10);ballMapGreen(7,6,0,0,10);ballMapGreen(7,7,0,0,10);ballMapGreen(7,8,0,0,10);ballMapGreen(7,9,0,0,10);ballMapGreen(0,22,0,0,10);ballMapGreen(0,23,0,0,10);ballMapGreen(0,24,0,0,10);ballMapGreen(0,25,0,0,10);ballMapGreen(1,22,0,0,10);ballMapGreen(1,23,0,0,10);ballMapGreen(1,24,0,0,10);ballMapGreen(1,25,0,0,10);ballMapGreen(2,23,0,0,10);ballMapGreen(2,24,0,0,10);ballMapGreen(5,18,0,0,10);ballMapGreen(5,19,0,0,10);ballMapGreen(5,27,0,0,10);ballMapGreen(5,28,0,0,10);ballMapGreen(6,17,0,0,10);ballMapGreen(6,18,0,0,10);ballMapGreen(6,19,0,0,10);ballMapGreen(6,20,0,0,10);ballMapGreen(6,26,0,0,10);ballMapGreen(6,27,0,0,10);ballMapGreen(6,28,0,0,10);ballMapGreen(6,29,0,0,10);ballMapGreen(7,17,0,0,10);ballMapGreen(7,18,0,0,10);ballMapGreen(7,19,0,0,10);ballMapGreen(7,20,0,0,10);ballMapGreen(7,26,0,0,10);ballMapGreen(7,27,0,0,10);ballMapGreen(7,28,0,0,10);ballMapGreen(7,29,0,0,10);

ballStep();

ballMapBlue(0,2,0,0,10);ballMapBlue(0,3,0,0,10);ballMapBlue(0,11,0,0,10);ballMapBlue(0,12,0,0,10);ballMapBlue(1,1,0,0,10);ballMapBlue(1,2,0,0,10);ballMapBlue(1,3,0,0,10);ballMapBlue(1,4,0,0,10);ballMapBlue(1,10,0,0,10);ballMapBlue(1,11,0,0,10);ballMapBlue(1,12,0,0,10);ballMapBlue(1,13,0,0,10);ballMapBlue(2,1,0,0,10);ballMapBlue(2,2,0,0,10);ballMapBlue(2,3,0,0,10);ballMapBlue(2,4,0,0,10);ballMapBlue(2,10,0,0,10);ballMapBlue(2,11,0,0,10);ballMapBlue(2,12,0,0,10);ballMapBlue(2,13,0,0,10);ballMapBlue(3,2,0,0,10);ballMapBlue(3,3,0,0,10);ballMapBlue(3,11,0,0,10);ballMapBlue(3,12,0,0,10);ballMapBlue(4,7,0,0,10);ballMapBlue(4,8,0,0,10);ballMapBlue(5,6,0,0,10);ballMapBlue(5,7,0,0,10);ballMapBlue(5,8,0,0,10);ballMapBlue(5,9,0,0,10);ballMapBlue(6,6,0,0,10);ballMapBlue(6,7,0,0,10);ballMapBlue(6,8,0,0,10);ballMapBlue(6,9,0,0,10);ballMapBlue(7,7,0,0,10);ballMapBlue(7,8,0,0,10);ballMapBlue(0,23,0,0,10);ballMapBlue(0,24,0,0,10);ballMapBlue(1,22,0,0,10);ballMapBlue(1,23,0,0,10);ballMapBlue(1,24,0,0,10);ballMapBlue(1,25,0,0,10);ballMapBlue(2,22,0,0,10);ballMapBlue(2,23,0,0,10);ballMapBlue(2,24,0,0,10);ballMapBlue(2,25,0,0,10);ballMapBlue(3,23,0,0,10);ballMapBlue(3,24,0,0,10);ballMapBlue(4,18,0,0,10);ballMapBlue(4,19,0,0,10);ballMapBlue(4,27,0,0,10);ballMapBlue(4,28,0,0,10);ballMapBlue(5,17,0,0,10);ballMapBlue(5,18,0,0,10);ballMapBlue(5,19,0,0,10);ballMapBlue(5,20,0,0,10);ballMapBlue(5,26,0,0,10);ballMapBlue(5,27,0,0,10);ballMapBlue(5,28,0,0,10);ballMapBlue(5,29,0,0,10);ballMapBlue(6,17,0,0,10);ballMapBlue(6,18,0,0,10);ballMapBlue(6,19,0,0,10);ballMapBlue(6,20,0,0,10);ballMapBlue(6,26,0,0,10);ballMapBlue(6,27,0,0,10);ballMapBlue(6,28,0,0,10);ballMapBlue(6,29,0,0,10);ballMapBlue(7,18,0,0,10);ballMapBlue(7,19,0,0,10);ballMapBlue(7,27,0,0,10);ballMapBlue(7,28,0,0,10);
ballColorStep();
ballMapRed(0,2,0,0,10);ballMapRed(0,3,0,0,10);ballMapRed(0,11,0,0,10);ballMapRed(0,12,0,0,10);ballMapRed(1,1,0,0,10);ballMapRed(1,2,0,0,10);ballMapRed(1,3,0,0,10);ballMapRed(1,4,0,0,10);ballMapRed(1,10,0,0,10);ballMapRed(1,11,0,0,10);ballMapRed(1,12,0,0,10);ballMapRed(1,13,0,0,10);ballMapRed(2,1,0,0,10);ballMapRed(2,2,0,0,10);ballMapRed(2,3,0,0,10);ballMapRed(2,4,0,0,10);ballMapRed(2,10,0,0,10);ballMapRed(2,11,0,0,10);ballMapRed(2,12,0,0,10);ballMapRed(2,13,0,0,10);ballMapRed(3,2,0,0,10);ballMapRed(3,3,0,0,10);ballMapRed(3,11,0,0,10);ballMapRed(3,12,0,0,10);ballMapRed(4,7,0,0,10);ballMapRed(4,8,0,0,10);ballMapRed(5,6,0,0,10);ballMapRed(5,7,0,0,10);ballMapRed(5,8,0,0,10);ballMapRed(5,9,0,0,10);ballMapRed(6,6,0,0,10);ballMapRed(6,7,0,0,10);ballMapRed(6,8,0,0,10);ballMapRed(6,9,0,0,10);ballMapRed(7,7,0,0,10);ballMapRed(7,8,0,0,10);ballMapRed(0,23,0,0,10);ballMapRed(0,24,0,0,10);ballMapRed(1,22,0,0,10);ballMapRed(1,23,0,0,10);ballMapRed(1,24,0,0,10);ballMapRed(1,25,0,0,10);ballMapRed(2,22,0,0,10);ballMapRed(2,23,0,0,10);ballMapRed(2,24,0,0,10);ballMapRed(2,25,0,0,10);ballMapRed(3,23,0,0,10);ballMapRed(3,24,0,0,10);ballMapRed(4,18,0,0,10);ballMapRed(4,19,0,0,10);ballMapRed(4,27,0,0,10);ballMapRed(4,28,0,0,10);ballMapRed(5,17,0,0,10);ballMapRed(5,18,0,0,10);ballMapRed(5,19,0,0,10);ballMapRed(5,20,0,0,10);ballMapRed(5,26,0,0,10);ballMapRed(5,27,0,0,10);ballMapRed(5,28,0,0,10);ballMapRed(5,29,0,0,10);ballMapRed(6,17,0,0,10);ballMapRed(6,18,0,0,10);ballMapRed(6,19,0,0,10);ballMapRed(6,20,0,0,10);ballMapRed(6,26,0,0,10);ballMapRed(6,27,0,0,10);ballMapRed(6,28,0,0,10);ballMapRed(6,29,0,0,10);ballMapRed(7,18,0,0,10);ballMapRed(7,19,0,0,10);ballMapRed(7,27,0,0,10);ballMapRed(7,28,0,0,10);
ballColorStep();
ballMapGreen(0,2,0,0,10);ballMapGreen(0,3,0,0,10);ballMapGreen(0,11,0,0,10);ballMapGreen(0,12,0,0,10);ballMapGreen(1,1,0,0,10);ballMapGreen(1,2,0,0,10);ballMapGreen(1,3,0,0,10);ballMapGreen(1,4,0,0,10);ballMapGreen(1,10,0,0,10);ballMapGreen(1,11,0,0,10);ballMapGreen(1,12,0,0,10);ballMapGreen(1,13,0,0,10);ballMapGreen(2,1,0,0,10);ballMapGreen(2,2,0,0,10);ballMapGreen(2,3,0,0,10);ballMapGreen(2,4,0,0,10);ballMapGreen(2,10,0,0,10);ballMapGreen(2,11,0,0,10);ballMapGreen(2,12,0,0,10);ballMapGreen(2,13,0,0,10);ballMapGreen(3,2,0,0,10);ballMapGreen(3,3,0,0,10);ballMapGreen(3,11,0,0,10);ballMapGreen(3,12,0,0,10);ballMapGreen(4,7,0,0,10);ballMapGreen(4,8,0,0,10);ballMapGreen(5,6,0,0,10);ballMapGreen(5,7,0,0,10);ballMapGreen(5,8,0,0,10);ballMapGreen(5,9,0,0,10);ballMapGreen(6,6,0,0,10);ballMapGreen(6,7,0,0,10);ballMapGreen(6,8,0,0,10);ballMapGreen(6,9,0,0,10);ballMapGreen(7,7,0,0,10);ballMapGreen(7,8,0,0,10);ballMapGreen(0,23,0,0,10);ballMapGreen(0,24,0,0,10);ballMapGreen(1,22,0,0,10);ballMapGreen(1,23,0,0,10);ballMapGreen(1,24,0,0,10);ballMapGreen(1,25,0,0,10);ballMapGreen(2,22,0,0,10);ballMapGreen(2,23,0,0,10);ballMapGreen(2,24,0,0,10);ballMapGreen(2,25,0,0,10);ballMapGreen(3,23,0,0,10);ballMapGreen(3,24,0,0,10);ballMapGreen(4,18,0,0,10);ballMapGreen(4,19,0,0,10);ballMapGreen(4,27,0,0,10);ballMapGreen(4,28,0,0,10);ballMapGreen(5,17,0,0,10);ballMapGreen(5,18,0,0,10);ballMapGreen(5,19,0,0,10);ballMapGreen(5,20,0,0,10);ballMapGreen(5,26,0,0,10);ballMapGreen(5,27,0,0,10);ballMapGreen(5,28,0,0,10);ballMapGreen(5,29,0,0,10);ballMapGreen(6,17,0,0,10);ballMapGreen(6,18,0,0,10);ballMapGreen(6,19,0,0,10);ballMapGreen(6,20,0,0,10);ballMapGreen(6,26,0,0,10);ballMapGreen(6,27,0,0,10);ballMapGreen(6,28,0,0,10);ballMapGreen(6,29,0,0,10);ballMapGreen(7,18,0,0,10);ballMapGreen(7,19,0,0,10);ballMapGreen(7,27,0,0,10);ballMapGreen(7,28,0,0,10);

ballStep();

ballMapBlue(1,2,0,0,10);ballMapBlue(1,3,0,0,10);ballMapBlue(1,11,0,0,10);ballMapBlue(1,12,0,0,10);ballMapBlue(2,1,0,0,10);ballMapBlue(2,2,0,0,10);ballMapBlue(2,3,0,0,10);ballMapBlue(2,4,0,0,10);ballMapBlue(2,10,0,0,10);ballMapBlue(2,11,0,0,10);ballMapBlue(2,12,0,0,10);ballMapBlue(2,13,0,0,10);ballMapBlue(3,1,0,0,10);ballMapBlue(3,2,0,0,10);ballMapBlue(3,3,0,0,10);ballMapBlue(3,4,0,0,10);ballMapBlue(3,7,0,0,10);ballMapBlue(3,8,0,0,10);ballMapBlue(3,10,0,0,10);ballMapBlue(3,11,0,0,10);ballMapBlue(3,12,0,0,10);ballMapBlue(3,13,0,0,10);ballMapBlue(4,2,0,0,10);ballMapBlue(4,3,0,0,10);ballMapBlue(4,6,0,0,10);ballMapBlue(4,7,0,0,10);ballMapBlue(4,8,0,0,10);ballMapBlue(4,9,0,0,10);ballMapBlue(4,11,0,0,10);ballMapBlue(4,12,0,0,10);ballMapBlue(5,6,0,0,10);ballMapBlue(5,7,0,0,10);ballMapBlue(5,8,0,0,10);ballMapBlue(5,9,0,0,10);ballMapBlue(6,7,0,0,10);ballMapBlue(6,8,0,0,10);ballMapBlue(1,23,0,0,10);ballMapBlue(1,24,0,0,10);ballMapBlue(2,22,0,0,10);ballMapBlue(2,23,0,0,10);ballMapBlue(2,24,0,0,10);ballMapBlue(2,25,0,0,10);ballMapBlue(3,18,0,0,10);ballMapBlue(3,19,0,0,10);ballMapBlue(3,22,0,0,10);ballMapBlue(3,23,0,0,10);ballMapBlue(3,24,0,0,10);ballMapBlue(3,25,0,0,10);ballMapBlue(3,27,0,0,10);ballMapBlue(3,28,0,0,10);ballMapBlue(4,17,0,0,10);ballMapBlue(4,18,0,0,10);ballMapBlue(4,19,0,0,10);ballMapBlue(4,20,0,0,10);ballMapBlue(4,23,0,0,10);ballMapBlue(4,24,0,0,10);ballMapBlue(4,26,0,0,10);ballMapBlue(4,27,0,0,10);ballMapBlue(4,28,0,0,10);ballMapBlue(4,29,0,0,10);ballMapBlue(5,17,0,0,10);ballMapBlue(5,18,0,0,10);ballMapBlue(5,19,0,0,10);ballMapBlue(5,20,0,0,10);ballMapBlue(5,26,0,0,10);ballMapBlue(5,27,0,0,10);ballMapBlue(5,28,0,0,10);ballMapBlue(5,29,0,0,10);ballMapBlue(6,18,0,0,10);ballMapBlue(6,19,0,0,10);ballMapBlue(6,27,0,0,10);ballMapBlue(6,28,0,0,10);
ballColorStep();
ballMapRed(1,2,0,0,10);ballMapRed(1,3,0,0,10);ballMapRed(1,11,0,0,10);ballMapRed(1,12,0,0,10);ballMapRed(2,1,0,0,10);ballMapRed(2,2,0,0,10);ballMapRed(2,3,0,0,10);ballMapRed(2,4,0,0,10);ballMapRed(2,10,0,0,10);ballMapRed(2,11,0,0,10);ballMapRed(2,12,0,0,10);ballMapRed(2,13,0,0,10);ballMapRed(3,1,0,0,10);ballMapRed(3,2,0,0,10);ballMapRed(3,3,0,0,10);ballMapRed(3,4,0,0,10);ballMapRed(3,7,0,0,10);ballMapRed(3,8,0,0,10);ballMapRed(3,10,0,0,10);ballMapRed(3,11,0,0,10);ballMapRed(3,12,0,0,10);ballMapRed(3,13,0,0,10);ballMapRed(4,2,0,0,10);ballMapRed(4,3,0,0,10);ballMapRed(4,6,0,0,10);ballMapRed(4,7,0,0,10);ballMapRed(4,8,0,0,10);ballMapRed(4,9,0,0,10);ballMapRed(4,11,0,0,10);ballMapRed(4,12,0,0,10);ballMapRed(5,6,0,0,10);ballMapRed(5,7,0,0,10);ballMapRed(5,8,0,0,10);ballMapRed(5,9,0,0,10);ballMapRed(6,7,0,0,10);ballMapRed(6,8,0,0,10);ballMapRed(1,23,0,0,10);ballMapRed(1,24,0,0,10);ballMapRed(2,22,0,0,10);ballMapRed(2,23,0,0,10);ballMapRed(2,24,0,0,10);ballMapRed(2,25,0,0,10);ballMapRed(3,18,0,0,10);ballMapRed(3,19,0,0,10);ballMapRed(3,22,0,0,10);ballMapRed(3,23,0,0,10);ballMapRed(3,24,0,0,10);ballMapRed(3,25,0,0,10);ballMapRed(3,27,0,0,10);ballMapRed(3,28,0,0,10);ballMapRed(4,17,0,0,10);ballMapRed(4,18,0,0,10);ballMapRed(4,19,0,0,10);ballMapRed(4,20,0,0,10);ballMapRed(4,23,0,0,10);ballMapRed(4,24,0,0,10);ballMapRed(4,26,0,0,10);ballMapRed(4,27,0,0,10);ballMapRed(4,28,0,0,10);ballMapRed(4,29,0,0,10);ballMapRed(5,17,0,0,10);ballMapRed(5,18,0,0,10);ballMapRed(5,19,0,0,10);ballMapRed(5,20,0,0,10);ballMapRed(5,26,0,0,10);ballMapRed(5,27,0,0,10);ballMapRed(5,28,0,0,10);ballMapRed(5,29,0,0,10);ballMapRed(6,18,0,0,10);ballMapRed(6,19,0,0,10);ballMapRed(6,27,0,0,10);ballMapRed(6,28,0,0,10);
ballColorStep();
ballMapGreen(1,2,0,0,10);ballMapGreen(1,3,0,0,10);ballMapGreen(1,11,0,0,10);ballMapGreen(1,12,0,0,10);ballMapGreen(2,1,0,0,10);ballMapGreen(2,2,0,0,10);ballMapGreen(2,3,0,0,10);ballMapGreen(2,4,0,0,10);ballMapGreen(2,10,0,0,10);ballMapGreen(2,11,0,0,10);ballMapGreen(2,12,0,0,10);ballMapGreen(2,13,0,0,10);ballMapGreen(3,1,0,0,10);ballMapGreen(3,2,0,0,10);ballMapGreen(3,3,0,0,10);ballMapGreen(3,4,0,0,10);ballMapGreen(3,7,0,0,10);ballMapGreen(3,8,0,0,10);ballMapGreen(3,10,0,0,10);ballMapGreen(3,11,0,0,10);ballMapGreen(3,12,0,0,10);ballMapGreen(3,13,0,0,10);ballMapGreen(4,2,0,0,10);ballMapGreen(4,3,0,0,10);ballMapGreen(4,6,0,0,10);ballMapGreen(4,7,0,0,10);ballMapGreen(4,8,0,0,10);ballMapGreen(4,9,0,0,10);ballMapGreen(4,11,0,0,10);ballMapGreen(4,12,0,0,10);ballMapGreen(5,6,0,0,10);ballMapGreen(5,7,0,0,10);ballMapGreen(5,8,0,0,10);ballMapGreen(5,9,0,0,10);ballMapGreen(6,7,0,0,10);ballMapGreen(6,8,0,0,10);ballMapGreen(1,23,0,0,10);ballMapGreen(1,24,0,0,10);ballMapGreen(2,22,0,0,10);ballMapGreen(2,23,0,0,10);ballMapGreen(2,24,0,0,10);ballMapGreen(2,25,0,0,10);ballMapGreen(3,18,0,0,10);ballMapGreen(3,19,0,0,10);ballMapGreen(3,22,0,0,10);ballMapGreen(3,23,0,0,10);ballMapGreen(3,24,0,0,10);ballMapGreen(3,25,0,0,10);ballMapGreen(3,27,0,0,10);ballMapGreen(3,28,0,0,10);ballMapGreen(4,17,0,0,10);ballMapGreen(4,18,0,0,10);ballMapGreen(4,19,0,0,10);ballMapGreen(4,20,0,0,10);ballMapGreen(4,23,0,0,10);ballMapGreen(4,24,0,0,10);ballMapGreen(4,26,0,0,10);ballMapGreen(4,27,0,0,10);ballMapGreen(4,28,0,0,10);ballMapGreen(4,29,0,0,10);ballMapGreen(5,17,0,0,10);ballMapGreen(5,18,0,0,10);ballMapGreen(5,19,0,0,10);ballMapGreen(5,20,0,0,10);ballMapGreen(5,26,0,0,10);ballMapGreen(5,27,0,0,10);ballMapGreen(5,28,0,0,10);ballMapGreen(5,29,0,0,10);ballMapGreen(6,18,0,0,10);ballMapGreen(6,19,0,0,10);ballMapGreen(6,27,0,0,10);ballMapGreen(6,28,0,0,10);

ballStep();

ballMapBlue(2,2,0,0,10);ballMapBlue(2,3,0,0,10);ballMapBlue(2,7,0,0,10);ballMapBlue(2,8,0,0,10);ballMapBlue(2,11,0,0,10);ballMapBlue(2,12,0,0,10);ballMapBlue(3,1,0,0,10);ballMapBlue(3,2,0,0,10);ballMapBlue(3,3,0,0,10);ballMapBlue(3,4,0,0,10);ballMapBlue(3,6,0,0,10);ballMapBlue(3,7,0,0,10);ballMapBlue(3,8,0,0,10);ballMapBlue(3,9,0,0,10);ballMapBlue(3,10,0,0,10);ballMapBlue(3,11,0,0,10);ballMapBlue(3,12,0,0,10);ballMapBlue(3,13,0,0,10);ballMapBlue(4,1,0,0,10);ballMapBlue(4,2,0,0,10);ballMapBlue(4,3,0,0,10);ballMapBlue(4,4,0,0,10);ballMapBlue(4,6,0,0,10);ballMapBlue(4,7,0,0,10);ballMapBlue(4,8,0,0,10);ballMapBlue(4,9,0,0,10);ballMapBlue(4,10,0,0,10);ballMapBlue(4,11,0,0,10);ballMapBlue(4,12,0,0,10);ballMapBlue(4,13,0,0,10);ballMapBlue(5,2,0,0,10);ballMapBlue(5,3,0,0,10);ballMapBlue(5,7,0,0,10);ballMapBlue(5,8,0,0,10);ballMapBlue(5,11,0,0,10);ballMapBlue(5,12,0,0,10);ballMapBlue(2,18,0,0,10);ballMapBlue(2,19,0,0,10);ballMapBlue(2,23,0,0,10);ballMapBlue(2,24,0,0,10);ballMapBlue(2,27,0,0,10);ballMapBlue(2,28,0,0,10);ballMapBlue(3,17,0,0,10);ballMapBlue(3,18,0,0,10);ballMapBlue(3,19,0,0,10);ballMapBlue(3,20,0,0,10);ballMapBlue(3,22,0,0,10);ballMapBlue(3,23,0,0,10);ballMapBlue(3,24,0,0,10);ballMapBlue(3,25,0,0,10);ballMapBlue(3,26,0,0,10);ballMapBlue(3,27,0,0,10);ballMapBlue(3,28,0,0,10);ballMapBlue(3,29,0,0,10);ballMapBlue(4,17,0,0,10);ballMapBlue(4,18,0,0,10);ballMapBlue(4,19,0,0,10);ballMapBlue(4,20,0,0,10);ballMapBlue(4,22,0,0,10);ballMapBlue(4,23,0,0,10);ballMapBlue(4,24,0,0,10);ballMapBlue(4,25,0,0,10);ballMapBlue(4,26,0,0,10);ballMapBlue(4,27,0,0,10);ballMapBlue(4,28,0,0,10);ballMapBlue(4,29,0,0,10);ballMapBlue(5,18,0,0,10);ballMapBlue(5,19,0,0,10);ballMapBlue(5,23,0,0,10);ballMapBlue(5,24,0,0,10);ballMapBlue(5,27,0,0,10);ballMapBlue(5,28,0,0,10);
ballColorStep();
ballMapRed(2,2,0,0,10);ballMapRed(2,3,0,0,10);ballMapRed(2,7,0,0,10);ballMapRed(2,8,0,0,10);ballMapRed(2,11,0,0,10);ballMapRed(2,12,0,0,10);ballMapRed(3,1,0,0,10);ballMapRed(3,2,0,0,10);ballMapRed(3,3,0,0,10);ballMapRed(3,4,0,0,10);ballMapRed(3,6,0,0,10);ballMapRed(3,7,0,0,10);ballMapRed(3,8,0,0,10);ballMapRed(3,9,0,0,10);ballMapRed(3,10,0,0,10);ballMapRed(3,11,0,0,10);ballMapRed(3,12,0,0,10);ballMapRed(3,13,0,0,10);ballMapRed(4,1,0,0,10);ballMapRed(4,2,0,0,10);ballMapRed(4,3,0,0,10);ballMapRed(4,4,0,0,10);ballMapRed(4,6,0,0,10);ballMapRed(4,7,0,0,10);ballMapRed(4,8,0,0,10);ballMapRed(4,9,0,0,10);ballMapRed(4,10,0,0,10);ballMapRed(4,11,0,0,10);ballMapRed(4,12,0,0,10);ballMapRed(4,13,0,0,10);ballMapRed(5,2,0,0,10);ballMapRed(5,3,0,0,10);ballMapRed(5,7,0,0,10);ballMapRed(5,8,0,0,10);ballMapRed(5,11,0,0,10);ballMapRed(5,12,0,0,10);ballMapRed(2,18,0,0,10);ballMapRed(2,19,0,0,10);ballMapRed(2,23,0,0,10);ballMapRed(2,24,0,0,10);ballMapRed(2,27,0,0,10);ballMapRed(2,28,0,0,10);ballMapRed(3,17,0,0,10);ballMapRed(3,18,0,0,10);ballMapRed(3,19,0,0,10);ballMapRed(3,20,0,0,10);ballMapRed(3,22,0,0,10);ballMapRed(3,23,0,0,10);ballMapRed(3,24,0,0,10);ballMapRed(3,25,0,0,10);ballMapRed(3,26,0,0,10);ballMapRed(3,27,0,0,10);ballMapRed(3,28,0,0,10);ballMapRed(3,29,0,0,10);ballMapRed(4,17,0,0,10);ballMapRed(4,18,0,0,10);ballMapRed(4,19,0,0,10);ballMapRed(4,20,0,0,10);ballMapRed(4,22,0,0,10);ballMapRed(4,23,0,0,10);ballMapRed(4,24,0,0,10);ballMapRed(4,25,0,0,10);ballMapRed(4,26,0,0,10);ballMapRed(4,27,0,0,10);ballMapRed(4,28,0,0,10);ballMapRed(4,29,0,0,10);ballMapRed(5,18,0,0,10);ballMapRed(5,19,0,0,10);ballMapRed(5,23,0,0,10);ballMapRed(5,24,0,0,10);ballMapRed(5,27,0,0,10);ballMapRed(5,28,0,0,10);
ballColorStep();
ballMapGreen(2,2,0,0,10);ballMapGreen(2,3,0,0,10);ballMapGreen(2,7,0,0,10);ballMapGreen(2,8,0,0,10);ballMapGreen(2,11,0,0,10);ballMapGreen(2,12,0,0,10);ballMapGreen(3,1,0,0,10);ballMapGreen(3,2,0,0,10);ballMapGreen(3,3,0,0,10);ballMapGreen(3,4,0,0,10);ballMapGreen(3,6,0,0,10);ballMapGreen(3,7,0,0,10);ballMapGreen(3,8,0,0,10);ballMapGreen(3,9,0,0,10);ballMapGreen(3,10,0,0,10);ballMapGreen(3,11,0,0,10);ballMapGreen(3,12,0,0,10);ballMapGreen(3,13,0,0,10);ballMapGreen(4,1,0,0,10);ballMapGreen(4,2,0,0,10);ballMapGreen(4,3,0,0,10);ballMapGreen(4,4,0,0,10);ballMapGreen(4,6,0,0,10);ballMapGreen(4,7,0,0,10);ballMapGreen(4,8,0,0,10);ballMapGreen(4,9,0,0,10);ballMapGreen(4,10,0,0,10);ballMapGreen(4,11,0,0,10);ballMapGreen(4,12,0,0,10);ballMapGreen(4,13,0,0,10);ballMapGreen(5,2,0,0,10);ballMapGreen(5,3,0,0,10);ballMapGreen(5,7,0,0,10);ballMapGreen(5,8,0,0,10);ballMapGreen(5,11,0,0,10);ballMapGreen(5,12,0,0,10);ballMapGreen(2,18,0,0,10);ballMapGreen(2,19,0,0,10);ballMapGreen(2,23,0,0,10);ballMapGreen(2,24,0,0,10);ballMapGreen(2,27,0,0,10);ballMapGreen(2,28,0,0,10);ballMapGreen(3,17,0,0,10);ballMapGreen(3,18,0,0,10);ballMapGreen(3,19,0,0,10);ballMapGreen(3,20,0,0,10);ballMapGreen(3,22,0,0,10);ballMapGreen(3,23,0,0,10);ballMapGreen(3,24,0,0,10);ballMapGreen(3,25,0,0,10);ballMapGreen(3,26,0,0,10);ballMapGreen(3,27,0,0,10);ballMapGreen(3,28,0,0,10);ballMapGreen(3,29,0,0,10);ballMapGreen(4,17,0,0,10);ballMapGreen(4,18,0,0,10);ballMapGreen(4,19,0,0,10);ballMapGreen(4,20,0,0,10);ballMapGreen(4,22,0,0,10);ballMapGreen(4,23,0,0,10);ballMapGreen(4,24,0,0,10);ballMapGreen(4,25,0,0,10);ballMapGreen(4,26,0,0,10);ballMapGreen(4,27,0,0,10);ballMapGreen(4,28,0,0,10);ballMapGreen(4,29,0,0,10);ballMapGreen(5,18,0,0,10);ballMapGreen(5,19,0,0,10);ballMapGreen(5,23,0,0,10);ballMapGreen(5,24,0,0,10);ballMapGreen(5,27,0,0,10);ballMapGreen(5,28,0,0,10);

ballStep();

ballMapBlue(1,7,0,0,10);ballMapBlue(1,8,0,0,10);ballMapBlue(2,6,0,0,10);ballMapBlue(2,7,0,0,10);ballMapBlue(2,8,0,0,10);ballMapBlue(2,9,0,0,10);ballMapBlue(3,2,0,0,10);ballMapBlue(3,3,0,0,10);ballMapBlue(3,6,0,0,10);ballMapBlue(3,7,0,0,10);ballMapBlue(3,8,0,0,10);ballMapBlue(3,9,0,0,10);ballMapBlue(3,11,0,0,10);ballMapBlue(3,12,0,0,10);ballMapBlue(4,1,0,0,10);ballMapBlue(4,2,0,0,10);ballMapBlue(4,3,0,0,10);ballMapBlue(4,4,0,0,10);ballMapBlue(4,7,0,0,10);ballMapBlue(4,8,0,0,10);ballMapBlue(4,10,0,0,10);ballMapBlue(4,11,0,0,10);ballMapBlue(4,12,0,0,10);ballMapBlue(4,13,0,0,10);ballMapBlue(5,1,0,0,10);ballMapBlue(5,2,0,0,10);ballMapBlue(5,3,0,0,10);ballMapBlue(5,4,0,0,10);ballMapBlue(5,10,0,0,10);ballMapBlue(5,11,0,0,10);ballMapBlue(5,12,0,0,10);ballMapBlue(5,13,0,0,10);ballMapBlue(6,2,0,0,10);ballMapBlue(6,3,0,0,10);ballMapBlue(6,11,0,0,10);ballMapBlue(6,12,0,0,10);ballMapBlue(1,18,0,0,10);ballMapBlue(1,19,0,0,10);ballMapBlue(1,27,0,0,10);ballMapBlue(1,28,0,0,10);ballMapBlue(2,17,0,0,10);ballMapBlue(2,18,0,0,10);ballMapBlue(2,19,0,0,10);ballMapBlue(2,20,0,0,10);ballMapBlue(2,26,0,0,10);ballMapBlue(2,27,0,0,10);ballMapBlue(2,28,0,0,10);ballMapBlue(2,29,0,0,10);ballMapBlue(3,17,0,0,10);ballMapBlue(3,18,0,0,10);ballMapBlue(3,19,0,0,10);ballMapBlue(3,20,0,0,10);ballMapBlue(3,23,0,0,10);ballMapBlue(3,24,0,0,10);ballMapBlue(3,26,0,0,10);ballMapBlue(3,27,0,0,10);ballMapBlue(3,28,0,0,10);ballMapBlue(3,29,0,0,10);ballMapBlue(4,18,0,0,10);ballMapBlue(4,19,0,0,10);ballMapBlue(4,22,0,0,10);ballMapBlue(4,23,0,0,10);ballMapBlue(4,24,0,0,10);ballMapBlue(4,25,0,0,10);ballMapBlue(4,27,0,0,10);ballMapBlue(4,28,0,0,10);ballMapBlue(5,22,0,0,10);ballMapBlue(5,23,0,0,10);ballMapBlue(5,24,0,0,10);ballMapBlue(5,25,0,0,10);ballMapBlue(6,23,0,0,10);ballMapBlue(6,24,0,0,10);
ballColorStep();
ballMapRed(1,7,0,0,10);ballMapRed(1,8,0,0,10);ballMapRed(2,6,0,0,10);ballMapRed(2,7,0,0,10);ballMapRed(2,8,0,0,10);ballMapRed(2,9,0,0,10);ballMapRed(3,2,0,0,10);ballMapRed(3,3,0,0,10);ballMapRed(3,6,0,0,10);ballMapRed(3,7,0,0,10);ballMapRed(3,8,0,0,10);ballMapRed(3,9,0,0,10);ballMapRed(3,11,0,0,10);ballMapRed(3,12,0,0,10);ballMapRed(4,1,0,0,10);ballMapRed(4,2,0,0,10);ballMapRed(4,3,0,0,10);ballMapRed(4,4,0,0,10);ballMapRed(4,7,0,0,10);ballMapRed(4,8,0,0,10);ballMapRed(4,10,0,0,10);ballMapRed(4,11,0,0,10);ballMapRed(4,12,0,0,10);ballMapRed(4,13,0,0,10);ballMapRed(5,1,0,0,10);ballMapRed(5,2,0,0,10);ballMapRed(5,3,0,0,10);ballMapRed(5,4,0,0,10);ballMapRed(5,10,0,0,10);ballMapRed(5,11,0,0,10);ballMapRed(5,12,0,0,10);ballMapRed(5,13,0,0,10);ballMapRed(6,2,0,0,10);ballMapRed(6,3,0,0,10);ballMapRed(6,11,0,0,10);ballMapRed(6,12,0,0,10);ballMapRed(1,18,0,0,10);ballMapRed(1,19,0,0,10);ballMapRed(1,27,0,0,10);ballMapRed(1,28,0,0,10);ballMapRed(2,17,0,0,10);ballMapRed(2,18,0,0,10);ballMapRed(2,19,0,0,10);ballMapRed(2,20,0,0,10);ballMapRed(2,26,0,0,10);ballMapRed(2,27,0,0,10);ballMapRed(2,28,0,0,10);ballMapRed(2,29,0,0,10);ballMapRed(3,17,0,0,10);ballMapRed(3,18,0,0,10);ballMapRed(3,19,0,0,10);ballMapRed(3,20,0,0,10);ballMapRed(3,23,0,0,10);ballMapRed(3,24,0,0,10);ballMapRed(3,26,0,0,10);ballMapRed(3,27,0,0,10);ballMapRed(3,28,0,0,10);ballMapRed(3,29,0,0,10);ballMapRed(4,18,0,0,10);ballMapRed(4,19,0,0,10);ballMapRed(4,22,0,0,10);ballMapRed(4,23,0,0,10);ballMapRed(4,24,0,0,10);ballMapRed(4,25,0,0,10);ballMapRed(4,27,0,0,10);ballMapRed(4,28,0,0,10);ballMapRed(5,22,0,0,10);ballMapRed(5,23,0,0,10);ballMapRed(5,24,0,0,10);ballMapRed(5,25,0,0,10);ballMapRed(6,23,0,0,10);ballMapRed(6,24,0,0,10);
ballColorStep();
ballMapGreen(1,7,0,0,10);ballMapGreen(1,8,0,0,10);ballMapGreen(2,6,0,0,10);ballMapGreen(2,7,0,0,10);ballMapGreen(2,8,0,0,10);ballMapGreen(2,9,0,0,10);ballMapGreen(3,2,0,0,10);ballMapGreen(3,3,0,0,10);ballMapGreen(3,6,0,0,10);ballMapGreen(3,7,0,0,10);ballMapGreen(3,8,0,0,10);ballMapGreen(3,9,0,0,10);ballMapGreen(3,11,0,0,10);ballMapGreen(3,12,0,0,10);ballMapGreen(4,1,0,0,10);ballMapGreen(4,2,0,0,10);ballMapGreen(4,3,0,0,10);ballMapGreen(4,4,0,0,10);ballMapGreen(4,7,0,0,10);ballMapGreen(4,8,0,0,10);ballMapGreen(4,10,0,0,10);ballMapGreen(4,11,0,0,10);ballMapGreen(4,12,0,0,10);ballMapGreen(4,13,0,0,10);ballMapGreen(5,1,0,0,10);ballMapGreen(5,2,0,0,10);ballMapGreen(5,3,0,0,10);ballMapGreen(5,4,0,0,10);ballMapGreen(5,10,0,0,10);ballMapGreen(5,11,0,0,10);ballMapGreen(5,12,0,0,10);ballMapGreen(5,13,0,0,10);ballMapGreen(6,2,0,0,10);ballMapGreen(6,3,0,0,10);ballMapGreen(6,11,0,0,10);ballMapGreen(6,12,0,0,10);ballMapGreen(1,18,0,0,10);ballMapGreen(1,19,0,0,10);ballMapGreen(1,27,0,0,10);ballMapGreen(1,28,0,0,10);ballMapGreen(2,17,0,0,10);ballMapGreen(2,18,0,0,10);ballMapGreen(2,19,0,0,10);ballMapGreen(2,20,0,0,10);ballMapGreen(2,26,0,0,10);ballMapGreen(2,27,0,0,10);ballMapGreen(2,28,0,0,10);ballMapGreen(2,29,0,0,10);ballMapGreen(3,17,0,0,10);ballMapGreen(3,18,0,0,10);ballMapGreen(3,19,0,0,10);ballMapGreen(3,20,0,0,10);ballMapGreen(3,23,0,0,10);ballMapGreen(3,24,0,0,10);ballMapGreen(3,26,0,0,10);ballMapGreen(3,27,0,0,10);ballMapGreen(3,28,0,0,10);ballMapGreen(3,29,0,0,10);ballMapGreen(4,18,0,0,10);ballMapGreen(4,19,0,0,10);ballMapGreen(4,22,0,0,10);ballMapGreen(4,23,0,0,10);ballMapGreen(4,24,0,0,10);ballMapGreen(4,25,0,0,10);ballMapGreen(4,27,0,0,10);ballMapGreen(4,28,0,0,10);ballMapGreen(5,22,0,0,10);ballMapGreen(5,23,0,0,10);ballMapGreen(5,24,0,0,10);ballMapGreen(5,25,0,0,10);ballMapGreen(6,23,0,0,10);ballMapGreen(6,24,0,0,10);

ballStep();

ballMapBlue(0,7,0,0,10);ballMapBlue(0,8,0,0,10);ballMapBlue(1,6,0,0,10);ballMapBlue(1,7,0,0,10);ballMapBlue(1,8,0,0,10);ballMapBlue(1,9,0,0,10);ballMapBlue(2,6,0,0,10);ballMapBlue(2,7,0,0,10);ballMapBlue(2,8,0,0,10);ballMapBlue(2,9,0,0,10);ballMapBlue(3,7,0,0,10);ballMapBlue(3,8,0,0,10);ballMapBlue(4,2,0,0,10);ballMapBlue(4,3,0,0,10);ballMapBlue(4,11,0,0,10);ballMapBlue(4,12,0,0,10);ballMapBlue(5,1,0,0,10);ballMapBlue(5,2,0,0,10);ballMapBlue(5,3,0,0,10);ballMapBlue(5,4,0,0,10);ballMapBlue(5,10,0,0,10);ballMapBlue(5,11,0,0,10);ballMapBlue(5,12,0,0,10);ballMapBlue(5,13,0,0,10);ballMapBlue(6,1,0,0,10);ballMapBlue(6,2,0,0,10);ballMapBlue(6,3,0,0,10);ballMapBlue(6,4,0,0,10);ballMapBlue(6,10,0,0,10);ballMapBlue(6,11,0,0,10);ballMapBlue(6,12,0,0,10);ballMapBlue(6,13,0,0,10);ballMapBlue(7,2,0,0,10);ballMapBlue(7,3,0,0,10);ballMapBlue(7,11,0,0,10);ballMapBlue(7,12,0,0,10);ballMapBlue(0,18,0,0,10);ballMapBlue(0,19,0,0,10);ballMapBlue(0,27,0,0,10);ballMapBlue(0,28,0,0,10);ballMapBlue(1,17,0,0,10);ballMapBlue(1,18,0,0,10);ballMapBlue(1,19,0,0,10);ballMapBlue(1,20,0,0,10);ballMapBlue(1,26,0,0,10);ballMapBlue(1,27,0,0,10);ballMapBlue(1,28,0,0,10);ballMapBlue(1,29,0,0,10);ballMapBlue(2,17,0,0,10);ballMapBlue(2,18,0,0,10);ballMapBlue(2,19,0,0,10);ballMapBlue(2,20,0,0,10);ballMapBlue(2,26,0,0,10);ballMapBlue(2,27,0,0,10);ballMapBlue(2,28,0,0,10);ballMapBlue(2,29,0,0,10);ballMapBlue(3,18,0,0,10);ballMapBlue(3,19,0,0,10);ballMapBlue(3,27,0,0,10);ballMapBlue(3,28,0,0,10);ballMapBlue(4,23,0,0,10);ballMapBlue(4,24,0,0,10);ballMapBlue(5,22,0,0,10);ballMapBlue(5,23,0,0,10);ballMapBlue(5,24,0,0,10);ballMapBlue(5,25,0,0,10);ballMapBlue(6,22,0,0,10);ballMapBlue(6,23,0,0,10);ballMapBlue(6,24,0,0,10);ballMapBlue(6,25,0,0,10);ballMapBlue(7,23,0,0,10);ballMapBlue(7,24,0,0,10);
ballColorStep();
ballMapRed(0,7,0,0,10);ballMapRed(0,8,0,0,10);ballMapRed(1,6,0,0,10);ballMapRed(1,7,0,0,10);ballMapRed(1,8,0,0,10);ballMapRed(1,9,0,0,10);ballMapRed(2,6,0,0,10);ballMapRed(2,7,0,0,10);ballMapRed(2,8,0,0,10);ballMapRed(2,9,0,0,10);ballMapRed(3,7,0,0,10);ballMapRed(3,8,0,0,10);ballMapRed(4,2,0,0,10);ballMapRed(4,3,0,0,10);ballMapRed(4,11,0,0,10);ballMapRed(4,12,0,0,10);ballMapRed(5,1,0,0,10);ballMapRed(5,2,0,0,10);ballMapRed(5,3,0,0,10);ballMapRed(5,4,0,0,10);ballMapRed(5,10,0,0,10);ballMapRed(5,11,0,0,10);ballMapRed(5,12,0,0,10);ballMapRed(5,13,0,0,10);ballMapRed(6,1,0,0,10);ballMapRed(6,2,0,0,10);ballMapRed(6,3,0,0,10);ballMapRed(6,4,0,0,10);ballMapRed(6,10,0,0,10);ballMapRed(6,11,0,0,10);ballMapRed(6,12,0,0,10);ballMapRed(6,13,0,0,10);ballMapRed(7,2,0,0,10);ballMapRed(7,3,0,0,10);ballMapRed(7,11,0,0,10);ballMapRed(7,12,0,0,10);ballMapRed(0,18,0,0,10);ballMapRed(0,19,0,0,10);ballMapRed(0,27,0,0,10);ballMapRed(0,28,0,0,10);ballMapRed(1,17,0,0,10);ballMapRed(1,18,0,0,10);ballMapRed(1,19,0,0,10);ballMapRed(1,20,0,0,10);ballMapRed(1,26,0,0,10);ballMapRed(1,27,0,0,10);ballMapRed(1,28,0,0,10);ballMapRed(1,29,0,0,10);ballMapRed(2,17,0,0,10);ballMapRed(2,18,0,0,10);ballMapRed(2,19,0,0,10);ballMapRed(2,20,0,0,10);ballMapRed(2,26,0,0,10);ballMapRed(2,27,0,0,10);ballMapRed(2,28,0,0,10);ballMapRed(2,29,0,0,10);ballMapRed(3,18,0,0,10);ballMapRed(3,19,0,0,10);ballMapRed(3,27,0,0,10);ballMapRed(3,28,0,0,10);ballMapRed(4,23,0,0,10);ballMapRed(4,24,0,0,10);ballMapRed(5,22,0,0,10);ballMapRed(5,23,0,0,10);ballMapRed(5,24,0,0,10);ballMapRed(5,25,0,0,10);ballMapRed(6,22,0,0,10);ballMapRed(6,23,0,0,10);ballMapRed(6,24,0,0,10);ballMapRed(6,25,0,0,10);ballMapRed(7,23,0,0,10);ballMapRed(7,24,0,0,10);
ballColorStep();
ballMapGreen(0,7,0,0,10);ballMapGreen(0,8,0,0,10);ballMapGreen(1,6,0,0,10);ballMapGreen(1,7,0,0,10);ballMapGreen(1,8,0,0,10);ballMapGreen(1,9,0,0,10);ballMapGreen(2,6,0,0,10);ballMapGreen(2,7,0,0,10);ballMapGreen(2,8,0,0,10);ballMapGreen(2,9,0,0,10);ballMapGreen(3,7,0,0,10);ballMapGreen(3,8,0,0,10);ballMapGreen(4,2,0,0,10);ballMapGreen(4,3,0,0,10);ballMapGreen(4,11,0,0,10);ballMapGreen(4,12,0,0,10);ballMapGreen(5,1,0,0,10);ballMapGreen(5,2,0,0,10);ballMapGreen(5,3,0,0,10);ballMapGreen(5,4,0,0,10);ballMapGreen(5,10,0,0,10);ballMapGreen(5,11,0,0,10);ballMapGreen(5,12,0,0,10);ballMapGreen(5,13,0,0,10);ballMapGreen(6,1,0,0,10);ballMapGreen(6,2,0,0,10);ballMapGreen(6,3,0,0,10);ballMapGreen(6,4,0,0,10);ballMapGreen(6,10,0,0,10);ballMapGreen(6,11,0,0,10);ballMapGreen(6,12,0,0,10);ballMapGreen(6,13,0,0,10);ballMapGreen(7,2,0,0,10);ballMapGreen(7,3,0,0,10);ballMapGreen(7,11,0,0,10);ballMapGreen(7,12,0,0,10);ballMapGreen(0,18,0,0,10);ballMapGreen(0,19,0,0,10);ballMapGreen(0,27,0,0,10);ballMapGreen(0,28,0,0,10);ballMapGreen(1,17,0,0,10);ballMapGreen(1,18,0,0,10);ballMapGreen(1,19,0,0,10);ballMapGreen(1,20,0,0,10);ballMapGreen(1,26,0,0,10);ballMapGreen(1,27,0,0,10);ballMapGreen(1,28,0,0,10);ballMapGreen(1,29,0,0,10);ballMapGreen(2,17,0,0,10);ballMapGreen(2,18,0,0,10);ballMapGreen(2,19,0,0,10);ballMapGreen(2,20,0,0,10);ballMapGreen(2,26,0,0,10);ballMapGreen(2,27,0,0,10);ballMapGreen(2,28,0,0,10);ballMapGreen(2,29,0,0,10);ballMapGreen(3,18,0,0,10);ballMapGreen(3,19,0,0,10);ballMapGreen(3,27,0,0,10);ballMapGreen(3,28,0,0,10);ballMapGreen(4,23,0,0,10);ballMapGreen(4,24,0,0,10);ballMapGreen(5,22,0,0,10);ballMapGreen(5,23,0,0,10);ballMapGreen(5,24,0,0,10);ballMapGreen(5,25,0,0,10);ballMapGreen(6,22,0,0,10);ballMapGreen(6,23,0,0,10);ballMapGreen(6,24,0,0,10);ballMapGreen(6,25,0,0,10);ballMapGreen(7,23,0,0,10);ballMapGreen(7,24,0,0,10);

ballStep();

ballMapBlue(0,6,0,0,10);ballMapBlue(0,7,0,0,10);ballMapBlue(0,8,0,0,10);ballMapBlue(0,9,0,0,10);ballMapBlue(1,6,0,0,10);ballMapBlue(1,7,0,0,10);ballMapBlue(1,8,0,0,10);ballMapBlue(1,9,0,0,10);ballMapBlue(2,7,0,0,10);ballMapBlue(2,8,0,0,10);ballMapBlue(5,2,0,0,10);ballMapBlue(5,3,0,0,10);ballMapBlue(5,11,0,0,10);ballMapBlue(5,12,0,0,10);ballMapBlue(6,1,0,0,10);ballMapBlue(6,2,0,0,10);ballMapBlue(6,3,0,0,10);ballMapBlue(6,4,0,0,10);ballMapBlue(6,10,0,0,10);ballMapBlue(6,11,0,0,10);ballMapBlue(6,12,0,0,10);ballMapBlue(6,13,0,0,10);ballMapBlue(7,1,0,0,10);ballMapBlue(7,2,0,0,10);ballMapBlue(7,3,0,0,10);ballMapBlue(7,4,0,0,10);ballMapBlue(7,10,0,0,10);ballMapBlue(7,11,0,0,10);ballMapBlue(7,12,0,0,10);ballMapBlue(7,13,0,0,10);ballMapBlue(0,17,0,0,10);ballMapBlue(0,18,0,0,10);ballMapBlue(0,19,0,0,10);ballMapBlue(0,20,0,0,10);ballMapBlue(0,26,0,0,10);ballMapBlue(0,27,0,0,10);ballMapBlue(0,28,0,0,10);ballMapBlue(0,29,0,0,10);ballMapBlue(1,17,0,0,10);ballMapBlue(1,18,0,0,10);ballMapBlue(1,19,0,0,10);ballMapBlue(1,20,0,0,10);ballMapBlue(1,26,0,0,10);ballMapBlue(1,27,0,0,10);ballMapBlue(1,28,0,0,10);ballMapBlue(1,29,0,0,10);ballMapBlue(2,18,0,0,10);ballMapBlue(2,19,0,0,10);ballMapBlue(2,27,0,0,10);ballMapBlue(2,28,0,0,10);ballMapBlue(5,23,0,0,10);ballMapBlue(5,24,0,0,10);ballMapBlue(6,22,0,0,10);ballMapBlue(6,23,0,0,10);ballMapBlue(6,24,0,0,10);ballMapBlue(6,25,0,0,10);ballMapBlue(7,22,0,0,10);ballMapBlue(7,23,0,0,10);ballMapBlue(7,24,0,0,10);ballMapBlue(7,25,0,0,10);
ballColorStep();
ballMapRed(0,6,0,0,10);ballMapRed(0,7,0,0,10);ballMapRed(0,8,0,0,10);ballMapRed(0,9,0,0,10);ballMapRed(1,6,0,0,10);ballMapRed(1,7,0,0,10);ballMapRed(1,8,0,0,10);ballMapRed(1,9,0,0,10);ballMapRed(2,7,0,0,10);ballMapRed(2,8,0,0,10);ballMapRed(5,2,0,0,10);ballMapRed(5,3,0,0,10);ballMapRed(5,11,0,0,10);ballMapRed(5,12,0,0,10);ballMapRed(6,1,0,0,10);ballMapRed(6,2,0,0,10);ballMapRed(6,3,0,0,10);ballMapRed(6,4,0,0,10);ballMapRed(6,10,0,0,10);ballMapRed(6,11,0,0,10);ballMapRed(6,12,0,0,10);ballMapRed(6,13,0,0,10);ballMapRed(7,1,0,0,10);ballMapRed(7,2,0,0,10);ballMapRed(7,3,0,0,10);ballMapRed(7,4,0,0,10);ballMapRed(7,10,0,0,10);ballMapRed(7,11,0,0,10);ballMapRed(7,12,0,0,10);ballMapRed(7,13,0,0,10);ballMapRed(0,17,0,0,10);ballMapRed(0,18,0,0,10);ballMapRed(0,19,0,0,10);ballMapRed(0,20,0,0,10);ballMapRed(0,26,0,0,10);ballMapRed(0,27,0,0,10);ballMapRed(0,28,0,0,10);ballMapRed(0,29,0,0,10);ballMapRed(1,17,0,0,10);ballMapRed(1,18,0,0,10);ballMapRed(1,19,0,0,10);ballMapRed(1,20,0,0,10);ballMapRed(1,26,0,0,10);ballMapRed(1,27,0,0,10);ballMapRed(1,28,0,0,10);ballMapRed(1,29,0,0,10);ballMapRed(2,18,0,0,10);ballMapRed(2,19,0,0,10);ballMapRed(2,27,0,0,10);ballMapRed(2,28,0,0,10);ballMapRed(5,23,0,0,10);ballMapRed(5,24,0,0,10);ballMapRed(6,22,0,0,10);ballMapRed(6,23,0,0,10);ballMapRed(6,24,0,0,10);ballMapRed(6,25,0,0,10);ballMapRed(7,22,0,0,10);ballMapRed(7,23,0,0,10);ballMapRed(7,24,0,0,10);ballMapRed(7,25,0,0,10);
ballColorStep();
ballMapGreen(0,6,0,0,10);ballMapGreen(0,7,0,0,10);ballMapGreen(0,8,0,0,10);ballMapGreen(0,9,0,0,10);ballMapGreen(1,6,0,0,10);ballMapGreen(1,7,0,0,10);ballMapGreen(1,8,0,0,10);ballMapGreen(1,9,0,0,10);ballMapGreen(2,7,0,0,10);ballMapGreen(2,8,0,0,10);ballMapGreen(5,2,0,0,10);ballMapGreen(5,3,0,0,10);ballMapGreen(5,11,0,0,10);ballMapGreen(5,12,0,0,10);ballMapGreen(6,1,0,0,10);ballMapGreen(6,2,0,0,10);ballMapGreen(6,3,0,0,10);ballMapGreen(6,4,0,0,10);ballMapGreen(6,10,0,0,10);ballMapGreen(6,11,0,0,10);ballMapGreen(6,12,0,0,10);ballMapGreen(6,13,0,0,10);ballMapGreen(7,1,0,0,10);ballMapGreen(7,2,0,0,10);ballMapGreen(7,3,0,0,10);ballMapGreen(7,4,0,0,10);ballMapGreen(7,10,0,0,10);ballMapGreen(7,11,0,0,10);ballMapGreen(7,12,0,0,10);ballMapGreen(7,13,0,0,10);ballMapGreen(0,17,0,0,10);ballMapGreen(0,18,0,0,10);ballMapGreen(0,19,0,0,10);ballMapGreen(0,20,0,0,10);ballMapGreen(0,26,0,0,10);ballMapGreen(0,27,0,0,10);ballMapGreen(0,28,0,0,10);ballMapGreen(0,29,0,0,10);ballMapGreen(1,17,0,0,10);ballMapGreen(1,18,0,0,10);ballMapGreen(1,19,0,0,10);ballMapGreen(1,20,0,0,10);ballMapGreen(1,26,0,0,10);ballMapGreen(1,27,0,0,10);ballMapGreen(1,28,0,0,10);ballMapGreen(1,29,0,0,10);ballMapGreen(2,18,0,0,10);ballMapGreen(2,19,0,0,10);ballMapGreen(2,27,0,0,10);ballMapGreen(2,28,0,0,10);ballMapGreen(5,23,0,0,10);ballMapGreen(5,24,0,0,10);ballMapGreen(6,22,0,0,10);ballMapGreen(6,23,0,0,10);ballMapGreen(6,24,0,0,10);ballMapGreen(6,25,0,0,10);ballMapGreen(7,22,0,0,10);ballMapGreen(7,23,0,0,10);ballMapGreen(7,24,0,0,10);ballMapGreen(7,25,0,0,10);

ballStep();

ballMapBlue(0,6,0,0,10);ballMapBlue(0,7,0,0,10);ballMapBlue(0,8,0,0,10);ballMapBlue(0,9,0,0,10);ballMapBlue(1,7,0,0,10);ballMapBlue(1,8,0,0,10);ballMapBlue(6,2,0,0,10);ballMapBlue(6,3,0,0,10);ballMapBlue(6,11,0,0,10);ballMapBlue(6,12,0,0,10);ballMapBlue(7,1,0,0,10);ballMapBlue(7,2,0,0,10);ballMapBlue(7,3,0,0,10);ballMapBlue(7,4,0,0,10);ballMapBlue(7,10,0,0,10);ballMapBlue(7,11,0,0,10);ballMapBlue(7,12,0,0,10);ballMapBlue(7,13,0,0,10);ballMapBlue(0,17,0,0,10);ballMapBlue(0,18,0,0,10);ballMapBlue(0,19,0,0,10);ballMapBlue(0,20,0,0,10);ballMapBlue(0,26,0,0,10);ballMapBlue(0,27,0,0,10);ballMapBlue(0,28,0,0,10);ballMapBlue(0,29,0,0,10);ballMapBlue(1,18,0,0,10);ballMapBlue(1,19,0,0,10);ballMapBlue(1,27,0,0,10);ballMapBlue(1,28,0,0,10);ballMapBlue(6,23,0,0,10);ballMapBlue(6,24,0,0,10);ballMapBlue(7,22,0,0,10);ballMapBlue(7,23,0,0,10);ballMapBlue(7,24,0,0,10);ballMapBlue(7,25,0,0,10);
ballColorStep();
ballMapRed(0,6,0,0,10);ballMapRed(0,7,0,0,10);ballMapRed(0,8,0,0,10);ballMapRed(0,9,0,0,10);ballMapRed(1,7,0,0,10);ballMapRed(1,8,0,0,10);ballMapRed(6,2,0,0,10);ballMapRed(6,3,0,0,10);ballMapRed(6,11,0,0,10);ballMapRed(6,12,0,0,10);ballMapRed(7,1,0,0,10);ballMapRed(7,2,0,0,10);ballMapRed(7,3,0,0,10);ballMapRed(7,4,0,0,10);ballMapRed(7,10,0,0,10);ballMapRed(7,11,0,0,10);ballMapRed(7,12,0,0,10);ballMapRed(7,13,0,0,10);ballMapRed(0,17,0,0,10);ballMapRed(0,18,0,0,10);ballMapRed(0,19,0,0,10);ballMapRed(0,20,0,0,10);ballMapRed(0,26,0,0,10);ballMapRed(0,27,0,0,10);ballMapRed(0,28,0,0,10);ballMapRed(0,29,0,0,10);ballMapRed(1,18,0,0,10);ballMapRed(1,19,0,0,10);ballMapRed(1,27,0,0,10);ballMapRed(1,28,0,0,10);ballMapRed(6,23,0,0,10);ballMapRed(6,24,0,0,10);ballMapRed(7,22,0,0,10);ballMapRed(7,23,0,0,10);ballMapRed(7,24,0,0,10);ballMapRed(7,25,0,0,10);
ballColorStep();
ballMapGreen(0,6,0,0,10);ballMapGreen(0,7,0,0,10);ballMapGreen(0,8,0,0,10);ballMapGreen(0,9,0,0,10);ballMapGreen(1,7,0,0,10);ballMapGreen(1,8,0,0,10);ballMapGreen(6,2,0,0,10);ballMapGreen(6,3,0,0,10);ballMapGreen(6,11,0,0,10);ballMapGreen(6,12,0,0,10);ballMapGreen(7,1,0,0,10);ballMapGreen(7,2,0,0,10);ballMapGreen(7,3,0,0,10);ballMapGreen(7,4,0,0,10);ballMapGreen(7,10,0,0,10);ballMapGreen(7,11,0,0,10);ballMapGreen(7,12,0,0,10);ballMapGreen(7,13,0,0,10);ballMapGreen(0,17,0,0,10);ballMapGreen(0,18,0,0,10);ballMapGreen(0,19,0,0,10);ballMapGreen(0,20,0,0,10);ballMapGreen(0,26,0,0,10);ballMapGreen(0,27,0,0,10);ballMapGreen(0,28,0,0,10);ballMapGreen(0,29,0,0,10);ballMapGreen(1,18,0,0,10);ballMapGreen(1,19,0,0,10);ballMapGreen(1,27,0,0,10);ballMapGreen(1,28,0,0,10);ballMapGreen(6,23,0,0,10);ballMapGreen(6,24,0,0,10);ballMapGreen(7,22,0,0,10);ballMapGreen(7,23,0,0,10);ballMapGreen(7,24,0,0,10);ballMapGreen(7,25,0,0,10);

ballStep();

ballMapBlue(0,7,0,0,10);ballMapBlue(0,8,0,0,10);ballMapBlue(7,2,0,0,10);ballMapBlue(7,3,0,0,10);ballMapBlue(7,11,0,0,10);ballMapBlue(7,12,0,0,10);ballMapBlue(0,18,0,0,10);ballMapBlue(0,19,0,0,10);ballMapBlue(0,27,0,0,10);ballMapBlue(0,28,0,0,10);ballMapBlue(7,23,0,0,10);ballMapBlue(7,24,0,0,10);
ballColorStep();
ballMapRed(0,7,0,0,10);ballMapRed(0,8,0,0,10);ballMapRed(7,2,0,0,10);ballMapRed(7,3,0,0,10);ballMapRed(7,11,0,0,10);ballMapRed(7,12,0,0,10);ballMapRed(0,18,0,0,10);ballMapRed(0,19,0,0,10);ballMapRed(0,27,0,0,10);ballMapRed(0,28,0,0,10);ballMapRed(7,23,0,0,10);ballMapRed(7,24,0,0,10);
ballColorStep();
ballMapGreen(0,7,0,0,10);ballMapGreen(0,8,0,0,10);ballMapGreen(7,2,0,0,10);ballMapGreen(7,3,0,0,10);ballMapGreen(7,11,0,0,10);ballMapGreen(7,12,0,0,10);ballMapGreen(0,18,0,0,10);ballMapGreen(0,19,0,0,10);ballMapGreen(0,27,0,0,10);ballMapGreen(0,28,0,0,10);ballMapGreen(7,23,0,0,10);ballMapGreen(7,24,0,0,10);

ballStep();
}
void ballMapGreen(int x, int y, byte RED, byte  GREEN, byte BLUE) {
  int RGBlocation = 0;

  if (y % 2 == 0) { //even column
    RGBlocation = x + y * 8;

  } else { //odd column
    RGBlocation = 7 - x + y * 8;
  }
  strip.setPixelColor(RGBlocation,colors[colorPick]);
  
}
void ballMapRed(int x, int y, byte RED, byte  GREEN, byte BLUE) {
  int RGBlocation = 0;

  if (y % 2 == 0) { //even column
    RGBlocation = x + y * 8;

  } else { //odd column
    RGBlocation = 7 - x + y * 8;
  }
  strip.setPixelColor(RGBlocation,colors[1]);
  
}
void ballMapBlue(int x, int y, byte RED, byte  GREEN, byte BLUE) {
  int RGBlocation = 0;

  if (y % 2 == 0) { //even column
    RGBlocation = x + y * 8;

  } else { //odd column
    RGBlocation = 7 - x + y * 8;
  }
  strip.setPixelColor(RGBlocation,colors[3]);
  
}
void ballColorStep(){
  strip.show();
  delay(rate*3);
  clearLED();
  
}
void ballStep(){
  strip.show();
  delay(rate*3);
  clearLED();
}
void gfx(long prev){
  mapLEDXY(0,0,0,0,10);mapLEDXY(7,0,0,0,10);mapLEDXY(0,31,0,0,10);mapLEDXY(7,31,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  mapLEDXY(0,1,0,0,10);mapLEDXY(1,0,0,0,10);mapLEDXY(1,1,0,0,10);mapLEDXY(6,0,0,0,10);mapLEDXY(6,1,0,0,10);mapLEDXY(7,1,0,0,10);mapLEDXY(0,30,0,0,10);mapLEDXY(1,30,0,0,10);mapLEDXY(1,31,0,0,10);mapLEDXY(6,30,0,0,10);mapLEDXY(6,31,0,0,10);mapLEDXY(7,30,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  mapLEDXY(0,2,0,0,10);mapLEDXY(1,2,0,0,10);mapLEDXY(2,0,0,0,10);mapLEDXY(2,1,0,0,10);mapLEDXY(2,2,0,0,10);mapLEDXY(5,0,0,0,10);mapLEDXY(5,1,0,0,10);mapLEDXY(5,2,0,0,10);mapLEDXY(6,2,0,0,10);mapLEDXY(7,2,0,0,10);mapLEDXY(0,29,0,0,10);mapLEDXY(1,29,0,0,10);mapLEDXY(2,29,0,0,10);mapLEDXY(2,30,0,0,10);mapLEDXY(2,31,0,0,10);mapLEDXY(5,29,0,0,10);mapLEDXY(5,30,0,0,10);mapLEDXY(5,31,0,0,10);mapLEDXY(6,29,0,0,10);mapLEDXY(7,29,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  mapLEDXY(0,3,0,0,10);mapLEDXY(1,3,0,0,10);mapLEDXY(2,3,0,0,10);mapLEDXY(3,0,0,0,10);mapLEDXY(3,1,0,0,10);mapLEDXY(3,2,0,0,10);mapLEDXY(3,3,0,0,10);mapLEDXY(4,0,0,0,10);mapLEDXY(4,1,0,0,10);mapLEDXY(4,2,0,0,10);mapLEDXY(4,3,0,0,10);mapLEDXY(5,3,0,0,10);mapLEDXY(6,3,0,0,10);mapLEDXY(7,3,0,0,10);mapLEDXY(0,28,0,0,10);mapLEDXY(1,28,0,0,10);mapLEDXY(2,28,0,0,10);mapLEDXY(3,28,0,0,10);mapLEDXY(3,29,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(3,31,0,0,10);mapLEDXY(4,28,0,0,10);mapLEDXY(4,29,0,0,10);mapLEDXY(4,30,0,0,10);mapLEDXY(4,31,0,0,10);mapLEDXY(5,28,0,0,10);mapLEDXY(6,28,0,0,10);mapLEDXY(7,28,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  mapLEDXY(0,4,0,0,10);mapLEDXY(1,4,0,0,10);mapLEDXY(2,4,0,0,10);mapLEDXY(3,4,0,0,10);mapLEDXY(4,4,0,0,10);mapLEDXY(5,4,0,0,10);mapLEDXY(6,4,0,0,10);mapLEDXY(7,4,0,0,10);mapLEDXY(0,27,0,0,10);mapLEDXY(1,27,0,0,10);mapLEDXY(2,27,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(4,27,0,0,10);mapLEDXY(5,27,0,0,10);mapLEDXY(6,27,0,0,10);mapLEDXY(7,27,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  mapLEDXY(0,5,0,0,10);mapLEDXY(1,5,0,0,10);mapLEDXY(2,5,0,0,10);mapLEDXY(3,5,0,0,10);mapLEDXY(4,5,0,0,10);mapLEDXY(5,5,0,0,10);mapLEDXY(6,5,0,0,10);mapLEDXY(7,5,0,0,10);mapLEDXY(0,26,0,0,10);mapLEDXY(1,26,0,0,10);mapLEDXY(2,26,0,0,10);mapLEDXY(3,26,0,0,10);mapLEDXY(4,26,0,0,10);mapLEDXY(5,26,0,0,10);mapLEDXY(6,26,0,0,10);mapLEDXY(7,26,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  mapLEDXY(0,6,0,0,10);mapLEDXY(1,6,0,0,10);mapLEDXY(2,6,0,0,10);mapLEDXY(3,6,0,0,10);mapLEDXY(4,6,0,0,10);mapLEDXY(5,6,0,0,10);mapLEDXY(6,6,0,0,10);mapLEDXY(7,6,0,0,10);mapLEDXY(0,25,0,0,10);mapLEDXY(1,25,0,0,10);mapLEDXY(2,25,0,0,10);mapLEDXY(3,25,0,0,10);mapLEDXY(4,25,0,0,10);mapLEDXY(5,25,0,0,10);mapLEDXY(6,25,0,0,10);mapLEDXY(7,25,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  mapLEDXY(0,7,0,0,10);mapLEDXY(1,7,0,0,10);mapLEDXY(2,7,0,0,10);mapLEDXY(3,7,0,0,10);mapLEDXY(4,7,0,0,10);mapLEDXY(5,7,0,0,10);mapLEDXY(6,7,0,0,10);mapLEDXY(7,7,0,0,10);mapLEDXY(0,24,0,0,10);mapLEDXY(1,24,0,0,10);mapLEDXY(2,24,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(4,24,0,0,10);mapLEDXY(5,24,0,0,10);mapLEDXY(6,24,0,0,10);mapLEDXY(7,24,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  mapLEDXY(0,8,0,0,10);mapLEDXY(0,9,0,0,10);mapLEDXY(0,10,0,0,10);mapLEDXY(0,11,0,0,10);mapLEDXY(0,12,0,0,10);mapLEDXY(0,13,0,0,10);mapLEDXY(0,14,0,0,10);mapLEDXY(0,15,0,0,10);mapLEDXY(1,8,0,0,10);mapLEDXY(1,15,0,0,10);mapLEDXY(2,8,0,0,10);mapLEDXY(2,15,0,0,10);mapLEDXY(3,8,0,0,10);mapLEDXY(3,15,0,0,10);mapLEDXY(4,8,0,0,10);mapLEDXY(4,15,0,0,10);mapLEDXY(5,8,0,0,10);mapLEDXY(5,15,0,0,10);mapLEDXY(6,8,0,0,10);mapLEDXY(6,15,0,0,10);mapLEDXY(7,8,0,0,10);mapLEDXY(7,9,0,0,10);mapLEDXY(7,10,0,0,10);mapLEDXY(7,11,0,0,10);mapLEDXY(7,12,0,0,10);mapLEDXY(7,13,0,0,10);mapLEDXY(7,14,0,0,10);mapLEDXY(7,15,0,0,10);mapLEDXY(0,16,0,0,10);mapLEDXY(0,17,0,0,10);mapLEDXY(0,18,0,0,10);mapLEDXY(0,19,0,0,10);mapLEDXY(0,20,0,0,10);mapLEDXY(0,21,0,0,10);mapLEDXY(0,22,0,0,10);mapLEDXY(0,23,0,0,10);mapLEDXY(1,16,0,0,10);mapLEDXY(1,23,0,0,10);mapLEDXY(2,16,0,0,10);mapLEDXY(2,23,0,0,10);mapLEDXY(3,16,0,0,10);mapLEDXY(3,23,0,0,10);mapLEDXY(4,16,0,0,10);mapLEDXY(4,23,0,0,10);mapLEDXY(5,16,0,0,10);mapLEDXY(5,23,0,0,10);mapLEDXY(6,16,0,0,10);mapLEDXY(6,23,0,0,10);mapLEDXY(7,16,0,0,10);mapLEDXY(7,17,0,0,10);mapLEDXY(7,18,0,0,10);mapLEDXY(7,19,0,0,10);mapLEDXY(7,20,0,0,10);mapLEDXY(7,21,0,0,10);mapLEDXY(7,22,0,0,10);mapLEDXY(7,23,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  mapLEDXY(1,9,0,0,10);mapLEDXY(1,10,0,0,10);mapLEDXY(1,11,0,0,10);mapLEDXY(1,12,0,0,10);mapLEDXY(1,13,0,0,10);mapLEDXY(1,14,0,0,10);mapLEDXY(2,9,0,0,10);mapLEDXY(2,14,0,0,10);mapLEDXY(3,9,0,0,10);mapLEDXY(3,14,0,0,10);mapLEDXY(4,9,0,0,10);mapLEDXY(4,14,0,0,10);mapLEDXY(5,9,0,0,10);mapLEDXY(5,14,0,0,10);mapLEDXY(6,9,0,0,10);mapLEDXY(6,10,0,0,10);mapLEDXY(6,11,0,0,10);mapLEDXY(6,12,0,0,10);mapLEDXY(6,13,0,0,10);mapLEDXY(6,14,0,0,10);mapLEDXY(1,17,0,0,10);mapLEDXY(1,18,0,0,10);mapLEDXY(1,19,0,0,10);mapLEDXY(1,20,0,0,10);mapLEDXY(1,21,0,0,10);mapLEDXY(1,22,0,0,10);mapLEDXY(2,17,0,0,10);mapLEDXY(2,22,0,0,10);mapLEDXY(3,17,0,0,10);mapLEDXY(3,22,0,0,10);mapLEDXY(4,17,0,0,10);mapLEDXY(4,22,0,0,10);mapLEDXY(5,17,0,0,10);mapLEDXY(5,22,0,0,10);mapLEDXY(6,17,0,0,10);mapLEDXY(6,18,0,0,10);mapLEDXY(6,19,0,0,10);mapLEDXY(6,20,0,0,10);mapLEDXY(6,21,0,0,10);mapLEDXY(6,22,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  mapLEDXY(2,10,0,0,10);mapLEDXY(2,11,0,0,10);mapLEDXY(2,12,0,0,10);mapLEDXY(2,13,0,0,10);mapLEDXY(3,10,0,0,10);mapLEDXY(3,13,0,0,10);mapLEDXY(4,10,0,0,10);mapLEDXY(4,13,0,0,10);mapLEDXY(5,10,0,0,10);mapLEDXY(5,11,0,0,10);mapLEDXY(5,12,0,0,10);mapLEDXY(5,13,0,0,10);mapLEDXY(2,18,0,0,10);mapLEDXY(2,19,0,0,10);mapLEDXY(2,20,0,0,10);mapLEDXY(2,21,0,0,10);mapLEDXY(3,18,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(4,18,0,0,10);mapLEDXY(4,21,0,0,10);mapLEDXY(5,18,0,0,10);mapLEDXY(5,19,0,0,10);mapLEDXY(5,20,0,0,10);mapLEDXY(5,21,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  mapLEDXY(3,11,0,0,10);mapLEDXY(3,12,0,0,10);mapLEDXY(4,11,0,0,10);mapLEDXY(4,12,0,0,10);mapLEDXY(3,19,0,0,10);mapLEDXY(3,20,0,0,10);mapLEDXY(4,19,0,0,10);mapLEDXY(4,20,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
}

void pacman(){
  mapLEDXY(2,0,0,0,10);mapLEDXY(3,0,0,0,10);mapLEDXY(4,0,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(1,0,0,0,10);mapLEDXY(2,1,0,0,10);mapLEDXY(3,0,0,0,10);mapLEDXY(4,1,0,0,10);mapLEDXY(5,0,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,0,0,0,10);mapLEDXY(1,1,0,0,10);mapLEDXY(2,0,0,0,10);mapLEDXY(4,0,0,0,10);mapLEDXY(5,1,0,0,10);mapLEDXY(6,0,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,0,0,0,10);mapLEDXY(0,1,0,0,10);mapLEDXY(1,2,0,0,10);mapLEDXY(2,3,0,0,10);mapLEDXY(3,2,0,0,10);mapLEDXY(4,3,0,0,10);mapLEDXY(5,2,0,0,10);mapLEDXY(6,0,0,0,10);mapLEDXY(6,1,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,0,0,0,10);mapLEDXY(0,1,0,0,10);mapLEDXY(0,2,0,0,10);mapLEDXY(1,3,0,0,10);mapLEDXY(2,4,0,0,10);mapLEDXY(3,4,0,0,10);mapLEDXY(4,4,0,0,10);mapLEDXY(5,3,0,0,10);mapLEDXY(6,0,0,0,10);mapLEDXY(6,1,0,0,10);mapLEDXY(6,2,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,1,0,0,10);mapLEDXY(0,2,0,0,10);mapLEDXY(0,3,0,0,10);mapLEDXY(1,0,0,0,10);mapLEDXY(1,4,0,0,10);mapLEDXY(2,5,0,0,10);mapLEDXY(3,4,0,0,10);mapLEDXY(4,5,0,0,10);mapLEDXY(5,0,0,0,10);mapLEDXY(5,4,0,0,10);mapLEDXY(6,1,0,0,10);mapLEDXY(6,2,0,0,10);mapLEDXY(6,3,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,2,0,0,10);mapLEDXY(0,3,0,0,10);mapLEDXY(0,4,0,0,10);mapLEDXY(1,1,0,0,10);mapLEDXY(1,5,0,0,10);mapLEDXY(2,0,0,0,10);mapLEDXY(2,4,0,0,10);mapLEDXY(3,0,0,0,10);mapLEDXY(3,3,0,0,10);mapLEDXY(4,0,0,0,10);mapLEDXY(4,4,0,0,10);mapLEDXY(5,1,0,0,10);mapLEDXY(5,5,0,0,10);mapLEDXY(6,2,0,0,10);mapLEDXY(6,3,0,0,10);mapLEDXY(6,4,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,3,0,0,10);mapLEDXY(0,4,0,0,10);mapLEDXY(0,5,0,0,10);mapLEDXY(1,2,0,0,10);mapLEDXY(1,6,0,0,10);mapLEDXY(2,1,0,0,10);mapLEDXY(2,7,0,0,10);mapLEDXY(3,1,0,0,10);mapLEDXY(3,6,0,0,10);mapLEDXY(4,1,0,0,10);mapLEDXY(4,7,0,0,10);mapLEDXY(5,2,0,0,10);mapLEDXY(5,6,0,0,10);mapLEDXY(6,3,0,0,10);mapLEDXY(6,4,0,0,10);mapLEDXY(6,5,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,4,0,0,10);mapLEDXY(0,5,0,0,10);mapLEDXY(0,6,0,0,10);mapLEDXY(1,3,0,0,10);mapLEDXY(1,7,0,0,10);mapLEDXY(2,2,0,0,10);mapLEDXY(2,8,0,0,10);mapLEDXY(3,2,0,0,10);mapLEDXY(3,8,0,0,10);mapLEDXY(4,2,0,0,10);mapLEDXY(4,8,0,0,10);mapLEDXY(5,3,0,0,10);mapLEDXY(5,7,0,0,10);mapLEDXY(6,4,0,0,10);mapLEDXY(6,5,0,0,10);mapLEDXY(6,6,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,5,0,0,10);mapLEDXY(0,6,0,0,10);mapLEDXY(0,7,0,0,10);mapLEDXY(1,4,0,0,10);mapLEDXY(1,8,0,0,10);mapLEDXY(2,3,0,0,10);mapLEDXY(2,9,0,0,10);mapLEDXY(3,3,0,0,10);mapLEDXY(3,8,0,0,10);mapLEDXY(4,3,0,0,10);mapLEDXY(4,9,0,0,10);mapLEDXY(5,4,0,0,10);mapLEDXY(5,8,0,0,10);mapLEDXY(6,5,0,0,10);mapLEDXY(6,6,0,0,10);mapLEDXY(6,7,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,6,0,0,10);mapLEDXY(0,7,0,0,10);mapLEDXY(0,8,0,0,10);mapLEDXY(1,5,0,0,10);mapLEDXY(1,9,0,0,10);mapLEDXY(2,4,0,0,10);mapLEDXY(2,8,0,0,10);mapLEDXY(3,4,0,0,10);mapLEDXY(3,7,0,0,10);mapLEDXY(3,12,0,0,10);mapLEDXY(3,15,0,0,10);mapLEDXY(4,4,0,0,10);mapLEDXY(4,8,0,0,10);mapLEDXY(5,5,0,0,10);mapLEDXY(5,9,0,0,10);mapLEDXY(6,6,0,0,10);mapLEDXY(6,7,0,0,10);mapLEDXY(6,8,0,0,10);mapLEDXY(3,18,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,7,0,0,10);mapLEDXY(0,8,0,0,10);mapLEDXY(0,9,0,0,10);mapLEDXY(1,6,0,0,10);mapLEDXY(1,10,0,0,10);mapLEDXY(2,5,0,0,10);mapLEDXY(2,9,0,0,10);mapLEDXY(3,5,0,0,10);mapLEDXY(3,8,0,0,10);mapLEDXY(3,12,0,0,10);mapLEDXY(3,15,0,0,10);mapLEDXY(4,5,0,0,10);mapLEDXY(4,9,0,0,10);mapLEDXY(5,6,0,0,10);mapLEDXY(5,10,0,0,10);mapLEDXY(6,7,0,0,10);mapLEDXY(6,8,0,0,10);mapLEDXY(6,9,0,0,10);mapLEDXY(3,18,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,8,0,0,10);mapLEDXY(0,9,0,0,10);mapLEDXY(0,10,0,0,10);mapLEDXY(1,7,0,0,10);mapLEDXY(1,11,0,0,10);mapLEDXY(2,6,0,0,10);mapLEDXY(2,12,0,0,10);mapLEDXY(3,6,0,0,10);mapLEDXY(3,11,0,0,10);mapLEDXY(3,12,0,0,10);mapLEDXY(3,15,0,0,10);mapLEDXY(4,6,0,0,10);mapLEDXY(4,12,0,0,10);mapLEDXY(5,7,0,0,10);mapLEDXY(5,11,0,0,10);mapLEDXY(6,8,0,0,10);mapLEDXY(6,9,0,0,10);mapLEDXY(6,10,0,0,10);mapLEDXY(3,18,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,9,0,0,10);mapLEDXY(0,10,0,0,10);mapLEDXY(0,11,0,0,10);mapLEDXY(1,8,0,0,10);mapLEDXY(1,12,0,0,10);mapLEDXY(2,7,0,0,10);mapLEDXY(2,13,0,0,10);mapLEDXY(3,7,0,0,10);mapLEDXY(3,13,0,0,10);mapLEDXY(3,15,0,0,10);mapLEDXY(4,7,0,0,10);mapLEDXY(4,13,0,0,10);mapLEDXY(5,8,0,0,10);mapLEDXY(5,12,0,0,10);mapLEDXY(6,9,0,0,10);mapLEDXY(6,10,0,0,10);mapLEDXY(6,11,0,0,10);mapLEDXY(3,18,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,10,0,0,10);mapLEDXY(0,11,0,0,10);mapLEDXY(0,12,0,0,10);mapLEDXY(1,9,0,0,10);mapLEDXY(1,13,0,0,10);mapLEDXY(2,8,0,0,10);mapLEDXY(2,12,0,0,10);mapLEDXY(3,8,0,0,10);mapLEDXY(3,11,0,0,10);mapLEDXY(3,15,0,0,10);mapLEDXY(4,8,0,0,10);mapLEDXY(4,12,0,0,10);mapLEDXY(5,9,0,0,10);mapLEDXY(5,13,0,0,10);mapLEDXY(6,10,0,0,10);mapLEDXY(6,11,0,0,10);mapLEDXY(6,12,0,0,10);mapLEDXY(3,18,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,12,0,0,10);mapLEDXY(0,13,0,0,10);mapLEDXY(0,14,0,0,10);mapLEDXY(1,11,0,0,10);mapLEDXY(1,15,0,0,10);mapLEDXY(2,10,0,0,10);mapLEDXY(3,10,0,0,10);mapLEDXY(4,10,0,0,10);mapLEDXY(5,11,0,0,10);mapLEDXY(5,15,0,0,10);mapLEDXY(6,12,0,0,10);mapLEDXY(6,13,0,0,10);mapLEDXY(6,14,0,0,10);mapLEDXY(2,16,0,0,10);mapLEDXY(3,16,0,0,10);mapLEDXY(3,18,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,16,0,0,10);
  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,13,0,0,10);mapLEDXY(0,14,0,0,10);mapLEDXY(0,15,0,0,10);mapLEDXY(1,12,0,0,10);mapLEDXY(2,11,0,0,10);mapLEDXY(3,11,0,0,10);mapLEDXY(4,11,0,0,10);mapLEDXY(5,12,0,0,10);mapLEDXY(6,13,0,0,10);mapLEDXY(6,14,0,0,10);mapLEDXY(6,15,0,0,10);mapLEDXY(1,16,0,0,10);mapLEDXY(2,17,0,0,10);mapLEDXY(3,16,0,0,10);mapLEDXY(3,18,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,17,0,0,10);mapLEDXY(5,16,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,14,0,0,10);mapLEDXY(0,15,0,0,10);mapLEDXY(1,13,0,0,10);mapLEDXY(2,12,0,0,10);mapLEDXY(3,12,0,0,10);mapLEDXY(4,12,0,0,10);mapLEDXY(5,13,0,0,10);mapLEDXY(6,14,0,0,10);mapLEDXY(6,15,0,0,10);mapLEDXY(0,16,0,0,10);mapLEDXY(1,17,0,0,10);mapLEDXY(2,16,0,0,10);mapLEDXY(3,18,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,16,0,0,10);mapLEDXY(5,17,0,0,10);mapLEDXY(6,16,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,15,0,0,10);mapLEDXY(1,14,0,0,10);mapLEDXY(2,13,0,0,10);mapLEDXY(3,13,0,0,10);mapLEDXY(4,13,0,0,10);mapLEDXY(5,14,0,0,10);mapLEDXY(6,15,0,0,10);mapLEDXY(0,16,0,0,10);mapLEDXY(0,17,0,0,10);mapLEDXY(1,18,0,0,10);mapLEDXY(2,19,0,0,10);mapLEDXY(3,18,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,19,0,0,10);mapLEDXY(5,18,0,0,10);mapLEDXY(6,16,0,0,10);mapLEDXY(6,17,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(1,15,0,0,10);mapLEDXY(2,14,0,0,10);mapLEDXY(3,14,0,0,10);mapLEDXY(4,14,0,0,10);mapLEDXY(5,15,0,0,10);mapLEDXY(0,16,0,0,10);mapLEDXY(0,17,0,0,10);mapLEDXY(0,18,0,0,10);mapLEDXY(1,19,0,0,10);mapLEDXY(2,18,0,0,10);mapLEDXY(3,17,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,18,0,0,10);mapLEDXY(5,19,0,0,10);mapLEDXY(6,16,0,0,10);mapLEDXY(6,17,0,0,10);mapLEDXY(6,18,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(2,15,0,0,10);mapLEDXY(3,15,0,0,10);mapLEDXY(4,15,0,0,10);mapLEDXY(0,17,0,0,10);mapLEDXY(0,18,0,0,10);mapLEDXY(0,19,0,0,10);mapLEDXY(1,16,0,0,10);mapLEDXY(1,20,0,0,10);mapLEDXY(2,21,0,0,10);mapLEDXY(3,20,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,21,0,0,10);mapLEDXY(5,16,0,0,10);mapLEDXY(5,20,0,0,10);mapLEDXY(6,17,0,0,10);mapLEDXY(6,18,0,0,10);mapLEDXY(6,19,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,18,0,0,10);mapLEDXY(0,19,0,0,10);mapLEDXY(0,20,0,0,10);mapLEDXY(1,17,0,0,10);mapLEDXY(1,21,0,0,10);mapLEDXY(2,16,0,0,10);mapLEDXY(2,22,0,0,10);mapLEDXY(3,16,0,0,10);mapLEDXY(3,22,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,16,0,0,10);mapLEDXY(4,22,0,0,10);mapLEDXY(5,17,0,0,10);mapLEDXY(5,21,0,0,10);mapLEDXY(6,18,0,0,10);mapLEDXY(6,19,0,0,10);mapLEDXY(6,20,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,19,0,0,10);mapLEDXY(0,20,0,0,10);mapLEDXY(0,21,0,0,10);mapLEDXY(1,18,0,0,10);mapLEDXY(1,22,0,0,10);mapLEDXY(2,17,0,0,10);mapLEDXY(2,23,0,0,10);mapLEDXY(3,17,0,0,10);mapLEDXY(3,22,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,17,0,0,10);mapLEDXY(4,23,0,0,10);mapLEDXY(5,18,0,0,10);mapLEDXY(5,22,0,0,10);mapLEDXY(6,19,0,0,10);mapLEDXY(6,20,0,0,10);mapLEDXY(6,21,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,20,0,0,10);mapLEDXY(0,21,0,0,10);mapLEDXY(0,22,0,0,10);mapLEDXY(1,19,0,0,10);mapLEDXY(1,23,0,0,10);mapLEDXY(2,18,0,0,10);mapLEDXY(2,22,0,0,10);mapLEDXY(3,18,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,18,0,0,10);mapLEDXY(4,22,0,0,10);mapLEDXY(5,19,0,0,10);mapLEDXY(5,23,0,0,10);mapLEDXY(6,20,0,0,10);mapLEDXY(6,21,0,0,10);mapLEDXY(6,22,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,21,0,0,10);mapLEDXY(0,22,0,0,10);mapLEDXY(0,23,0,0,10);mapLEDXY(1,20,0,0,10);mapLEDXY(1,24,0,0,10);mapLEDXY(2,19,0,0,10);mapLEDXY(2,25,0,0,10);mapLEDXY(3,19,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,19,0,0,10);mapLEDXY(4,25,0,0,10);mapLEDXY(5,20,0,0,10);mapLEDXY(5,24,0,0,10);mapLEDXY(6,21,0,0,10);mapLEDXY(6,22,0,0,10);mapLEDXY(6,23,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,22,0,0,10);mapLEDXY(0,23,0,0,10);mapLEDXY(0,24,0,0,10);mapLEDXY(1,21,0,0,10);mapLEDXY(1,25,0,0,10);mapLEDXY(2,20,0,0,10);mapLEDXY(2,24,0,0,10);mapLEDXY(3,20,0,0,10);mapLEDXY(3,23,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,20,0,0,10);mapLEDXY(4,24,0,0,10);mapLEDXY(5,21,0,0,10);mapLEDXY(5,25,0,0,10);mapLEDXY(6,22,0,0,10);mapLEDXY(6,23,0,0,10);mapLEDXY(6,24,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,23,0,0,10);mapLEDXY(0,24,0,0,10);mapLEDXY(0,25,0,0,10);mapLEDXY(1,22,0,0,10);mapLEDXY(1,26,0,0,10);mapLEDXY(2,21,0,0,10);mapLEDXY(2,25,0,0,10);mapLEDXY(3,21,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,21,0,0,10);mapLEDXY(4,25,0,0,10);mapLEDXY(5,22,0,0,10);mapLEDXY(5,26,0,0,10);mapLEDXY(6,23,0,0,10);mapLEDXY(6,24,0,0,10);mapLEDXY(6,25,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,24,0,0,10);mapLEDXY(0,25,0,0,10);mapLEDXY(0,26,0,0,10);mapLEDXY(1,23,0,0,10);mapLEDXY(1,27,0,0,10);mapLEDXY(2,22,0,0,10);mapLEDXY(2,28,0,0,10);mapLEDXY(3,22,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,22,0,0,10);mapLEDXY(4,28,0,0,10);mapLEDXY(5,23,0,0,10);mapLEDXY(5,27,0,0,10);mapLEDXY(6,24,0,0,10);mapLEDXY(6,25,0,0,10);mapLEDXY(6,26,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,25,0,0,10);mapLEDXY(0,26,0,0,10);mapLEDXY(0,27,0,0,10);mapLEDXY(1,24,0,0,10);mapLEDXY(1,28,0,0,10);mapLEDXY(2,23,0,0,10);mapLEDXY(2,29,0,0,10);mapLEDXY(3,23,0,0,10);mapLEDXY(3,29,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,23,0,0,10);mapLEDXY(4,29,0,0,10);mapLEDXY(5,24,0,0,10);mapLEDXY(5,28,0,0,10);mapLEDXY(6,25,0,0,10);mapLEDXY(6,26,0,0,10);mapLEDXY(6,27,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
mapLEDXY(0,26,0,0,10);mapLEDXY(0,27,0,0,10);mapLEDXY(0,28,0,0,10);mapLEDXY(1,25,0,0,10);mapLEDXY(1,29,0,0,10);mapLEDXY(2,24,0,0,10);mapLEDXY(2,30,0,0,10);mapLEDXY(3,24,0,0,10);mapLEDXY(3,29,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,24,0,0,10);mapLEDXY(4,30,0,0,10);mapLEDXY(5,25,0,0,10);mapLEDXY(5,29,0,0,10);mapLEDXY(6,26,0,0,10);mapLEDXY(6,27,0,0,10);mapLEDXY(6,28,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,27,0,0,10);mapLEDXY(0,28,0,0,10);mapLEDXY(0,29,0,0,10);mapLEDXY(1,26,0,0,10);mapLEDXY(1,30,0,0,10);mapLEDXY(2,25,0,0,10);mapLEDXY(2,29,0,0,10);mapLEDXY(3,25,0,0,10);mapLEDXY(3,28,0,0,10);mapLEDXY(4,25,0,0,10);mapLEDXY(4,29,0,0,10);mapLEDXY(5,26,0,0,10);mapLEDXY(5,30,0,0,10);mapLEDXY(6,27,0,0,10);mapLEDXY(6,28,0,0,10);mapLEDXY(6,29,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,28,0,0,10);mapLEDXY(0,29,0,0,10);mapLEDXY(0,30,0,0,10);mapLEDXY(1,27,0,0,10);mapLEDXY(1,31,0,0,10);mapLEDXY(2,26,0,0,10);mapLEDXY(3,26,0,0,10);mapLEDXY(3,31,0,0,10);mapLEDXY(4,26,0,0,10);mapLEDXY(5,27,0,0,10);mapLEDXY(5,31,0,0,10);mapLEDXY(6,28,0,0,10);mapLEDXY(6,29,0,0,10);mapLEDXY(6,30,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,29,0,0,10);mapLEDXY(0,30,0,0,10);mapLEDXY(0,31,0,0,10);mapLEDXY(1,28,0,0,10);mapLEDXY(2,27,0,0,10);mapLEDXY(3,27,0,0,10);mapLEDXY(4,27,0,0,10);mapLEDXY(5,28,0,0,10);mapLEDXY(6,29,0,0,10);mapLEDXY(6,30,0,0,10);mapLEDXY(6,31,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,30,0,0,10);mapLEDXY(0,31,0,0,10);mapLEDXY(1,29,0,0,10);mapLEDXY(2,28,0,0,10);mapLEDXY(3,28,0,0,10);mapLEDXY(4,28,0,0,10);mapLEDXY(5,29,0,0,10);mapLEDXY(6,30,0,0,10);mapLEDXY(6,31,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(0,31,0,0,10);mapLEDXY(1,30,0,0,10);mapLEDXY(2,29,0,0,10);mapLEDXY(3,29,0,0,10);mapLEDXY(4,29,0,0,10);mapLEDXY(5,30,0,0,10);mapLEDXY(6,31,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(1,31,0,0,10);mapLEDXY(2,30,0,0,10);mapLEDXY(3,30,0,0,10);mapLEDXY(4,30,0,0,10);mapLEDXY(5,31,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();
  mapLEDXY(2,31,0,0,10);mapLEDXY(3,31,0,0,10);mapLEDXY(4,31,0,0,10);

  strip.show();
  delay(rate*10);
  randomColor();
  clearLED();

}

void mapLEDXY(int x, int y, byte RED, byte  GREEN, byte BLUE) {
  int RGBlocation = 0;

  if (y % 2 == 0) { //even column
    RGBlocation = x + y * 8;

  } else { //odd column
    RGBlocation = 7 - x + y * 8;
  }
  strip.setPixelColor(RGBlocation,colors[colorPick]);
  
}
void randomColor(){
  colorPick = random(10);
}

void solid(uint32_t color, long prev){
  
  for(int i=0;i<strip.numPixels();i++){
    strip.setPixelColor(i,color);
    
  }
  strip.show();
  while(true){
    if((code != prev) || (color != colors[colorPick])) {
      break;
    }
  }
}

void flashBang(uint32_t color, long prev){
  
  for(int i=0;i<strip.numPixels();i++){
    if(code != prev){
    break;
  }
    strip.setPixelColor(i,color);
  }
  strip.show();
  delay(rate*4);
  for(int i=0;i<strip.numPixels();i++){
    strip.setPixelColor(i,strip.Color(0,0,0));
  }
  strip.show();
  delay(rate*4);
}

void colorLoop(long prev){
  
    // Fade IN
    for(int k = 0; k < 256; k++) {
      if(code != prev){
      break;
    }
      switch(colorPick) {
        case 0: setAll(k,k,k); break;
        case 1: setAll(k,0,0); break;
        case 2: setAll(0,k,0); break;
        case 3: setAll(0,0,k); break;
        case 4: setAll(0,k,k); break;
        case 5: setAll(k,0,k); break;
        case 6: setAll(k,k,0); break;
        case 7: setAll(k,k/2,0); break;
        case 8: setAll(k/2,k,0); break;
        case 9: setAll(k/2,0,k); break;
      }
      strip.show();
      delay(rate);
    }
    // Fade OUT
    for(int k = 255; k >= 0; k--) {
      if(code != prev){
      break;
    }
      switch(colorPick) {
        case 0: setAll(k,k,k); break;
        case 1: setAll(k,0,0); break;
        case 2: setAll(0,k,0); break;
        case 3: setAll(0,0,k); break;
        case 4: setAll(0,k,k); break;
        case 5: setAll(k,0,k); break;
        case 6: setAll(k,k,0); break;
        case 7: setAll(k,k/2,0); break;
        case 8: setAll(k/2,k,0); break;
        case 9: setAll(k/2,0,k); break;
      }
      strip.show();
      delay(rate);
    }
  
}

void RGBLoop(long prev){
  for(int j = 0; j < 10; j++ ) {
    // Fade IN
    for(int k = 0; k < 256; k++) {
      if(code != prev){
      break;
    }
      switch(j) {
        case 0: setAll(k,k,k); break;
        case 1: setAll(k,0,0); break;
        case 2: setAll(0,k,0); break;
        case 3: setAll(0,0,k); break;
        case 4: setAll(0,k,k); break;
        case 5: setAll(k,0,k); break;
        case 6: setAll(k,k,0); break;
        case 7: setAll(k,k/2,0); break;
        case 8: setAll(k/2,k,0); break;
        case 9: setAll(k/2,0,k); break;
      }
      strip.show();
      delay(rate);
    }
    // Fade OUT
    for(int k = 255; k >= 0; k--) {
      if(code != prev){
      break;
    }
      switch(j) {
        case 0: setAll(k,k,k); break;
        case 1: setAll(k,0,0); break;
        case 2: setAll(0,k,0); break;
        case 3: setAll(0,0,k); break;
        case 4: setAll(0,k,k); break;
        case 5: setAll(k,0,k); break;
        case 6: setAll(k,k,0); break;
        case 7: setAll(k,k/2,0); break;
        case 8: setAll(k/2,k,0); break;
        case 9: setAll(k/2,0,k); break;
      }
      strip.show();
      delay(rate);
    }
  }
}

void setAll(int r,int g,int b){
  for(int i=0;i<strip.numPixels();i++){
    strip.setPixelColor(i,strip.Color(r,g,b));
  }
}

void rainbow(long prev) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    if(code != prev){
        break;
      }
    for(int i=0; i<strip.numPixels(); i++) {
      if(code != prev){
        break;
      }
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(10+rate);  // Pause for a moment
  }
}

void stars(long prev){
  
  for(int i =0; i<15;i++){
    if(code != prev){
      break;
    }
    xTaskCreate(oneStar,"oneStar",10000,NULL,1,NULL);
    delay(100);
  }
                    
}
void oneStar(void * parameter){
  int star = random(236);
  for(int i=100; i>0; i=i-5){
    strip.setPixelColor(star, strip.Color(i,i,i));
    
    strip.show();
    delay(rate);
  }
  vTaskDelete (NULL);
}

void colorvsColor(long prev) {
  int color1 = colors[random(10)];
  int color2 = colors[random(10)];
  for(int i = 1; i<8; i++){
    if(code != prev){
      break;
    }
    for(int x = 1; x<31; x++)
    strip.setPixelColor(i*x, color1); 
    strip.show();
    delay(rate);
    }
    
  
  for(int i = 8; i>0; i--){
    if(code != prev){
      break;
    }
    for(int x = 31; x>0; x--)
    strip.setPixelColor(i*x, color2); 
    strip.show();
    delay(rate);
    }
    
  }
  


void lineChase(uint32_t color, long prev) {
  for (int i=0; i<236; i++){
    if((code != prev) || (color != colors[colorPick])) {
      break;
    }
    for(int x=0; x<10; x++){
      strip.setPixelColor(i+x, color);
    }
    strip.setPixelColor(i-1, (0,0,0));
    strip.show();
    
    delay(rate);
  }
}

void upStrob(uint32_t color,long prev){
    
      
    
    for(int i=0; i<31; i++) { // For each pixel in strip...
    if((code != prev) || (color != colors[colorPick])) {
      break;
    }
    strip.setPixelColor(112+i, color); //  Set pixel's color (in RAM)
    strip.setPixelColor(173-i, color); //  Set pixel's color (in RAM)
    strip.setPixelColor(174+i, color); //  Set pixel's color (in RAM)
    strip.setPixelColor(235-i, color); //  Set pixel's color (in RAM)
    strip.setPixelColor(112+i-1, (0,0,0)); //  Set pixel's color (in RAM)
    strip.setPixelColor(173-i+1, (0,0,0)); //  Set pixel's color (in RAM)
    strip.setPixelColor(174-1+i, (0,0,0)); //  Set pixel's color (in RAM)
    strip.setPixelColor(235-i+1, (0,0,0)); //  Set pixel's color (in RAM)
    if(i<=27){
      strip.setPixelColor(i+1, color);
      strip.setPixelColor(i, (0,0,0));
      strip.setPixelColor(i+1+28, color);
      strip.setPixelColor(i+28, (0,0,0));
      strip.setPixelColor(i+1+56, color);
      strip.setPixelColor(i+56, (0,0,0));
      strip.setPixelColor(i+1+84, color);
      strip.setPixelColor(i+84, (0,0,0));
    }
    strip.show();//  Update strip to match
    delay(rate);
    }
  
  strip.setPixelColor(143,0);
  strip.setPixelColor(142,0);
  strip.setPixelColor(205,0);
  strip.setPixelColor(204,0);
  strip.show();
  
    }


void clearLED() {
  for(int i=0; i< strip.numPixels(); i++){
        strip.setPixelColor(i,(0,0,0));
        
      }
      strip.show();
}
void colorWipe(uint32_t color ,long prevColor) {
  
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    if((code != prevColor) || (color != colors[colorPick])) {
      break;
    }
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(rate);                           //  Pause for a moment
    
  }
  
}

void meteorRain(uint32_t color, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, long prev) {  
  int NUM_LEDS = strip.numPixels();
  
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
    if((code != prev) || (color != colors[colorPick])) {
      break;
    }
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        strip.setPixelColor(i-j, color);
      }
    }
   
    strip.show();
    delay(rate);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}
