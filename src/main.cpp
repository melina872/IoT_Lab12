// Lab 11 – Analog Input and Output: ADC, DAC and PWM on the ESP32
// STARTER FILE – follow the lab procedure and add your code under each comment.
#include <Arduino.h>

// ---------- Choose which part runs ----------
// Change this number to the part you are working on, then upload.
// 3 = potentiometer, 4 = resolution, 5 = sampling, 6 = DAC, 7 = PWM, 8 = RC filter
#define PART 6

// ---------- Pin numbers ----------
#define POT_PIN    34    // potentiometer wiper (ADC1)
#define SIG_PIN    32    // signal generator (ADC1)
#define LOOP_PIN   33    // reads the DAC (Part VI) and the RC filter (Part VIII) (ADC1)
#define DAC_PIN    25    // DAC1 output
#define LED_PIN    18    // PWM: LED and RC filter input

// ---------- Global variables ----------
// Part IV – resolution
const int ADC_BITS = 12;       // try 12, 11, 10 and 9

// Part V – sampling
const unsigned long SAMPLE_RATE = 200;   // samples per second (Hz)
const unsigned long SAMPLE_PERIOD = 1000000 / SAMPLE_RATE;   // µs between samples
unsigned long lastSample = 0;   // time of the last sample (µs)
unsigned long sampleCount = 0;  // samples taken in this second
unsigned long lastSecond = 0;   // time the count started (ms)

// ---------- Part functions ----------
// Part III – read the potentiometer and send it to the Serial Plotter
void partPot()
{
  // Read the potentiometer (Common Commands: "Read an Analog Pin").
  int potValue = analogRead(POT_PIN);

  // Send the value to the Serial Plotter.
  // A line that starts with > becomes a graph in Teleplot.
  Serial.print(">pot:");  Serial.println(potValue);
  delay(50);

}

// Part IV – resolution and voltage
void partResolution()
{
  analogReadResolution(ADC_BITS);           // use ADC_BITS bits
  int raw = analogRead(POT_PIN);
  int maxValue = pow(2, ADC_BITS) - 1;      // largest value: 2^bits – 1
  float volts = raw * 3.3 / maxValue;       // raw value → volts (Key Formulas)

  analogReadResolution(12);                 // analogReadMilliVolts() is always used at 12 bits
  int mv = analogReadMilliVolts(POT_PIN);   // calibrated reading in mV

  Serial.print("bits = ");          Serial.print(ADC_BITS);
  Serial.print("   raw = ");        Serial.print(raw);
  Serial.print("   calculated = "); Serial.print(volts, 3);  Serial.print(" V");
  Serial.print("   analogReadMilliVolts = ");  Serial.print(mv);  Serial.println(" mV");
  delay(500);

}

// Part V – sample the signal generator at SAMPLE_RATE
void partSampling()
{
  // Is it time for the next sample? (Common Commands: "Time Passed (µs)")
  if (micros() - lastSample >= SAMPLE_PERIOD)
  {
    lastSample = micros();               // save the time of this sample
    int value = analogRead(SIG_PIN);     // take one sample
    Serial.print(">signal:");  Serial.println(value);
    sampleCount++;                       // count this sample
  }

  // Once per second, plot how many samples were taken (the real sample rate).
  if (millis() - lastSecond >= 1000)
  {
    lastSecond = millis();
    Serial.print(">rate:");  Serial.println(sampleCount);
    sampleCount = 0;
  }

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
  analogReadResolution(12);            // 12 bits: values 0 – 4095
  analogSetAttenuation(ADC_11db);      // measure the full 0 – 3.3 V range

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
