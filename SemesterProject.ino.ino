

// global variables for the LEDS 
int livesLED_1 = 2;
int livesLED_2 = 3;
int livesLED_3 = 4;
int livesLED_4 = 10;
int livesLED_5 = 11;
int livesLED_6 = 12;
int player1Win =  5;
int player2Win =  9;

int score = 0;

int p1_lives = 3;  // Player 1 has 3 lives
int p2_lives = 3;  // Player 2 has 3 lives

int player1Score;// ignore
int player2Score;// ignore

int p1_lives_prev = 0; // initial value
int p2_lives_prev = 0;
//int blocksinthescreen; // blocks that are in the screen 
//int playerSlider1; // slider
//int playerSlider2; // slider 

int pong1; // balls for the player to use to hit blocks. 
int pong2; // ball for player 2 

int joystick1; // might not need in this code 
int joystick2; // might not need in this code 

bool  gameOver; 


void setup() {
  Serial.begin(115200);

    
  pinMode(livesLED_1, OUTPUT);
  pinMode(livesLED_2, OUTPUT);
  pinMode(livesLED_3, OUTPUT);
  pinMode(livesLED_4, OUTPUT);
  pinMode(livesLED_5, OUTPUT);
  pinMode(livesLED_6, OUTPUT);
  pinMode(player1Win, OUTPUT);
  pinMode(player2Win, OUTPUT);

  digitalWrite(livesLED_1, HIGH);
  digitalWrite(livesLED_2, HIGH);
  digitalWrite(livesLED_3, HIGH);
  digitalWrite(livesLED_4, HIGH);
  digitalWrite(livesLED_5, HIGH);
  digitalWrite(livesLED_6, HIGH);

  digitalWrite(player1Win, LOW);
  digitalWrite(player2Win, LOW);
}

byte bytebuff[2];

byte p2_byte;
byte p1_byte;

byte p1_has_won = 0;
byte p1_score = 0;
byte p1_redlives = 0;

byte p2_has_won = 0;
byte p2_score = 0;
byte p2_redlives = 0;

void updateLives(int p1_lives, int p2_lives) {
  if (p1_lives < p1_lives_prev) {
    if (p1_lives == 2) {
      digitalWrite(livesLED_3, LOW);
    } else if (p1_lives == 1) {
      digitalWrite(livesLED_2, LOW);
    } else if (p1_lives == 0) {
      digitalWrite(livesLED_1, LOW);
      // player 2 wins, game over
      gameOver = true;
    }
    p1_lives_prev = p1_lives;
  }
  
  if (p2_lives < p2_lives_prev) {
    if (p2_lives == 2) {
      digitalWrite(livesLED_6, LOW);
    } else if (p2_lives == 1) {
      digitalWrite(livesLED_5, LOW);
    } else if (p2_lives == 0) {
      digitalWrite(livesLED_4, LOW);
      // player 1 wins, game over
      gameOver = true;
    }
    p2_lives_prev = p2_lives;
  }
}

//
void displayLives(byte p1, byte p2)
{
  if (p1 == 3) {
    digitalWrite(livesLED_1, HIGH);
    digitalWrite(livesLED_2, HIGH);
    digitalWrite(livesLED_3, HIGH);
  }
  if (p1 == 2) {
  digitalWrite(livesLED_1, HIGH);
  digitalWrite(livesLED_2, HIGH);
  digitalWrite(livesLED_3, LOW);
  }
  if (p1 == 1) {
  digitalWrite(livesLED_1, HIGH);
  digitalWrite(livesLED_2, LOW);
  digitalWrite(livesLED_3, LOW);
  }
  if (p1 == 0) {
    digitalWrite(livesLED_1, LOW);
    digitalWrite(livesLED_2, LOW);
    digitalWrite(livesLED_3, LOW);
  }
  if (p1 == 3) {
    digitalWrite(livesLED_1, HIGH);
    digitalWrite(livesLED_2, HIGH);
    digitalWrite(livesLED_3, HIGH);
  }
  if (p2 == 2) {
  digitalWrite(livesLED_1, HIGH);
  digitalWrite(livesLED_2, HIGH);
  digitalWrite(livesLED_3, LOW);
  }
  if (p2 == 1) {
  digitalWrite(livesLED_1, HIGH);
  digitalWrite(livesLED_2, LOW);
  digitalWrite(livesLED_3, LOW);
  }
  if (p2 == 0) {
    digitalWrite(livesLED_1, LOW);
    digitalWrite(livesLED_2, LOW);
    digitalWrite(livesLED_3, LOW);
  }

}

// 0-15 to 0-255
// analogWrite is a range from 0 - 255
#define THRESHOLD 3
void displayScore(byte score1, byte score2){
  //byte p1_normalized = score1 * (255 / 15); 
  //byte p2_normalized = score2 * (255 / 15);
  if(score1 >= THRESHOLD) {
    analogWrite(player1Win, 150);
  }
  if(score2 >= THRESHOLD) {
    analogWrite(player2Win, 150);
  }
}

byte win_old = 0;
void signal(byte win) {
  if(win_old == win) {
    return;
  }
  if(win) {
    Serial.write(0x1);
  }
  win_old = win;
}

byte displayWin(byte p1_win, byte p2_win) {
  if (p1_win) {
    digitalWrite(player1Win, HIGH);
    signal(1);
    return 1;
  } else { 
    digitalWrite(player1Win, LOW);
    // lose game if player 2 gets more points;
  }

  if (p2_win) { 
    digitalWrite(player2Win, HIGH);
    signal(1);
    return 1;
  } else { 
    digitalWrite(player2Win, LOW); 
    // lose game
  }
  signal(0);
  return 0;
}

void loop() {
  if (Serial.available() > 0) {
    Serial.readBytes(bytebuff, 2);
    //Serial.println(bytebuff[0], BIN); // 
    //Serial.println(bytebuff[1], BIN); // 
    if(bytebuff[1] & (1 << 6)) {
      p2_byte = bytebuff[1];
      p1_byte = bytebuff[0];
    } else {
      p2_byte = bytebuff[0];
      p1_byte = bytebuff[1];
    }
    p1_has_won = p1_byte & (1 << 5);
    p1_score = (p1_byte & 0x01C) >> 2;
    p1_redlives = p1_byte & 0x03;

    p2_has_won = p2_byte & (1 << 5);
    p2_score = (p2_byte & 0x01C) >> 2;
    p2_redlives = p2_byte & 0x03;
    
    //Serial.println(p1_has_won);
    //Serial.println(p1_score);
    //Serial.println(p1_redlives);
    //Serial.println("-----------");
    //Serial.println(p2_has_won);
    //Serial.println(p2_score);
    //Serial.println(p2_redlives);
  }

  displayLives(p1_redlives, p2_redlives);

  if(!displayWin(p1_has_won, p2_has_won)) {
    displayScore(p1_score, p2_score);
  }
}
