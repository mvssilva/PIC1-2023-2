#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); 

#define verdeLed 8
#define vermelhoLed 9
#define amareloLed 10
#define azulLed 11

#define verdeBotao 3
#define vermelhoBotao 4
#define amareloBotao 5
#define azulBotao 6

#define buzzer 12
#define iniciar 2

int numAtualSorteado = 0;
int numSorteados[10];

int numAtualEscolhido = 0;
int numEscolhidos[10];

int nivel = 0;
int nivelDisplay = 1;
int verifica = 0;

int delayDificuldade = 0;

bool iniciarAcionado = false;
bool sortear = false;

void setup() 
{
  lcd.init();  // INICIALIZAÇÃO DO DISPLAY
  lcd.backlight();

  Serial.begin(9600);
  pinMode(verdeBotao, INPUT_PULLUP); //LED VERDE
  pinMode(vermelhoBotao, INPUT_PULLUP); //LED VERMELHO
  pinMode(amareloBotao, INPUT_PULLUP); //LED AMARELO
  pinMode(azulBotao, INPUT_PULLUP); //LED AZUL

  pinMode(iniciar, INPUT_PULLUP); //BOTÃO INICIAR
  attachInterrupt(digitalPinToInterrupt(iniciar), iniciarPressionado, HIGH);

  pinMode(verdeLed, OUTPUT);  //BOTÃO LED VERDE
  pinMode(vermelhoLed, OUTPUT);  //BOTÃO LED VERMELHO
  pinMode(amareloLed, OUTPUT); //BOTÃO LED AMARERLO
  pinMode(azulLed, OUTPUT); //BOTÃO LED AZUL

  pinMode(buzzer, OUTPUT); //BUZZER
  
  randomSeed(analogRead(0));
}

void iniciarPressionado() 
{
    iniciarAcionado = !iniciarAcionado;
}

void piscaPisca(){
    digitalWrite(verdeLed, HIGH);
    delay(250);
    digitalWrite(amareloLed, HIGH);
    delay(250);
    digitalWrite(vermelhoLed, HIGH);
    delay(250);
    digitalWrite(azulLed, HIGH);
    delay(250);

    // Desligar todos os LEDs
    digitalWrite(verdeLed, LOW);
    delay(250);
    digitalWrite(amareloLed, LOW);
    delay(250);
    digitalWrite(vermelhoLed, LOW);
    delay(250);
    digitalWrite(azulLed, LOW);
    delay(250);

}

void inicializarJogo()
{
    digitalWrite(verdeLed, HIGH);
    digitalWrite(vermelhoLed, HIGH);
    digitalWrite(amareloLed, HIGH);
    digitalWrite(azulLed, HIGH);
    delay(600);
    digitalWrite(verdeLed, LOW);
    digitalWrite(vermelhoLed, LOW);
    digitalWrite(amareloLed, LOW);
    digitalWrite(azulLed, LOW);
    delay(600);

    nivel = 0;
    verifica = 0;
    sortear = true;
}

void mostrarSequencia()
{   
    lcd.setCursor(0, 0);   
    lcd.print("Lembre a ");
    lcd.setCursor(0, 1);   
    lcd.print("Sequencia!");
    
    Serial.println();
    Serial.print("NIVEL ATUAL - ");
    Serial.println(nivel);
    Serial.println("-- ORDEM CORRETA --");
    
    for(int i=0; i < nivel; i++)
    {   
        Serial.print("NIVEL - ");
        Serial.print(i);
        Serial.print(" - ");

        switch(numSorteados[i])
        {
            case 1:
                Serial.println("Verde");
                digitalWrite(verdeLed, HIGH);
                tone(buzzer, 100);
                delay(300-delayDificuldade);
                digitalWrite(verdeLed, LOW);
                noTone(buzzer);
                delay(300-delayDificuldade);
            break;

            case 2:
                Serial.println("Vermelho");
                digitalWrite(vermelhoLed, HIGH);
                tone(buzzer, 200);
                delay(300-delayDificuldade);
                digitalWrite(vermelhoLed, LOW);
                noTone(buzzer);
                delay(300-delayDificuldade);
            break;

            case 3:
                Serial.println("Amarelo");
                digitalWrite(amareloLed, HIGH);
                tone(buzzer, 300);
                delay(300-delayDificuldade);
                digitalWrite(amareloLed, LOW);
                noTone(buzzer);
                delay(300-delayDificuldade);
            break;
        
            case 4:
                Serial.println("Azul");
                digitalWrite(azulLed, HIGH);
                tone(buzzer, 400);
                delay(300-delayDificuldade);
                digitalWrite(azulLed, LOW);
                noTone(buzzer);
                delay(300-delayDificuldade);
            break;

            default:
                Serial.println("ERRO - CASO DEFAULT");
            break;
        }
    }
    Serial.println();

}

void definirSequencia()
{   
    if(nivel == 0)
    {
        for(int i = 0; i < 4 ; i++){
            numAtualSorteado = random(1, 5);
            numSorteados[nivel] = numAtualSorteado;
            nivel++;
        }
    }
    else
    {
        numAtualSorteado = random(1, 5);
        numSorteados[nivel] = numAtualSorteado;
        nivel++;
        delayDificuldade += 20;
    }

    mostrarSequencia();
}

void verificar(int pinoSaida, int som, int i)
{
    if(numAtualEscolhido == numSorteados[i])
    {   
        lcd.clear();
        lcd.setCursor(0, 0);   
        lcd.print("CORRETO!");
        lcd.setCursor(0, 1);   
        lcd.print("PARABENS!");
        digitalWrite(pinoSaida, HIGH);
        tone(buzzer, som);
        delay(300);
        digitalWrite(pinoSaida, LOW);
        noTone(buzzer);
        delay(300);
    }
    else
        escolhaErrada();
}

void escolhaErrada()
{       lcd.clear();
        lcd.setCursor(5, 0);   
        lcd.print("ERRADO");
        lcd.setCursor(0, 1);   
        lcd.print("TENTE NOVAMENTE");
    for(int i=0; i<=3; i++)
    {
        noTone(buzzer);
        digitalWrite(verdeLed, HIGH);
        digitalWrite(amareloLed, HIGH);
        tone(buzzer, 100);
        delay(100);
        
        noTone(buzzer);
        tone(buzzer, 200);
        digitalWrite(verdeLed, LOW);
        digitalWrite(amareloLed, LOW);
        delay(100);

        noTone(buzzer);
        tone(buzzer, 300);
        digitalWrite(vermelhoLed, HIGH);
        digitalWrite(azulLed, HIGH);
        delay(100);

        noTone(buzzer);
        tone(buzzer, 400);
        digitalWrite(vermelhoLed, LOW);
        digitalWrite(azulLed, LOW);
        delay(100);
    }

    noTone(buzzer);

    for(int i = 0; i < nivel; i++)
    {
        numSorteados[i] = 0;
    }
    delayDificuldade = 0;
    sortear = false;
    iniciarAcionado = false;
}

void respostaUsuario()
{   
    lcd.clear();
    lcd.setCursor(0, 0);   
    lcd.print("Aguardando");
    lcd.setCursor(0, 1);   
    lcd.print("sua resposta!");
    int i = 0;
    while(i < nivel){
                while (digitalRead(verdeBotao) == HIGH && 
                    digitalRead(vermelhoBotao) == HIGH && 
                    digitalRead(amareloBotao) == HIGH && 
                    digitalRead(azulBotao) == HIGH){
                      delay(30);
                    }
                
            
            if(digitalRead(verdeBotao) == 0)
            {   
                Serial.println("Verde");
                numAtualEscolhido = 1;
                verificar(verdeLed, 100, i);
            }
            else if(digitalRead(vermelhoBotao) == 0)
            {
                Serial.println("Vermelho");
                numAtualEscolhido = 2;
                verificar(vermelhoLed, 200, i);
            }
            else if(digitalRead(amareloBotao) == 0)
            {
                Serial.println("Amarelo");
                numAtualEscolhido = 3;
                verificar(amareloLed, 300, i);
            }
            else if(digitalRead(azulBotao) == 0)
            {
                Serial.println("Azul");
                numAtualEscolhido = 4;
                verificar(azulLed, 400, i);
            }

            if(sortear == false)
                break;
        i++;
    }

}

void loop()
{   
    lcd.setCursor(0, 0); 
    lcd.print("Projeto PIC 1");
    lcd.setCursor(0, 1); 
    lcd.print("Jogo Genius!");
    piscaPisca();

    if (iniciarAcionado == true)
    {    
        lcd.clear();
        lcd.setCursor(0, 0);   
        lcd.print("Inicializando!");
        lcd.setCursor(0, 1);   
        lcd.print("Boa Sorte!");
        inicializarJogo();
        delay(500);
        
        while (sortear)
        {   
            lcd.clear();
            definirSequencia();
            delay(300);
            respostaUsuario();
        }  
        lcd.clear();

    }

}