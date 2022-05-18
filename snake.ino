#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// pin 9  - Serial clock out (SCLK)
// pin 10 - Serial data out (DIN)
// pin 11 - Data/Command select (D/C)
// pin 13 - LCD chip select (CS)
// pin 12 - LCD reset (RST)

Adafruit_PCD8544 display = Adafruit_PCD8544(9, 10, 11, 13, 12);

#define LEFT 5    //D5
#define DOWN 4    //D4
#define RIGHT 3   //D3
#define UP 2      //D2
#define PAUSE 6   //D6
#define MAX_WIDTH 84 //display 84x48
#define MAX_HEIGHT 48
#define speakerPin A5

int menuitem = 1;
int page = 1;

boolean didMyOneTimeAction = false;

boolean dl = false, dr = false, du = false, dd = false; // Kiểm tra hướng con rắn đang bò

int x[200], y[200], i, slength, tempx = 10, tempy = 10, xx, yy;
unsigned int high;
uint8_t bh, bl;
int xegg, yegg;
int freq, tb;
int l, r, u, d, p;
unsigned long time = 280, beeptime = 50;
int score = 0, flag = 0;

void setup()
{

  display.begin();
  delay(3500);
  display.clearDisplay();

  pinMode(LEFT, INPUT_PULLUP); // Khai báo nút bấm
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(PAUSE, INPUT_PULLUP);
  pinMode(8, OUTPUT);

  digitalWrite(8, LOW); //Turn Backlight ON

}

//Movement Begins after here
void loop()
{
  checkButtonMenu();
  drawMenu();

}

void checkButton() {
  l = digitalRead(LEFT); //Kiểm tra tín hiệu nút bấm
  d = digitalRead(DOWN);
  r = digitalRead(RIGHT);
  u = digitalRead(UP);
  p = digitalRead(PAUSE);
}

void movesnake()
{
  checkButton();
  if (flag == 0)
  {
    direct(); //When key is pressed,this will change the coordinates accordingly and set flag to 1
    //flag would be set to 1 so that direction is not changed multiple times in a short duration
  }

  if (millis() % time < 5) //this condition becomes true after every 'time' milliseconds...millis() returns the time since launch of program
  {

    if (flag == 0) //flag 0 means no directional key has been pressed in the last 'time' milliseconds
    {
      if (dr == true)
      {
        tempx = x[0] + 3;
        tempy = y[0];
      } // so the snake moves one step in the direction it is moving currently
      if (dl == true)
      {
        tempx = x[0] - 3;
        tempy = y[0];
      } //The new coordinates of head of snake goes in tempx,tempy
      if (du == true)
      {
        tempy = y[0] - 3;
        tempx = x[0];
      }
      if (dd == true)
      {
        tempy = y[0] + 3;
        tempx = x[0];
      }
    }

    flag = 0;
    checkgame(); //Check if snake has met egg or coincided with itself
    checkegg();

    if (tempx <= 0)
    {
      tempx = 84 + tempx;
    } //If the new coordinates are out of screen, set them accordingly
    if (tempx >= 84)
    {
      tempx = tempx - 84;
    }
    if (tempy <= 0)
    {
      tempy = 48 + tempy;
    }
    if (tempy >= 48)
    {
      tempy = tempy - 48;
    }

    for (i = 0; i <= slength; i++) //Change the coordinates of all points of snake
    {
      xx = x[i];
      yy = y[i];
      x[i] = tempx;
      y[i] = tempy;
      tempx = xx;
      tempy = yy;
    }

    drawsnake(); //Draw the snake and egg at the new coordinates
  }
}

void checkgame() //Kiểm tra game thua
{
  for (i = 1; i < slength; i++) //Kiểm tra tọa độ của đầu rắn có bằng tọa độ của các điểm của thân rắn hay không!? Có = thua
  {
    if (x[i] == x[0] && y[i] == y[0])
    {

      bh = EEPROM.read(1);
      bl = EEPROM.read(0);
      high = (((0xff00 + bh) << 8) + bl);

      if (score > high)
      {
        high = score;
        bh = (high >> 8);
        bl = high & 0xff;

        EEPROM.write(1, bh);
        EEPROM.write(0, bl);
      }

      display.clearDisplay();
      display.setTextColor(BLACK);
      display.setTextSize(1);
      display.setCursor(20, 12);
      display.print("Game Over");
      display.setCursor(15, 30);
      display.print("Score: ");
      display.print(score);
      display.setCursor(15, 40);
      display.print("High: ");
      display.print(high);

      display.display();

      display.clearDisplay();

      slength = 8; //Resetting the values
      score = 0;
      time = 280;

      redraw(); //Restart game by drawing snake with the resetted length and score
    }
  }
}

void checkegg() //Nếu rắn ăn trứng
{
  if (x[0] == xegg or x[0] == (xegg + 1) or x[0] == (xegg + 2) or x[0] == (xegg - 1)) //Snake in close vicinity of egg
  {
    if (y[0] == yegg or y[0] == (yegg + 1) or y[0] == (yegg + 2) or y[0] == (yegg - 1))
    {
      score += 1; //Tăng độ dài và điểm + Tăng tốc độ di chuyển bằng cách giảm biến 'time'
      slength += 1;
      if (time >= 90)
      {
        time -= 20;
      }

      display.fillRect(xegg, yegg, 3, 3, WHITE); //Xóa trứng rắn vừa ăn
      display.display();
      xegg = random(1, 80); //Tạo trứng mới ở điểm ngẫu nhiên
      yegg = random(1, 40);
    }
  }
}

void direct() //Kiểm tra nếu người chơi nhấn nút và thay đổi hướng đi của rắn
{
  if (l == LOW and dr == false) //Nếu nút LEFT được nhấn, L sẽ trở thành LOW
  {
    dl = true;
    du = false;
    dd = false;
    tempx = x[0] - 3; //Save the new coordinates of head in tempx,tempy
    tempy = y[0];
    flag = 1; //Do not change direction any further for the ongoing 'time' milliseconds
  }
  else if (r == LOW and dl == false)
  {
    dr = true;
    du = false;
    dd = false;
    tempx = x[0] + 3;
    tempy = y[0];
    flag = 1;
  }
  else if (u == LOW and dd == false)
  {
    du = true;
    dl = false;
    dr = false;
    tempy = y[0] - 3;
    tempx = x[0];
    flag = 1;
  }
  else if (d == LOW and du == false)
  {
    dd = true;
    dl = false;
    dr = false;
    tempy = y[0] + 3;
    tempx = x[0];
    flag = 1;
  }
  else if (p == LOW) //Dừng game khoảng 5 giây
  {
    display.clearDisplay();
    display.setTextColor(BLACK);

    for (i = 5; i > 0; i--)
    {
      display.setCursor(25, 10);
      display.setTextSize(1);
      display.print("PAUSED");
      display.setCursor(40, 30);
      display.print(i);
      display.display();
      delay(1000);
      display.clearDisplay();
    }
    redraw(); //Redraw the snake and egg at the same position as it was
  }
}

void drawsnake() //Draw snake and egg at newly changed positions
{
  display.fillRect(xegg, yegg, 3, 3, BLACK); //Vẽ trứng ở điểm mới

  display.drawCircle(x[0], y[0], 1, BLACK);             //Vẽ đầu mới của rắn
  display.drawCircle(x[slength], y[slength], 1, WHITE); //Xóa đuôi cũ của rắn

  display.display();
}

void redraw() //Vẽ lại tất cả điểm của rắn và trứng
{
  display.fillRect(xegg, yegg, 3, 3, BLACK);
  for (i = 0; i < slength; i++)
  {
    display.drawCircle(x[i], y[i], 1, BLACK);
  }
  display.display();
}

// Hiện menu
void drawMenu() {

  if (page == 1) {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("MAIN MENU");
    display.drawFastHLine(0, 10, 83, BLACK);

    display.setCursor(0, 15);
    if (menuitem == 1)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Play Snake");  //Hiện nút chơi rắn

    display.setCursor(0, 25);
    if (menuitem == 2)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Play CTF ");  //Hiện nút


    display.setCursor(0, 35);
    if (menuitem == 3)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">About us");  //Hiện nút Thông tin
    display.display();
  }

  else if (page == 2) {

    // Vẽ tiếp menu trang 2
    // Menu 2 code
  }

  else if (page == 3) {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("INFOMATION");
    display.drawFastHLine(0, 10, 83, BLACK);

    display.setCursor(0, 15);
    if (menuitem == 1)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print("- 70DCTT22");  //Hiện nút chơi rắn

    display.setCursor(0, 25);
    if (menuitem == 2)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print("- Group 11 ");  //Hiện nút


    display.setCursor(0, 35);
    if (menuitem == 3)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print("- Play Animation");  //Hiện nút Thông tin
    display.display();
  }

  else if (page == 4) {
    playSnake();
    // Vẽ tiếp menu trang 4
    // Menu 4 code
  }
}

void playSnake() {

  if (didMyOneTimeAction == false) {
    slength = 8; //Bắt đầu với độ dài của rắn = 8

    xegg = (display.width()) / 2;

    yegg = (display.height()) / 2;

    for (i = 0; i <= slength; i++) //Tọa độ bắt đầu của rắn
    {
      x[i] = 25 - 3 * i;
      y[i] = 10;
    }
    for (i = 0; i < slength; i++) //Vẽ rắn
    {
      display.drawCircle(x[i], y[i], 1, BLACK);
    }
    display.display();
    dr = true; //Going to move right initially
    didMyOneTimeAction = true;
  }
  movesnake();
}

void checkButtonMenu() {
  checkButton();
  if (u == LOW && page == 1) {
    u = HIGH;
    delay(100);
    menuitem--;
    if (menuitem == 0)
    {
      menuitem = 3;
    }
  }
  if (u == LOW && page == 3) {
    u = HIGH;
    delay(100);
    menuitem--;
    if (menuitem == 0)
    {
      menuitem = 3;
    }
  }

  if (d == LOW && page == 1) {
    d = HIGH;
    delay(100);
    menuitem++;
    if (menuitem == 4)
    {
      menuitem = 1;
    }
  }
  if (d == LOW && page == 3) {
    d = HIGH;
    delay(100);
    menuitem++;
    if (menuitem == 4)
    {
      menuitem = 1;
    }
  }

  if (r == LOW) {
    r = HIGH;
    delay(100);

    if (page == 1 && menuitem == 1) {
      display.clearDisplay();
      page = 4;

    }
    if (page == 1 && menuitem == 2) {
      //Code menuitem 2
    }
    else if (page == 1 && menuitem == 3) {
      page = 3;
      menuitem = 1;
      //About us
    }
    else if (page == 3 && menuitem == 3) {
      display.clearDisplay();
      countdown(5);
      text();
      flappybird();
      thankyou();
      page = 1;
    }
  }

  if (l == LOW) {
    l = HIGH;
    delay(100);
    if (page == 2) {
      page = 1;
    }
    if (page == 3) {
      page = 1;
    }
  }
}
