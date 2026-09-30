// 設定連接 RGB LED 的 PWM 腳位
const int redPin = 9;
const int greenPin = 11;
const int bluePin = 10;

// 設定按鈕連接 D2 腳位
const int buttonPin = 2;

// 若燈泡是共陽極請設 true；共陰極請設 false
const bool COMMON_ANODE = true;

// 8 個顏色節點 (紅、橙、黃、綠、藍、靛、紫、紅)
const int NUM_COLORS = 8;
const int rainbowColors[NUM_COLORS][3] = {
  {255,   0,   0}, // 1. 紅色
  {255, 100,   0}, // 2. 橘色
  {255, 255,   0}, // 3. 黃色
  {  0, 255,   0}, // 4. 綠色
  {  0,   0, 255}, // 5. 藍色
  { 75,   0, 130}, // 6. 靛色
  {148,   0, 211}, // 7. 紫色
  {255,   0,   0}  // 8. 紅色 (接回起點)
};

bool ledPowerOn = false;        // 預設為關閉狀態 (開機時不亮)
bool lastButtonState = HIGH;   // 紀錄按鈕上一次狀態

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  // 設定 D2 為輸入模式並開啟內建上拉電阻 (沒按為 HIGH，按下去為 LOW)
  pinMode(buttonPin, INPUT_PULLUP);

  // 初始化關閉 LED
  setLEDColor(0, 0, 0);
}

void loop() {
  // 讀取 D2 按鈕狀態
  bool currentButtonState = digitalRead(buttonPin);

  // 偵測按鈕按下瞬間 (從 HIGH 變成 LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    ledPowerOn = !ledPowerOn; // 切換燈光電源 (亮燈 / 關燈)
    delay(50);                // 機械按鈕防彈跳 (Debounce)
  }
  lastButtonState = currentButtonState;

  // 判斷當前電源狀態
  if (ledPowerOn) {
    // 亮燈模式：計算 10 秒彩虹漸層
    const unsigned long duration = 10000;
    unsigned long timeOffset = millis() % duration;

    float progress = ((float)timeOffset / (float)duration) * (NUM_COLORS - 1);
    int index = floor(progress);
    float fraction = progress - index;

    int r = rainbowColors[index][0] + (rainbowColors[index + 1][0] - rainbowColors[index][0]) * fraction;
    int g = rainbowColors[index][1] + (rainbowColors[index + 1][1] - rainbowColors[index][1]) * fraction;
    int b = rainbowColors[index][2] + (rainbowColors[index + 1][2] - rainbowColors[index][2]) * fraction;

    setLEDColor(r, g, b);
  } else {
    // 關燈模式：將 RGB 通道全部關閉
    setLEDColor(0, 0, 0);
  }
}

void setLEDColor(int r, int g, int b) {
  if (COMMON_ANODE) {
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
  }
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}
