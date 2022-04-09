#include <Wire.h> // Necesario para trabajar con el bus I2C
//#include <utilitytwi.h> // Necesario para la constante CPU_FREQ
int up, down, left, right, select, start, A, B;
int b6, b7;
void setup()
{
  Serial.begin(9600); // Inicializamos el puerto serie a una velocidad baja para que se puedan ver los datos
  Wire.begin(); // Inicializamos la librería wire para trabajar con I2C
  //TWBR = ((CPU_FREQ / 400000L) - 16) / 2; // Configuramos el I2C para el modo Fast (400Kb/s)

  // Inicializamos el nunchuk sin encriptación
  Wire.beginTransmission(0x52);
  Wire.write(0xF0);
  Wire.write(0x55);
  Wire.endTransmission();

  Wire.beginTransmission(0x52);
  Wire.write(0xFB);
  Wire.write(0x00);
  Wire.endTransmission();
}

void loop ()
{
  ctrlUpdate();
  printValues();
}

// Imprimimos los datos en el puerto serie
void printValues()
{
  Serial.print("UP: ");
  Serial.print(up);
  Serial.print(" DOWN: ");
  Serial.print(down);
  Serial.print(" LEFT: ");
  Serial.print(left);
  Serial.print(" RIGHT: ");
  Serial.print(right);
  Serial.print(" A: ");
  Serial.print(A);
  Serial.print(" B: ");
  Serial.print(B);
  Serial.print(" SELECT: ");
  Serial.print(select);
  Serial.print(" START: ");
  Serial.println(start);
}

void ctrlUpdate()
{
  uint8_t buffer[6];

  // Pedimos que se actualicen los datos
  Wire.beginTransmission(0x52);
  Wire.write(0);
  Wire.endTransmission();

  // Recogemos y guardamos los datos
  Wire.requestFrom(0x52, 8);

  for (int indice = 0; indice < 8; indice++)
  {
    buffer[indice] = Wire.read();
    //Serial.print(int(buffer[indice]));         // print the character
    //Serial.print(",");
    if (indice == 6)b6 = int(buffer[indice]);
    if (indice == 7)b7 = int(buffer[indice]);
  }
  if (b6 == 127 || b6 == 63 || b6 == 111 || b6 == 123) right = 1; else right = 0;
  if (b6 == 191 || b6 == 63 || b6 == 175 || b6 == 187) down = 1; else down = 0;
  if (b6 == 239 || b6 == 111 || b6 == 175 || b6 == 227) select = 1; else select = 0;
  if (b6 == 251 || b6 == 123 || b6 == 187 || b6 == 227) start = 1; else start = 0;

  if (b7 == 254 || b7 == 252 || b7 == 238 || b7 == 190) up = 1; else up = 0;
  if (b7 == 253 || b7 == 252 || b7 == 237 || b7 == 189) left = 1; else left = 0;
  if (b7 == 239 || b7 == 238 || b7 == 237 || b7 == 175) A = 1; else A = 0;
  if (b7 == 191 || b7 == 190 || b7 == 189 || b7 == 175) B = 1; else B = 0;
}
