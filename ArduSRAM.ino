
#define A_0 A0
#define A_1 A1
#define A_2 A2
#define A_3 A3
#define A_4 A4
#define A_5 A5
#define A_6 11
#define A_7 13
#define A_8 12

#define D_0 2
#define D_1 3
#define D_2 4
#define D_3 5
#define D_4 6
#define D_5 7
#define D_6 8
#define D_7 9

#define WR 10

void setup() {
  pinMode(A_0, OUTPUT);
  pinMode(A_1, OUTPUT);
  pinMode(A_2, OUTPUT);
  pinMode(A_3, OUTPUT);
  pinMode(A_4, OUTPUT);
  pinMode(A_5, OUTPUT);
  pinMode(A_6, OUTPUT);
  pinMode(A_7, OUTPUT);
  pinMode(A_8, OUTPUT);
  pinMode(WR, OUTPUT);
  digitalWrite(WR, HIGH);
  Serial.begin(115200);
}

void addr(int addr) {
  digitalWrite(A_0, addr & 1);
  digitalWrite(A_1, addr & 2);
  digitalWrite(A_2, addr & 4);
  digitalWrite(A_3, addr & 8);
  digitalWrite(A_4, addr & 16);
  digitalWrite(A_5, addr & 32);
  digitalWrite(A_6, addr & 64);
  digitalWrite(A_7, addr & 128);
  byte addrH = (addr >> 8);
  // workaround: addr & 256 always returns 0, because somehow addr is coerced to uint8
  digitalWrite(A_8, addrH & 1);
}

void dataIn() {
  pinMode(D_0, INPUT);
  pinMode(D_1, INPUT);
  pinMode(D_2, INPUT);
  pinMode(D_3, INPUT);
  pinMode(D_4, INPUT);
  pinMode(D_5, INPUT);
  pinMode(D_6, INPUT);
  pinMode(D_7, INPUT);
}

void dataOut() {
  pinMode(D_0, OUTPUT);
  pinMode(D_1, OUTPUT);
  pinMode(D_2, OUTPUT);
  pinMode(D_3, OUTPUT);
  pinMode(D_4, OUTPUT);
  pinMode(D_5, OUTPUT);
  pinMode(D_6, OUTPUT);
  pinMode(D_7, OUTPUT);
}

void write(byte data) {
  dataOut();
  digitalWrite(D_0, data & 1);
  digitalWrite(D_1, data & 2);
  digitalWrite(D_2, data & 4);
  digitalWrite(D_3, data & 8);
  digitalWrite(D_4, data & 16);
  digitalWrite(D_5, data & 32);
  digitalWrite(D_6, data & 64);
  digitalWrite(D_7, data & 128);
  digitalWrite(WR, LOW);
  dataIn();
  digitalWrite(WR, HIGH);
}

byte read() {
  byte data = digitalRead(D_0);
  data |= (digitalRead(D_1) << 1);
  data |= (digitalRead(D_2) << 2);
  data |= (digitalRead(D_3) << 3);
  data |= (digitalRead(D_4) << 4);
  data |= (digitalRead(D_5) << 5);
  data |= (digitalRead(D_6) << 6);
  data |= (digitalRead(D_7) << 7);
  return data;
}

int to_test = 512;
int random_seed = 0;

void loop() {
  randomSeed(random_seed);
  for (int i=0; i<to_test; i++) {
    addr(i);
    write(random(0,255));
  }
  
  int okBytes = 0;
  randomSeed(random_seed);
  
  for (int i=0; i<to_test; i++) {
    addr(i);
    byte data = read();
    byte expData = random(0,255);
    if (data == expData) okBytes++;
  }
  
  if (okBytes == to_test) {
    Serial.print(okBytes);
    Serial.println(" bytes OK");
  } else { // fixme: actually store the failures
    for (int i=0; i<to_test; i++) {
      addr(i);
      byte data = read();
      byte expData = random(0,255);
      if (data == expData)    
      {
        Serial.print(".");
      } else {
        Serial.print("!");
      }
    }
    Serial.println("");
  }
  random_seed++;
}
