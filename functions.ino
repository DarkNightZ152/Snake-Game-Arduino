void countdown(int x)
{
  for (int i=x;i>0;i--)
  {
    display.setTextSize(5);         //change font size
    display.setTextColor(BLACK);    //set text color, only BLACK or WHITE is to be used
    display.setCursor(30,6);        //set cursor position
    display.print(i);               //print variable "i"
    display.display();              //display the content (i.e. the value of variable i), 
    delay(1000);
    display.clearDisplay();         //clears display content
  }
}

void text()
{
    display.setTextSize(1);         //change font size
    display.setTextColor(BLACK);    //set text color, only BLACK or WHITE is to be used
    display.setCursor(0,0);         //set cursor position
    display.print("Group 11");
    display.setTextSize(2);
    display.setCursor(12,16);
    display.print("70DCTT22");
    display.display();              //clears display content
    delay(600);
    display.setTextSize(1);
    display.setCursor(36,38);
    display.print("ARDUINO");
    display.display();
    delay(2000);
    display.clearDisplay();
}

void thankyou()
{
    display.fillScreen(BLACK);        //fill entire screen with black pixels
    display.setTextSize(2);           //change font size
    display.setTextColor(WHITE);      //set text color, only BLACK or WHITE is to be used
    display.setCursor(8,10);          //set cursor position
    display.print("Thank   You..");
    display.display();               
    delay(1000);
    display.clearDisplay();           //clears display content
    display.fillScreen(BLACK);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(8,10);
    display.print("For");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(8,22);
    display.print("LISTENING");
    display.display();
    delay(1500);
    display.clearDisplay();
}

void flappybird()
{
  int a1=70,b1=30,a2,b2=20,a3,b3=45;
  int cur=0,counter=0;
  int flappyY=0,ob=0,obInc=0;
  flappyY=b1+10;

  display.setRotation(3);         //rotate display by 270 deg
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,4);
  display.print("FlyThru");
  display.setCursor(3,60);
  display.print("Loading");
  display.display();
  delay(1000);
  
  //main game animation
  for (int m=0;m<200;m++)
  {
    display.drawRect(0,80,48,4,BLACK);        //draw a rectangle
    display.fillRect(0,80,48,4,BLACK);        //color the rectangle

    display.setRotation(3);
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(cur,4);
    display.print("FlyThru");

    //first pillar
    display.drawRect(a1,0,7,b1,BLACK);
    display.fillRect(a1,0,7,b1,BLACK);
    display.drawRect(a1-2,b1,11,4,BLACK);
    display.fillRect(a1-2,b1,11,4,BLACK);

    display.drawRect(a1-2,b1+20,11,4,BLACK);
    display.fillRect(a1-2,b1+20,11,4,BLACK);
    display.drawRect(a1,b1+20,7,64-b1,BLACK);
    display.fillRect(a1,b1+20,7,64-b1,BLACK);
    
    a2=a1+28;
    //second pillar
    display.drawRect(a2,0,7,b2,BLACK);
    display.fillRect(a2,0,7,b2,BLACK);
    display.drawRect(a2-2,b2,11,4,BLACK);
    display.fillRect(a2-2,b2,11,4,BLACK);

    display.drawRect(a2-2,b2+20,11,4,BLACK);
    display.fillRect(a2-2,b2+20,11,4,BLACK);
    display.drawRect(a2,b2+20,7,64-b2,BLACK);
    display.fillRect(a2,b2+20,7,64-b2,BLACK);
    
    a3=a2+28;
    //third pillar
    display.drawRect(a3,0,7,b3,BLACK);
    display.fillRect(a3,0,7,b3,BLACK);
    display.drawRect(a3-2,b3,11,4,BLACK);
    display.fillRect(a3-2,b3,11,4,BLACK);

    display.drawRect(a3-2,b3+20,11,4,BLACK);
    display.fillRect(a3-2,b3+20,11,4,BLACK);
    display.drawRect(a3,b3+20,7,64-b3,BLACK);
    display.fillRect(a3,b3+20,7,64-b3,BLACK);
    
     //game progress counter panel 
     if (cur<-45)
    {
     display.drawRect(35,0,14,9,BLACK);
     display.fillRect(35,0,14,9,BLACK);

     display.setTextSize(1);
     display.setTextColor(WHITE);
     display.setCursor(36,1);
     display.print(counter);
    }

    //bird
    ob=b1+10;
    obInc=(ob-flappyY)/10;
    if (obInc==0)
    {
     if (ob>flappyY) obInc=1;
     if (ob<flappyY) obInc=-1;
    }
    if (obInc==1 || obInc==-1)
    {
     if (ob>flappyY) obInc=2;
     if (ob<flappyY) obInc=-2;
    }
    if (obInc==2 || obInc==-2)
    {
     if (ob>flappyY) obInc=3;
     if (ob<flappyY) obInc=-3;
    }
    if (flappyY!=ob)
    {
      flappyY=flappyY+obInc;
    }
    if (cur<-45)
    {
      display.drawRect(4,flappyY,6,3,BLACK);
      display.drawPixel(10,flappyY+2,BLACK);
      display.drawLine(4,flappyY+3,10,flappyY+3,BLACK);
    }
    
    display.display();
    delay(80);

    a1--;
    if (cur>-60) cur--;
    
    if (a1==-9)
    {
      a1=a2;
      
      b1=b2;
      b2=b3;
      b3=random(20,44);
      counter++;
    }
    display.clearDisplay();
  }
  display.clearDisplay();
  display.setRotation(0);
  
}
