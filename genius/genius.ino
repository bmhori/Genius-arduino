#define NOTE_D4 29 
#define NOTE_G4 39 
#define NOTE_A4 44 
#define NOTE_A5 88 

int tons[4] = {NOTE_A5, NOTE_A4, NOTE_G4, NOTE_D4};
int sequencia[100] = {};
bool iniciar_jogo = false;
int rodada_atual = 0;
int passo_atual_na_sequencia = 0;

int pinoAudio = 8;

int pinosLeds[4] = {7, 6, 5, 4};
int pinosBotoes[4] = {12, 11, 10, 9};
const byte pinoStart = 3;
int botao_pressionado = 0;
bool perdeu_o_jogo = false;

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial
  for (int i = 0; i <= 3; i++) {
    pinMode(pinosLeds[i], OUTPUT);
    pinMode(pinosBotoes[i], INPUT_PULLUP);  // Configura os botões com resistores de pull-up internos
  }
  pinMode(pinoStart, INPUT);
  pinMode(pinoAudio, OUTPUT);
  randomSeed(analogRead(0));
  
  attachInterrupt(digitalPinToInterrupt(pinoStart),iniciarJogo, FALLING);
  reiniciarJogo();
}

void loop() {
  if (perdeu_o_jogo) {
    reiniciarJogo();
    Serial.println("problema");
  }
  
  if(iniciar_jogo == true){ 
    if (rodada_atual == 0) {
      tocarSomDeInicio();
      delay(500);
    }
    reproduzirSequencia();
    aguardarJogador();
    proximaRodada();
    delay(1000);
  }
}

void reiniciarJogo() {
  for (int i = 0; i <= 3; i++) {
    digitalWrite(pinosLeds[i], LOW);
  }
  int sequencia[100] = {};
  rodada_atual = 0;
  passo_atual_na_sequencia = 0;
  perdeu_o_jogo = false;
  iniciar_jogo = false;
}

void reproduzirSequencia() {
  for (int i = 0; i < rodada_atual; i++) {
    if(iniciar_jogo == false){
      break;
    }
    tone(pinoAudio, tons[sequencia[i]]);
    digitalWrite(pinosLeds[sequencia[i]], HIGH);
    delay(500);
    noTone(pinoAudio);
    digitalWrite(pinosLeds[sequencia[i]], LOW);
    delay(100);
  }
  noTone(pinoAudio);
}

void aguardarJogador() {
  for (int i = 0; i < rodada_atual; i++) {
    if(iniciar_jogo == false){
      break;
    }
    aguardarJogada();
    verificarJogada();
    if (perdeu_o_jogo) {
      break;
    }
    passo_atual_na_sequencia++;
  }
  passo_atual_na_sequencia = 0;
}

void aguardarJogada() {
  boolean jogada_efetuada = false;
  while (jogada_efetuada == 0) {
    for (int i = 0; i <= 3; i++) {
      if(iniciar_jogo == false){
        break;
      }
      int estadoBotao = digitalRead(pinosBotoes[i]);
      //Serial.print("Botão ");
      //Serial.print(i);
      //Serial.print(": ");
     // Serial.println(estadoBotao);

      if (estadoBotao == LOW) {
        botao_pressionado = i;
        tone(pinoAudio, tons[i]);
        digitalWrite(pinosLeds[i], HIGH);
        delay(300);
        digitalWrite(pinosLeds[i], LOW);
        noTone(pinoAudio);
        jogada_efetuada = true;
      }
    }
    delay(10);
  }
}

void verificarJogada() {
  if (sequencia[passo_atual_na_sequencia] != botao_pressionado) {
    perdeu_o_jogo = true;
    for (int i = 0; i <= 3; i++) {
      tone(pinoAudio, tons[i]);
      digitalWrite(pinosLeds[i], HIGH);
      delay(200);
      digitalWrite(pinosLeds[i], LOW);
      noTone(pinoAudio);
    }
    tone(pinoAudio, tons[3]);
    for (int i = 0; i <= 3; i++) {
      digitalWrite(pinosLeds[i], HIGH);
      delay(100);
      digitalWrite(pinosLeds[i], LOW);
      delay(100);
    }
    noTone(pinoAudio);
  }
}

void tocarSomDeInicio() {
  tone(pinoAudio, tons[0]);
  for (int i = 0; i <= 3; i++) {
    digitalWrite(pinosLeds[i], HIGH);
  }
  delay(500);
  for (int i = 0; i <= 3; i++) {
    digitalWrite(pinosLeds[i], LOW);
  }
  delay(500);
  noTone(pinoAudio);
}

void proximaRodada() {
  
  sequencia[rodada_atual] = random(4); 
  rodada_atual++;
  passo_atual_na_sequencia = 0;
}

void iniciarJogo() {
  iniciar_jogo = true;
  Serial.println(iniciar_jogo);
}
