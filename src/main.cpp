// Lab 11 – Analog Input and Output: ADC, DAC and PWM on the ESP32
// STARTER FILE – follow the lab procedure and add your code under each comment.
#include <Arduino.h>

// ---------- Choose which part runs ----------
// Change this number to the part you are working on, then upload.
// 3 = potentiometer, 4 = resolution, 5 = sampling, 6 = DAC, 7 = PWM, 8 = RC filter
#define PART 3

// ---------- Pin numbers ----------
#define POT_PIN    34    // potentiometer wiper (ADC1)
#define SIG_PIN    32    // signal generator (ADC1)
#define LOOP_PIN   33    // reads the DAC (Part VI) and the RC filter (Part VIII) (ADC1)
#define DAC_PIN    25    // DAC1 output
#define LED_PIN    18    // PWM: LED and RC filter input

// ---------- Global variables ----------


// ---------- Part functions ----------
// Part III – read the potentiometer and send it to the Serial Plotter
void partPot()
{
}

// Part IV – resolution and voltage
void partResolution()
{
}

// Part V – sample the signal generator at SAMPLE_RATE
void partSampling()
{
}

// Part VI – DAC: a fixed voltage, then a slow sine wave read back by the ADC
void partDAC()
{
}

// Part VII – PWM: LED brightness follows the potentiometer
void partPWM()
{
}

// Part VIII – PWM through an RC filter, read back with the ADC
void partRC()
{
}

void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println("Lab 11: ADC and DAC");

  // ADC settings

  // PWM settings
}

void loop()
{
  // Runs only the part chosen with #define PART at the top (do not change).
  if (PART == 3)      partPot();
  else if (PART == 4) partResolution();
  else if (PART == 5) partSampling();
  else if (PART == 6) partDAC();
  else if (PART == 7) partPWM();
  else if (PART == 8) partRC();
}
