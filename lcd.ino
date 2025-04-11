// Pin variables
int en = 11;
int rs = 12;
int d4 = 2;
int d5 = 3;
int d6 = 4;
int d7 = 5;
int buttonPin = 7;
bool buttonState = true;

bool state = false;


// LCD state variables
bool showCursor = false;
bool blinkCursor = false;
bool showDisplay = true;


// Low level functions
void writeData(bool a0 = 0, bool a1 = 0, bool a2 = 0, bool a3 = 0) {
  digitalWrite(d4, a0);
  digitalWrite(d5, a1);
  digitalWrite(d6, a2);
  digitalWrite(d7, a3);
}

void flashEN() {
  delay(3);
  digitalWrite(en, HIGH);
  delay(3);
  digitalWrite(en, LOW);
}

void clear() {
  digitalWrite(rs, LOW);
  writeData(0, 0, 0, 0);
  flashEN();

  writeData(1, 0, 0, 0);
  flashEN();
}

void resetCursor() {
  digitalWrite(rs, LOW);
  writeData(0,0,0,0); // 0 0 0 0
  flashEN();

  writeData(0,1,0,0); // x 1 0 0
  flashEN();
}

// cursor move + display scroll.         
void entryModeSet(bool left = false, bool shift = false) {
  writeData(0,0,0,0);
  flashEN();

  writeData(shift,left,1,0); // Cursor moving r1/l0, shift display y1/n0
  flashEN();
}

void applyDisplaySettings() {
  digitalWrite(rs, LOW);
  writeData(0,0,0,0); // 0 0 0 0
  flashEN();
 
  writeData(blinkCursor,showCursor,showDisplay,1); // B C D 1
  flashEN();
}

void enableCursor(bool show = true, bool blink = false) {
  showCursor = show;
  blinkCursor = blink;
  applyDisplaySettings();
}

void enableDisplay(bool var) {
  showDisplay = var;
  applyDisplaySettings();
}

void moveCursor(bool right = true) {
  digitalWrite(rs, LOW);
  writeData(1,0,0,0); // 0 0 0 1
  flashEN();

  writeData(0,0,right,0); // x x R1/L0 S1/C0
  flashEN();
}

void moveCursor(int row, int col) {
  digitalWrite(rs, LOW);
  resetCursor();
  for (int i = 0; i < row; i++) {
    moveCursor();
  }
  for (int i = 0; i < col; i++) {
    for (int ii = 0; ii < 40; ii++) {
      moveCursor();
    }
  }
}

void moveText(bool right = true) {
  digitalWrite(rs, LOW);
  writeData(1,0,0,0); // 0 0 0 1
  flashEN();

  writeData(0,0,right,1); // x x R1/L0 S1/C0
  flashEN();
}

void configureDisplaySettings(bool eightBit = false, bool multiline = true, bool largeFont = false) {
  digitalWrite(rs, LOW);
  writeData(eightBit,1,0,0); // DL 1 0 0
  flashEN();

  writeData(0, 0, largeFont, multiline); // x x F N
  flashEN();
}

// Every 8th/10th adresses is a new character
void setCGRAMAdress(int adr) {
  bool binary[6];
  if (adr >= 64) {
    return;
  }
  for (int i = 6; i >= 0; i--) {
    int k = adr >> i;
    binary[i] = k & 1;
  }

  digitalWrite(rs, LOW);
  writeData(binary[4], binary[5], 1, 0);
  flashEN();

  writeData(binary[0], binary[1], binary[2], binary[3]);
  flashEN();
}

void setDDRAMAdress(int adr) {
  bool binary[7];
  if (adr >= 128) {
    return;
  }
  for (int i = 7; i >= 0; i--) {
    int k = adr >> i;
    binary[i] = k & 1;
  }

  digitalWrite(rs, LOW);
  writeData(binary[4], binary[5], binary[6], 1);
  flashEN();

  writeData(binary[0], binary[1], binary[2], binary[3]);
  flashEN();
}

// 0 ... 7 -> CGRAM Chars; 8 ... 15 same Chars;
// 16 ... 32 -> empty
void sendChar(int var) {
  bool binary[8] = {0,0,0,0,0,0,0,0};
  if (var >= 256) {
    return;
  }
  for (int i = 8; i >= 0; i--) {
    int k = var >> i;
    binary[i] = k & 1;
  }
  digitalWrite(rs, HIGH);
  writeData(binary[4], binary[5], binary[6], binary[8]);
  flashEN();

  writeData(binary[0], binary[1], binary[2], binary[3]);
  flashEN();
}

void print(char strg[]) {
  int len = strlen(strg);
  // Serial.print("Whole len: ");
  // Serial.print(len);
  // Serial.print(", strg: ");
  // Serial.print(strg);
  // Serial.println();
  for (int i = 0; i < len; i++) {
    // Serial.print(", i: ");
    // Serial.print(i);
    // Serial.print(", strg[i]: ");
    // Serial.print(strg[i]);
    // Serial.print(", strg_before: ");
    // Serial.print(strg);
    sendChar(strg[i]);
    // Serial.print(", strg_after: ");
    // Serial.print(strg);
  }
  // Serial.println();
  delay(10);
}




// init
void LCDInit() {
  digitalWrite(rs, 0);
  delay(15);
  writeData(1,1,0,0);
  flashEN();
  writeData(1,1,0,0);
  flashEN();
  writeData(1,1,0,0);
  flashEN();
  writeData(0,1,0,0);
  flashEN();

  configureDisplaySettings(false, true, false);
  enableDisplay(false);
  clear();
  entryModeSet(true, false);

}



// --- run ----
void run1() {
  print("*");
}

// --- run2 ----
void run3() {
  print("#");
}




// Setup
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 2; i <= 5; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(rs, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(buttonPin, INPUT);

  pinMode(13, OUTPUT);

  Serial.println();
  Serial.println("--start");
  
  LCDInit();

  enableDisplay(true);
  enableCursor(true, true);

  moveCursor(5, 1);

  entryModeSet(false, true);
}


// Loop
void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(buttonPin) && !buttonState) {
    state = !state;
    if (state) {
      run1();
    } else {
      run3();
    }
  }
  buttonState = digitalRead(buttonPin);
  delay(30);
}














