  /**********************/
 /*     DEPURACION     */
/**********************/
#define DEPURACION false

  /******************/
 /*     MACROS     */
/******************/
#define TIEMPO_EN (int)1
#define NUM_PUERTOS (int)4

#define RS (byte)12
#define RW (byte)11
#define E  (byte)10

#define D4 (byte)5
#define D5 (byte)4
#define D6 (byte)3
#define D7 (byte)2

  /********************************/
 /*     PROTOTIPOS FUNCIONES     */
/********************************/
void inicializacion();
void comando( int codigo );
void habilitarComando();
void escribirLCD(char caracter);
char leerCaracter();
void establecerDireccion( int dir );

 
 /*********************/
 /*     VARIABLES     */
/*********************/
int mLcdPuertos[NUM_PUERTOS];

  /*****************/
 /*     SETUP     */
/*****************/
void setup() {
  pinMode( LED_BUILTIN, OUTPUT);
  digitalWrite( LED_BUILTIN , LOW );
  Serial.begin(9600);

  inicializacion();
  for(int cont = 0, i = 0; i < 15; cont+=2, i++)
  {
    escribirLCD('0'+cont);
    delay(100);
  }

  establecerDireccion(0x00);
  
}

  /****************/
 /*     LOOP     */
/****************/
void loop() {
  digitalWrite( LED_BUILTIN , HIGH );
  delay(500);
  digitalWrite( LED_BUILTIN , LOW );
  Serial.println( leerCaracter() );
  delay(500);
}

  /**************************/
 /*     INICIALIZACION     */
/**************************/
void inicializacion()
{
  mLcdPuertos[ 0 ] = D4;
  mLcdPuertos[ 1 ] = D5;
  mLcdPuertos[ 2 ] = D6;
  mLcdPuertos[ 3 ] = D7;

  pinMode( RS , OUTPUT );
  digitalWrite( RS , LOW);
  pinMode( RW , OUTPUT );
  digitalWrite( RW , LOW );
  pinMode( E  , OUTPUT );
  digitalWrite( E , LOW );

  for(byte indx = 0 ; indx < NUM_PUERTOS  ; indx++)
  {
    pinMode( mLcdPuertos[indx] , OUTPUT);
    digitalWrite( mLcdPuertos[indx] , LOW);
  }

  comando( 0x33 );
  comando( 0x32 );
  comando( 0x21 );
  comando( 0x0F );
  comando( 0x01 );
}

  /*******************/
 /*     COMANDO     */
/*******************/
void comando(int codigo)
{
  int Val = 0;
  digitalWrite( RS , LOW );
  digitalWrite( RW , LOW );
  digitalWrite( E  , LOW );

  for(byte indx = 0 ; indx < NUM_PUERTOS  ; indx++)
  {
    pinMode( mLcdPuertos[indx] , OUTPUT);
  }

  for(byte iteraciones = 0 ; iteraciones < 2 ; iteraciones++)
  {
    for(byte indx = 0 ; indx < NUM_PUERTOS ; indx++)
    {
      Val = ( (codigo & 0x80) == 0x80) ? HIGH : LOW;
      digitalWrite( mLcdPuertos[ NUM_PUERTOS - 1 - indx ] , Val );
      codigo <<= 1;
    }
    
    habilitarComando();
  }
}

  /*****************************/
 /*     HABILITAR COMANDO     */
/*****************************/
void habilitarComando()
{
  digitalWrite( E , HIGH );
  delay(TIEMPO_EN);
  digitalWrite( E , LOW );
  delay(TIEMPO_EN);
}

  /*************************/
 /*     LEER CARACTER     */
/*************************/
char leerCaracter(){

  digitalWrite( RS , HIGH );
  digitalWrite( RW , HIGH );
  digitalWrite( E  , LOW );

  for(byte indx = 0 ; indx < NUM_PUERTOS  ; indx++)
  {
    pinMode( mLcdPuertos[indx] , INPUT);
  }

  unsigned int dato = 0;
  for(byte iteraciones = 0 ; iteraciones < 2 ; iteraciones++)
  {
    digitalWrite( E  , HIGH );
    for(byte indx = 0 ; indx < NUM_PUERTOS ; indx++)
    {
      dato <<= 1;
      dato |= ( digitalRead( mLcdPuertos[ NUM_PUERTOS - 1 - indx ] ) ) ? 0x01 : 0x00 ;
      #if (DEPURACION == true)
      Serial.print( dato, HEX );
      Serial.print( ( indx < NUM_PUERTOS - 1 ) ? " - " : " " );    
      #endif
    }
    delay(10);
    digitalWrite( E  , LOW );
    #if (DEPURACION == true)
    Serial.println("");
    Serial.println( "=====" );
    #endif
  }
  return dato;
}

  /*******************************/
 /*     ESTABLECE DIRECCION     */
/*******************************/
void establecerDireccion( int dir )
{  
  dir = dir | 0x80;
  digitalWrite( RS , LOW );
  digitalWrite( RW , LOW );
  digitalWrite( E  , LOW );

  for(byte indx = 0 ; indx < NUM_PUERTOS  ; indx++)
  {
    pinMode( mLcdPuertos[indx] , OUTPUT);
  }

  for(byte iteraciones = 0 ; iteraciones < 2 ; iteraciones++)
  {
    for(byte indx = 0 ; indx < NUM_PUERTOS ; indx++)
    {
      digitalWrite( mLcdPuertos[ NUM_PUERTOS - 1 - indx ] ,
                   ( (dir & 0x80) == 0x80) ? HIGH : LOW );
      dir <<= 1;
    }

    habilitarComando();
  }
}

  /************************/
 /*     ESCRIBIR LCD     */
/************************/
void escribirLCD(char caracter)
{
  
  digitalWrite( RS , HIGH );
  digitalWrite( RW , LOW );
  digitalWrite( E  , LOW );

  for(byte indx = 0 ; indx < NUM_PUERTOS  ; indx++)
  {
    pinMode( mLcdPuertos[indx] , OUTPUT);
  }

  for(byte iteraciones = 0 ; iteraciones < 2 ; iteraciones++)
  {
    for(byte indx = 0 ; indx < NUM_PUERTOS ; indx++)
    {
      digitalWrite( mLcdPuertos[ NUM_PUERTOS - 1 - indx ] ,
                   ( (caracter & 0x80) == 0x80) ? HIGH : LOW );
      caracter <<= 1;
    }

    habilitarComando();
  }
}
