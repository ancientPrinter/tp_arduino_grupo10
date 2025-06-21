#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LED_ROJO 3
#define BOTON_ROJO 2

#define LED_VERDE 5
#define BOTON_VERDE 4

#define LED_AZUL 6
#define BOTON_AZUL 7

#define LED_AMARILLO 9
#define BOTON_AMARILLO 8

#define TIMER 11
#define BOTON_INICIO 13
#define POT_PIN A1

LiquidCrystal_I2C lcd(0x27, 16, 2);

int botonInicioState = 0;
int numeroAleatorio = 0;

int botonRojoStateAnterior = 0;
int botonVerdeStateAnterior = 0;
int botonAzulStateAnterior = 0;
int botonAmarilloStateAnterior = 0;

int estadoActual = 0;
int rondasAnteriores[99] = {0};
int botonesElegidos[99] = {0};
int contador = 0;
int indice = 0;
int index = 0;
int botonesApretados = 0;
int nivel = 0;

const int DelayDificultad[3] = {1000, 600, 300};

void setup() {
  pinMode(LED_ROJO, OUTPUT);      pinMode(BOTON_ROJO, INPUT);
  pinMode(LED_VERDE, OUTPUT);     pinMode(BOTON_VERDE, INPUT);
  pinMode(LED_AZUL, OUTPUT);      pinMode(BOTON_AZUL, INPUT);
  pinMode(LED_AMARILLO, OUTPUT);  pinMode(BOTON_AMARILLO, INPUT);
  pinMode(TIMER, OUTPUT);
  pinMode(BOTON_INICIO, INPUT);
  pinMode(POT_PIN, INPUT);

  randomSeed(analogRead(0));
  lcd.init();
  lcd.backlight();
}

void loop() {  
  int potVal = analogRead(POT_PIN);
  int nivel;

  if (potVal < 341) {
    nivel = 0;
  }
  else if (potVal >= 341 && potVal < 682) {
    nivel = 1;
  }
  else {
    nivel = 2;
  }
  switch (estadoActual) {
    case 0:
      analogWrite(TIMER, 0);
      while (digitalRead(BOTON_INICIO) == LOW) {

        potVal = analogRead(POT_PIN);

        if (potVal >= 0 && potVal < 341) {
          nivel = 0;  
        }
        else if (potVal >= 341 && potVal < 682) {
          nivel = 1; 
        }
        else {
          nivel = 2;  
        }

        const char* names[3] = {"Facil", "Medio", "Dificil"};

        lcd.setCursor(0, 0);
        lcd.print("Presione Inicio");

        lcd.setCursor(0, 1);
        lcd.print("                "); 

        lcd.setCursor(0, 1);
        lcd.print("Nivel: ");
        lcd.print(names[nivel]);

        delay(2000);
            
        digitalWrite(LED_ROJO, HIGH);
        delay(300);
        digitalWrite(LED_ROJO, LOW);

        digitalWrite(LED_VERDE, HIGH);
        delay(300);
        digitalWrite(LED_VERDE, LOW);

        digitalWrite(LED_AZUL, HIGH);
        delay(300);
        digitalWrite(LED_AZUL, LOW);

        digitalWrite(LED_AMARILLO, HIGH);
        delay(300);
        digitalWrite(LED_AMARILLO, LOW);

        digitalWrite(LED_AZUL, HIGH);
        delay(300);
        digitalWrite(LED_AZUL, LOW);

        digitalWrite(LED_VERDE, HIGH);
        delay(300);
        digitalWrite(LED_VERDE, LOW);

        digitalWrite(LED_ROJO, HIGH);
        delay(300);
        digitalWrite(LED_ROJO, LOW);
      }
      estadoActual = 1;
      break;

    case 1:
      digitalWrite(LED_ROJO, LOW); digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AZUL, LOW); digitalWrite(LED_AMARILLO, LOW);

      numeroAleatorio = random(1, 5);
      rondasAnteriores[indice] = numeroAleatorio;
      indice++;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ronda: ");
      lcd.print(contador + 1);
      lcd.setCursor(0, 1);
      lcd.print("Memorice secuencia");

      // Bucle del array de rondas anteriores
      for (int j = 0; j < contador; j++) {
        switch (rondasAnteriores[j]) {
          case 1:
            digitalWrite(LED_ROJO, HIGH);
          delay(DelayDificultad[nivel]);
            break;
          case 2:
            digitalWrite(LED_VERDE, HIGH);
          delay(DelayDificultad[nivel]);
            break;
          case 3:
            digitalWrite(LED_AZUL, HIGH);
          delay(DelayDificultad[nivel]);
            break;
          case 4:
            digitalWrite(LED_AMARILLO, HIGH);
          delay(DelayDificultad[nivel]);
            break;
        }
        

        // Apaga los LEDs entre secuencias
        digitalWrite(LED_ROJO, LOW);
        digitalWrite(LED_VERDE, LOW);
        digitalWrite(LED_AZUL, LOW);
        digitalWrite(LED_AMARILLO, LOW);
        delay(500);
      }

      // Agrega el nuevo número a la secuencia
      switch (numeroAleatorio) {
        case 1:
          digitalWrite(LED_ROJO, HIGH);
          delay(DelayDificultad[nivel]);
          break;
        case 2:
          digitalWrite(LED_VERDE, HIGH);
          delay(DelayDificultad[nivel]);
          break;
        case 3:
          digitalWrite(LED_AZUL, HIGH);
          delay(DelayDificultad[nivel]);
          break;
        case 4:
          digitalWrite(LED_AMARILLO, HIGH);
          delay(DelayDificultad[nivel]);
          break;
      }
    
      digitalWrite(LED_ROJO, LOW);
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AZUL, LOW);
      digitalWrite(LED_AMARILLO, LOW);
      contador++;
      estadoActual = 2;
      break;

    case 2:
      botonesApretados = 0;
      index = 0;
      unsigned long tiempoInicio = millis();
      int duracionTurno = 10000 - nivel * 2000 + (contador * 1000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tu turno:");
      lcd.setCursor(0, 1);
      lcd.print("Presiona boton");

      while (botonesApretados < contador) {
        unsigned long tiempoRestante = duracionTurno - (millis() - tiempoInicio);

        if (tiempoRestante <= 0) {
          analogWrite(TIMER, 0);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Tiempo Agotado");
          delay(1000);
          estadoActual = 0;
          indice = 0;
          contador = 0;
          delay(500);
          break;
        }

        int estadoTimer = map(tiempoRestante, 0, duracionTurno, 0, 255);
        analogWrite(TIMER, estadoTimer);

        if (digitalRead(BOTON_ROJO) == HIGH && botonRojoStateAnterior == LOW) {
          botonesElegidos[index] = 1;
          digitalWrite(LED_ROJO, HIGH);
          delay(500);
          digitalWrite(LED_ROJO, LOW);

          if (botonesElegidos[index] != rondasAnteriores[index]) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error: rojo");
            delay(1000);
            estadoActual = 0;
            indice = 0;
            contador = 0;
            delay(500);
            break;
          }
          index++;
          botonesApretados++;
        }
        botonRojoStateAnterior = digitalRead(BOTON_ROJO);

        if (digitalRead(BOTON_VERDE) == HIGH && botonVerdeStateAnterior == LOW) {
          botonesElegidos[index] = 2;
          digitalWrite(LED_VERDE, HIGH);
          delay(500);
          digitalWrite(LED_VERDE, LOW);

          if (botonesElegidos[index] != rondasAnteriores[index]) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error: verde");
            delay(1000);
            estadoActual = 0;
            indice = 0;
            contador = 0;
            delay(500);
            break;
          }
          index++;
          botonesApretados++;
        }
        botonVerdeStateAnterior = digitalRead(BOTON_VERDE);

        if (digitalRead(BOTON_AZUL) == HIGH && botonAzulStateAnterior == LOW) {
          botonesElegidos[index] = 3;
          digitalWrite(LED_AZUL, HIGH);
          delay(500);
          digitalWrite(LED_AZUL, LOW);

          if (botonesElegidos[index] != rondasAnteriores[index]) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error: azul");
            delay(1000);
            estadoActual = 0;
            indice = 0;
            contador = 0;
            delay(500);
            break;
          }
          index++;
          botonesApretados++;
        }
        botonAzulStateAnterior = digitalRead(BOTON_AZUL);

        if (digitalRead(BOTON_AMARILLO) == HIGH && botonAmarilloStateAnterior == LOW) {
          botonesElegidos[index] = 4;
          digitalWrite(LED_AMARILLO, HIGH);
          delay(500);
          digitalWrite(LED_AMARILLO, LOW);

          if (botonesElegidos[index] != rondasAnteriores[index]) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error: amarillo");
            delay(1000);
            estadoActual = 0;
            indice = 0;
            contador = 0;
            delay(500);
            break;
          }
          index++;
          botonesApretados++;
        }
        botonAmarilloStateAnterior = digitalRead(BOTON_AMARILLO);
      }

      if (estadoActual == 2) {
        analogWrite(TIMER, 0);
        estadoActual = 1;
      }
      break;
  }
}
