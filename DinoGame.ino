#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int joyY = A1;
const int greenLED = 8;
const int redLED = 9;

int dinoRow = 0;
int dinoCol = 5;
int obstacleCol = 15;
int obstacleRow = 1;

unsigned long lastMoveTime = 0;
const int moveDelay = 300;
unsigned long lastSpawnTime = 0;
int score = 0;
bool obstacleActive = true;

const int buzzerPin = 10;

bool waitingToStart = true;


// Custom characters
byte dinoChar[8] = {
  B00100,
  B01010,
  B00100,
  B00100,
  B11111,
  B00100,
  B01010,
  B10001
};

byte cactusChar[8] = {
  B00100,
  B00100,
  B10101,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100
};

void setup() {
  lcd.begin(16, 2);
  pinMode(joyY, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);


  lcd.createChar(0, dinoChar);
  lcd.createChar(1, cactusChar);

  randomSeed(analogRead(0));
  resetGame();
}

void resetGame() {
  dinoRow = 1;
  obstacleCol = 15;
  obstacleRow = 1;
  score = 0;
  obstacleActive = true;
  waitingToStart = true;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" 'Dino' Runner!! ");
  lcd.setCursor(0, 1);
  lcd.print("Push UP to play");
}


void loop() {
  if (waitingToStart) {
  int yVal = analogRead(joyY);
  if (yVal < 400) {
    waitingToStart = false;
    lcd.clear(); // start fresh
    lastMoveTime = millis(); // prevent instant move
  } else {
    return; // don't run game until push up
  }
}
  int yVal = analogRead(joyY);
  if (yVal < 400) dinoRow = 0; // jump
  else dinoRow = 1;            // fall back

  if (millis() - lastMoveTime > moveDelay) {
    lastMoveTime = millis();
    lcd.clear();

    // Draw dino
      lcd.setCursor(dinoCol, dinoRow);
      lcd.write(byte(0)); // draw dino

    // Draw cactus if active
    if (obstacleActive) {
      lcd.setCursor(obstacleCol, obstacleRow);
      lcd.write(byte(1));

      // Collision
if (obstacleCol == dinoCol && dinoRow == obstacleRow) {
        digitalWrite(redLED, HIGH);

tone(buzzerPin, 800, 200); delay(200);
tone(buzzerPin, 600, 200); delay(200);
tone(buzzerPin, 400, 400); delay(400);
noTone(buzzerPin);

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("GAME OVER");
lcd.setCursor(0, 1);
lcd.print("Score: ");
lcd.print(score);

delay(2000);
digitalWrite(redLED, LOW);
resetGame();

        return;
      }

      // Passed cactus
      if (obstacleCol == dinoCol && dinoRow != obstacleRow)
 {
        digitalWrite(greenLED, HIGH);
tone(buzzerPin, 500, 100); delay(150);
tone(buzzerPin, 700, 100); delay(150);
tone(buzzerPin, 900, 100); delay(150);
noTone(buzzerPin);

digitalWrite(greenLED, LOW);
score++;

        obstacleActive = false; // Wait before next cactus
        lastSpawnTime = millis();
      }

      obstacleCol--;
    }

    // Spawn new cactus at random interval
    if (!obstacleActive && millis() - lastSpawnTime > random(800, 1800)) {
      obstacleCol = 15;
      obstacleActive = true;
    }

    // Show score
    lcd.setCursor(10, 0);
    lcd.print("S:");
    lcd.print(score);
  }
}
