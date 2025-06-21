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

const int blinkDelay[3] = {1000, 600, 300};

// Variables para mostrar secuencia
bool mostrandoSecuencia = false;
unsigned long tiempoSecuenciaAnterior = 0;
int indiceSecuencia = 0;
int estadoLed = 0;
int delayActual = 1000;

// Efecto de botón sin delay
bool ledRojoActivo = false, ledVerdeActivo = false, ledAzulActivo = false, ledAmarilloActivo = false;
unsigned long tiempoLedRojo = 0, tiempoLedVerde = 0, tiempoLedAzul = 0, tiempoLedAmarillo = 0;

// Error sin delay
bool mostrandoError = false;
unsigned long tiempoError = 0;
String mensajeError = "";

unsigned long tiempoVictoria = 0;
bool primerIngresoVictoria = true;


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

void scrollCiclicoTexto(String texto, int fila, int delayScroll = 250) {
  static unsigned long ultimaActualizacion = 0;
  static int posicion = 0;

  if (millis() - ultimaActualizacion > delayScroll) {
    if (texto.length() <= 16) {
      lcd.setCursor(0, fila);
      lcd.print(texto + "                "); // Limpia línea
    } else {
      String ventana = texto.substring(posicion, posicion + 16);
      lcd.setCursor(0, fila);
      lcd.print(ventana);
      posicion++;
      if (posicion > texto.length() - 16) {
        posicion = 0;
      }
    }
    ultimaActualizacion = millis();
  }
}

void mostrarSecuencia(int nivel) {
  delayActual = blinkDelay[nivel];
  if (indiceSecuencia >= contador + 1) {
    mostrandoSecuencia = false;
    estadoActual = 2;
    return;
  }

  if (estadoLed == 0 && millis() - tiempoSecuenciaAnterior >= 200) {
    switch (rondasAnteriores[indiceSecuencia]) {
      case 1: digitalWrite(LED_ROJO, HIGH); break;
      case 2: digitalWrite(LED_VERDE, HIGH); break;
      case 3: digitalWrite(LED_AZUL, HIGH); break;
      case 4: digitalWrite(LED_AMARILLO, HIGH); break;
    }
    estadoLed = 1;
    tiempoSecuenciaAnterior = millis();
  }
  else if (estadoLed == 1 && millis() - tiempoSecuenciaAnterior >= delayActual) {
    digitalWrite(LED_ROJO, LOW); digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AZUL, LOW); digitalWrite(LED_AMARILLO, LOW);
    estadoLed = 0;
    indiceSecuencia++;
    tiempoSecuenciaAnterior = millis();
  }
}

void loop() {
  switch (estadoActual) {
    case 0:
      analogWrite(TIMER, 0);
      lcd.clear();
      while (digitalRead(BOTON_INICIO) == LOW) {
        int pot = analogRead(POT_PIN);
        nivel = map(pot, 0, 1023, 0, 2);
        const char* nombres[] = {"Facil", "Medio", "Dificil"};
        lcd.setCursor(0, 0);
        lcd.print("Presione Inicio ");
        lcd.setCursor(0, 1);
        lcd.print("Nivel: ");
        lcd.print(nombres[nivel]);
        delay(100);
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

      indiceSecuencia = 0;
      mostrandoSecuencia = true;
      estadoLed = 0;
      tiempoSecuenciaAnterior = millis();
      contador++;
      estadoActual = 3;
      break;

    case 3:
      mostrarSecuencia(nivel);
      break;

    case 2: {
      botonesApretados = 0;
      index = 0;
      static unsigned long tiempoInicio = millis();
      int duracionTurno = 10000 + (contador * 1000);

      lcd.setCursor(0, 0);
      lcd.print("Tu turno:");
      lcd.setCursor(0, 1);
      lcd.print("Presiona boton");

      while (botonesApretados < contador) {
        unsigned long tiempoRestante = duracionTurno - (millis() - tiempoInicio);
        if (tiempoRestante <= 0) {
          analogWrite(TIMER, 0);
          mensajeError = "Tiempo Agotado";
          estadoActual = 4;
          mostrandoError = true;
          tiempoError = millis();
          return;
        }

        int estadoTimer = map(tiempoRestante, 0, duracionTurno, 0, 255);
        analogWrite(TIMER, estadoTimer);

        if (digitalRead(BOTON_ROJO) == HIGH && botonRojoStateAnterior == LOW) {
          botonesElegidos[index] = 1;
          digitalWrite(LED_ROJO, HIGH);
          ledRojoActivo = true;
          tiempoLedRojo = millis();
          if (botonesElegidos[index] != rondasAnteriores[index]) {
            mensajeError = "Error: rojo";
            estadoActual = 4;
            mostrandoError = true;
            tiempoError = millis();
            return;
          }
          index++; botonesApretados++;
        }
        if (ledRojoActivo && millis() - tiempoLedRojo >= 500) {
          digitalWrite(LED_ROJO, LOW); ledRojoActivo = false;
        }
        botonRojoStateAnterior = digitalRead(BOTON_ROJO);

        if (digitalRead(BOTON_VERDE) == HIGH && botonVerdeStateAnterior == LOW) {
          botonesElegidos[index] = 2;
          digitalWrite(LED_VERDE, HIGH);
          ledVerdeActivo = true;
          tiempoLedVerde = millis();
          if (botonesElegidos[index] != rondasAnteriores[index]) {
            mensajeError = "Error: verde";
            estadoActual = 4;
            mostrandoError = true;
            tiempoError = millis();
            return;
          }
          index++; botonesApretados++;
        }
        if (ledVerdeActivo && millis() - tiempoLedVerde >= 500) {
          digitalWrite(LED_VERDE, LOW); ledVerdeActivo = false;
        }
        botonVerdeStateAnterior = digitalRead(BOTON_VERDE);

        if (digitalRead(BOTON_AZUL) == HIGH && botonAzulStateAnterior == LOW) {
          botonesElegidos[index] = 3;
          digitalWrite(LED_AZUL, HIGH);
          ledAzulActivo = true;
          tiempoLedAzul = millis();
          if (botonesElegidos[index] != rondasAnteriores[index]) {
            mensajeError = "Error: azul";
            estadoActual = 4;
            mostrandoError = true;
            tiempoError = millis();
            return;
          }
          index++; botonesApretados++;
        }
        if (ledAzulActivo && millis() - tiempoLedAzul >= 500) {
          digitalWrite(LED_AZUL, LOW); ledAzulActivo = false;
        }
        botonAzulStateAnterior = digitalRead(BOTON_AZUL);

        if (digitalRead(BOTON_AMARILLO) == HIGH && botonAmarilloStateAnterior == LOW) {
          botonesElegidos[index] = 4;
          digitalWrite(LED_AMARILLO, HIGH);
          ledAmarilloActivo = true;
          tiempoLedAmarillo = millis();
          if (botonesElegidos[index] != rondasAnteriores[index]) {
            mensajeError = "Error: amarillo";
            estadoActual = 4;
            mostrandoError = true;
            tiempoError = millis();
            return;
          }
          index++; botonesApretados++;
        }
        if (ledAmarilloActivo && millis() - tiempoLedAmarillo >= 500) {
          digitalWrite(LED_AMARILLO, LOW); ledAmarilloActivo = false;
        }
        botonAmarilloStateAnterior = digitalRead(BOTON_AMARILLO);
      }

      analogWrite(TIMER, 0);
	  if(contador == 2){  // ejemplo: si llega a ronda 10 gana
         estadoActual = 5;  // ir a pantalla de victoria
         } else {
                 estadoActual = 1;
                 }

      break;
    }

    case 4:
      if (mostrandoError) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(mensajeError);
        mostrandoError = false;
        tiempoError = millis();
      }
      if (millis() - tiempoError >= 1000) {
        estadoActual = 0;
        indice = 0;
        contador = 0;
      }
      break;
  
    
case 5:
  if (primerIngresoVictoria) {
    lcd.clear();
    primerIngresoVictoria = false;
    tiempoVictoria = millis();
  }
  scrollCiclicoTexto("¡Felicitaciones! Ganaste el juego.", 0, 200);
  lcd.setCursor(0,1);
  lcd.print("Reiniciando...");

  if (millis() - tiempoVictoria > 5000) {  // 5 segundos mostrando mensaje
    estadoActual = 0;
    contador = 0;
    indice = 0;
    primerIngresoVictoria = true;  // reset para la próxima vez
  }
  break;


  }
}
