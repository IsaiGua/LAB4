#include <Arduino.h>

// Pines y configuraciones del servomotor
const int servoPin = 18;
const int servoMin = 500;  // Ciclo de trabajo mínimo (en microsegundos)
const int servoMax = 2400; // Ciclo de trabajo máximo (en microsegundos)

// Pines y configuraciones del LED RGB
const int redPin = 21;
const int greenPin = 22;
const int bluePin = 23;

const int pwmChannelServo = 0;
const int pwmChannelRed = 1;
const int pwmChannelGreen = 2;
const int pwmChannelBlue = 3;
const int pwmFrequencyServo = 50; // Frecuencia de 50 Hz para el servomotor
const int pwmFrequencyLED = 1000; // Frecuencia para el LED RGB
const int pwmResolutionServo = 16;
const int pwmResolutionLED = 8;

// Botones
const int buttonB1 = 15; // Pin del botón B1
const int buttonB2 = 2;  // Pin del botón B2
const int buttonB3 = 4;  // Pin del botón B3
const int buttonB4 = 5;  // Pin del botón B4

int servoPosition = servoMin;
const int angleStep = 100; // Paso para aumentar o disminuir el ciclo de trabajo
int selectedColor = 0;     // Color seleccionado (0=Rojo, 1=Verde, 2=Azul)
int brightness = 0;        // Brillo (0-255)

// Variables para el debounce de cada botón
unsigned long lastDebounceTimeB1 = 0;
unsigned long lastDebounceTimeB2 = 0;
unsigned long lastDebounceTimeB3 = 0;
unsigned long lastDebounceTimeB4 = 0;
unsigned long debounceDelay = 50; // Tiempo de debounce de 50ms

void setup() {
  Serial.begin(115200);

  // Configuración PWM para el servomotor
  ledcSetup(pwmChannelServo, pwmFrequencyServo, pwmResolutionServo);
  ledcAttachPin(servoPin, pwmChannelServo);

  // Configuración PWM para el LED RGB
  ledcSetup(pwmChannelRed, pwmFrequencyLED, pwmResolutionLED);
  ledcSetup(pwmChannelGreen, pwmFrequencyLED, pwmResolutionLED);
  ledcSetup(pwmChannelBlue, pwmFrequencyLED, pwmResolutionLED);

  ledcAttachPin(redPin, pwmChannelRed);
  ledcAttachPin(greenPin, pwmChannelGreen);
  ledcAttachPin(bluePin, pwmChannelBlue);

  // Configuración de los pines de los botones
  pinMode(buttonB1, INPUT_PULLUP);
  pinMode(buttonB2, INPUT_PULLUP);
  pinMode(buttonB3, INPUT_PULLUP);
  pinMode(buttonB4, INPUT_PULLUP);

  // Inicializar posición del servomotor
  ledcWrite(pwmChannelServo, servoPosition);
}

void loop() {
  unsigned long currentTime = millis();

  // Lectura de botones para ajustar el ciclo de trabajo del servomotor
  if ((digitalRead(buttonB1) == LOW) && (currentTime - lastDebounceTimeB1 > debounceDelay)) {
    Serial.println("Botón B1 presionado");
    servoPosition += angleStep;
    if (servoPosition > servoMax) {
      servoPosition = servoMax;
    }
    Serial.print("Servo Position: ");
    Serial.println(servoPosition);
    ledcWrite(pwmChannelServo, servoPosition); // Ajusta el ciclo de trabajo
    lastDebounceTimeB1 = currentTime;
  }

  if ((digitalRead(buttonB2) == LOW) && (currentTime - lastDebounceTimeB2 > debounceDelay)) {
    Serial.println("Botón B2 presionado");
    servoPosition -= angleStep;
    if (servoPosition < servoMin) {
      servoPosition = servoMin;
    }
    Serial.print("Servo Position: ");
    Serial.println(servoPosition);
    ledcWrite(pwmChannelServo, servoPosition); // Ajusta el ciclo de trabajo
    lastDebounceTimeB2 = currentTime;
  }

  // Lectura de botones para el LED RGB
  if ((digitalRead(buttonB3) == LOW) && (currentTime - lastDebounceTimeB3 > debounceDelay)) {
    Serial.println("Botón B3 presionado");
    selectedColor = (selectedColor + 1) % 3;
    lastDebounceTimeB3 = currentTime;
  }

  if ((digitalRead(buttonB4) == LOW) && (currentTime - lastDebounceTimeB4 > debounceDelay)) {
    Serial.println("Botón B4 presionado");
    brightness += 15;
    if (brightness > 255) {
      brightness = 0;
    }
    lastDebounceTimeB4 = currentTime;
  }

  // Control del LED RGB basado en la selección de color
  if (selectedColor == 0) { // Rojo
    ledcWrite(pwmChannelRed, brightness);
    ledcWrite(pwmChannelGreen, 0);
    ledcWrite(pwmChannelBlue, 0);

  } else if (selectedColor == 1) { // Verde
    ledcWrite(pwmChannelRed, 0);
    ledcWrite(pwmChannelGreen, brightness);
    ledcWrite(pwmChannelBlue, 0);

  } else if (selectedColor == 2) { // Azul
    ledcWrite(pwmChannelRed, 0);
    ledcWrite(pwmChannelGreen, 0);
    ledcWrite(pwmChannelBlue, brightness);
  }

  delay(50);
}
