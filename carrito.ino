// Definición de pines del sensor ultrasónico
const int echo = 2; 
const int trig = 3;
const int cerca = 30; // Distancia crítica para detectar obstáculos (en cm), aumentada a 30 cm
const int duracionmaximapulso = 5000; // Tiempo máximo de espera para el pulso (5 ms)

// Definición de pines del controlador de motores
const int ENA = 11; // Pin para el motor izquierdo
const int Izq1 = 9; // Pin de dirección del motor izquierdo
const int Izq2 = 8; // Pin de dirección inversa del motor izquierdo
const int dere1 = 6; // Pin de dirección del motor derecho
const int dere2 = 7; // Pin de dirección inversa del motor derecho
const int ENB = 10; // Pin para el motor derecho

// Variables globales
volatile unsigned long distancia;
const int velocidadAvanceIzquierda = 100; // Velocidad para avanzar del motor izquierdo
const int velocidadAvanceDerecha = 110;  // Velocidad para avanzar del motor derecho (ajustar para compensar desvío)
const int velocidadRetroceso = 100; // Velocidad para retroceder
const int tiempoRetroceso = 300; // Tiempo en milisegundos para retroceder (ajustar según la distancia)
const int tiempoGiro = 300; // Tiempo en milisegundos para girar (ajustar según sea necesario)
const int velocidadGiroIzquierda = 120; // Velocidad para girar del motor izquierdo
const int velocidadGiroDerecha = 120; // Velocidad para girar del motor derecho

void setup() {
  // Configuración de pines del sensor
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Configuración de pines del controlador de motores
  pinMode(ENA, OUTPUT);
  pinMode(Izq1, OUTPUT);
  pinMode(Izq2, OUTPUT);
  pinMode(dere2, OUTPUT);
  pinMode(dere1, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  medir(); // Medir la distancia

  if (distancia < cerca) {
    obstaculo(); // Manejar el obstáculo si la distancia es menor que el umbral
  } else {
    adelante(); // Mover hacia adelante cuando no hay obstáculo
  }
}

// Función para medir la distancia usando el sensor HC-SR04
void medir() {
  // Emitir pulso
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Leer duración del pulso
  unsigned long duracion = pulseIn(echo, HIGH, duracionmaximapulso);

  // Convertir duración a distancia (en cm)
  distancia = (duracion * 0.0343) / 2;

  // Asignar un valor máximo en caso de lectura incorrecta
  if (distancia == 0 || distancia > cerca) {
    distancia = cerca;
  }

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
}

// Función para mover el robot hacia adelante en línea recta
void adelante() {
  digitalWrite(Izq1, HIGH);  // Motor izquierdo hacia adelante
  digitalWrite(Izq2, LOW);
  digitalWrite(dere2, LOW);   // Motor derecho hacia adelante
  digitalWrite(dere1, HIGH);

  analogWrite(ENA, velocidadAvanceIzquierda); // Ajustar velocidad de avance del motor izquierdo
  analogWrite(ENB, velocidadAvanceDerecha); // Ajustar velocidad de avance del motor derecho
}

// Función para retroceder el robot
void retroceder() {
  digitalWrite(Izq1, LOW); // Motor izquierdo hacia atrás
  digitalWrite(Izq2, HIGH);
  digitalWrite(dere2, HIGH); // Motor derecho hacia atrás
  digitalWrite(dere1, LOW);

  analogWrite(ENA, velocidadRetroceso); // Ajustar velocidad de retroceso
  analogWrite(ENB, velocidadRetroceso);

  delay(tiempoRetroceso); // Retroceder por un tiempo definido
}

// Función para girar el robot a la derecha
void derecha() {
  digitalWrite(Izq1, HIGH);
  digitalWrite(Izq2, LOW);
  digitalWrite(dere2, HIGH);
  digitalWrite(dere1, LOW);

  analogWrite(ENA, velocidadGiroIzquierda); // Ajustar velocidad del motor izquierdo durante el giro
  analogWrite(ENB, velocidadGiroDerecha); // Ajustar velocidad del motor derecho durante el giro
}

// Función para detener el robot
void detenido() {
  digitalWrite(Izq1, LOW);
  digitalWrite(Izq2, LOW);
  digitalWrite(dere2, LOW);
  digitalWrite(dere1, LOW);
  
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// Función para manejar el obstáculo
void obstaculo() {
  detenido();       // Detener el robot
  delay(300);       // Esperar para evitar colisiones inminentes
  
  retroceder();     // Retroceder un poco
  detenido();       // Detener el robot después de retroceder
  
  derecha();        // Girar a la derecha
  delay(tiempoGiro); // Ajustar el tiempo de giro según el tiempoGiro
  
  detenido();       // Detener el robot después del giro
  delay(500);       // Esperar antes de volver a avanzar
}
