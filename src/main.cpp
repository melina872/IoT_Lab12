// Lab 11 – Analog Input and Output: ADC, DAC and PWM on the ESP32
// STARTER FILE – follow the lab procedure and add your code under each comment.
#include <Arduino.h>

// ---------- Choose which part runs ----------
// Change this number to the part you are working on, then upload.
// 3 = potentiometer, 4 = resolution, 5 = sampling, 6 = DAC, 7 = PWM, 8 = RC filter
#define PART 7

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
const unsigned long SAMPLE_RATE = 10;   // samples per second (Hz)
const unsigned long SAMPLE_PERIOD = 1000000 / SAMPLE_RATE;   // µs between samples
unsigned long lastSample = 0;   // time of the last sample (µs)
unsigned long sampleCount = 0;  // samples taken in this second
unsigned long lastSecond = 0;   // time the count started (ms)

// Part VI – DAC
const int DAC_VALUE = 128;     // 0 – 255
const bool DAC_SINE = true;   // false = Step 1, true = Step 2 (sine wave)
const int SINE_STEPS = 32;     // points in one sine cycle
const int STEP_DELAY = 30;     // ms between points

// Part VII – PWM

const int PWM_FREQ = 5000;     // PWM frequency (Hz)
const int PWM_RES = 8;         // 8 bits: duty 0 – 255


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
  if (DAC_SINE == false)
{
  // Step 1 – a fixed voltage (Common Commands: "DAC Output")
    dacWrite(DAC_PIN, DAC_VALUE);
    float expected = DAC_VALUE * 3.3 / 255;        // 8 bits (Key Formulas)
    Serial.print("DAC value = ");   Serial.print(DAC_VALUE);
    Serial.print("   expected = "); Serial.print(expected, 3);  Serial.print(" V");
    Serial.print("   ADC = ");  Serial.println(analogReadMilliVolts(LOOP_PIN));
    delay(500);
  }
  else
  {
    // Step 2 – a sine wave, one point at a time
    for (int i = 0; i < SINE_STEPS; i++)
    {
      int value = 128 + 127 * sin(2 * PI * i / SINE_STEPS);   // 1 – 255
      dacWrite(DAC_PIN, value);
      delay(STEP_DELAY);                                       // wait STEP_DELAY ms
      Serial.print(">dac:");  Serial.println(value * 3300 / 255);
      Serial.print(">adc:");  Serial.println(analogReadMilliVolts(LOOP_PIN));
    }
  }

}

// Part VII – PWM: LED brightness follows the potentiometer
void partPWM()
{
  int potValue = analogRead(POT_PIN);

  // Brightness follows the pot (Common Commands: "Scale a Value")
  int duty = map(potValue, 0, 4095, 0, 255);     // 0 – 4095 → 0 – 255
  ledcWrite(LED_PIN, duty);                        // set the LED duty

  Serial.print("duty = ");  Serial.print(duty);
  Serial.print("   duty cycle = ");  Serial.print(duty * 100 / 255);  Serial.println(" %");
  delay(100);

}

// Part VIII – PWM through an RC filter, read back with the ADC
void partRC()
{
  int potValue = analogRead(POT_PIN);
  int duty = map(potValue, 0, 4095, 0, 255);
  ledcWrite(LED_PIN, duty);

  // 200 fast readings of point A: keep the lowest, the highest and the total
  int lowest = 3300;
  int highest = 0;
  long total = 0;
  for (int i = 0; i < 200; i++)
  {
    int v = analogReadMilliVolts(LOOP_PIN);      // one reading in mV
    if (v < lowest)  lowest = v;
    if (v > highest) highest = v;
    total = total + v;
  }

  int expected = duty * 3300 / total;    // average PWM voltage in mV
  int filtered = total / 200;           // average of the 200 readings in mV
  int ripple = highest - lowest;        // size of the up-and-down in mV

  Serial.print(">expected:");  Serial.println(expected);
  Serial.print(">filtered:");  Serial.println(filtered);
  Serial.print(">ripple:");    Serial.println(ripple);
  delay(10);

}

void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println("Lab 11: ADC and DAC");

  // ADC settings
  analogReadResolution(12);            // 12 bits: values 0 – 4095
  analogSetAttenuation(ADC_11db);      // measure the full 0 – 3.3 V range
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RES);  // core 3: pin, frequency, resolution

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
