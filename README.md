Bienvenidos al proyecto del laboratorio de computación 1 de la tecnicatura de programación de la Universidad de San Martín 2025:
Grupo 10
IDEA DEL PROYECTO:
La elaboración del juego simón dice  a través del microcontrolador Arduino uno.

Miembros del grupo:
Barraza Alejo
Eckart Lucas
Mendez Nicolas
Daniel Agustín Moreyra 

DIVISION DE GRUPO:

1 Diseño del esquema y conexión física del proyecto: dibujo del circuito (LEDs, botones, etc.), conexión del protoboard y armado del diagrama para presentar.
2 Programación de los  botones (flancos) y LEDs, reconoce cuándo se presiona un botón.
3 Programación de los estados del juego, los temporizadores, y la lógica del juego.
4 Preparación de la presentación y armado del esquema  en vivo. Explicación  del funcionamiento a los docentes además de escribir comentarios en el código.


MATERIALES
Entradas:
4 botones (pueden ser de la botonera 4x3)

Salidas:
4 LEDs (de colores diferentes)

Potenciómetro para elegir la velocidad (fácil, medio, difícil)

ejecución del juego

 Estado       Qué hace el sistema                       
  
 `INICIO`     Muestra “Pulsa para empezar” en el LCD    
 `MOSTRAR`    Enciende la secuencia de LEDs             
 `JUGADOR`    Espera que el jugador repita la secuencia 
 `VERIFICAR`  Compara las respuestas                    
 `GANASTE`    Muestra mensaje y pasa de nivel           
 `PERDISTE`   Reinicia el juego                         

![Vista previa del juego](tp-final.png)
