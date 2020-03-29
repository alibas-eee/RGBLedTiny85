#include <avr/interrupt.h>

#define LED_SUPPLY_VOLTAGE    12
#define LED_CUT_OFF_VOLTAGE   3
#define LED_PWM_PRECISION     256 //8 BIT
#define LED_MAX_VALUE         LED_PWM_PRECISION - 1
#define LED_CUT_OFF_VALUE     (LED_CUT_OFF_VOLTAGE*LED_MAX_VALUE)/LED_SUPPLY_VOLTAGE
#define LED_PERCENT_TO_VALUE  (LED_MAX_VALUE-LED_CUT_OFF_VALUE) /100

#define LED_ON    (LED_MAX_VALUE - 1)
#define LED_OFF   0

#define PIN_LED_R 0
#define PIN_LED_B 1
#define PIN_BTN   2

#define _TINY85_ 1

enum E_STATE{
  STATE_OFF = 0,
  STATE_ONLY_R = 1,
  STATE_ONLY_B = 2,
  STATE_BOTH_ON = 3,
  STATE_ANIM_SLOW = 4,  
  STATE_ANIM_FAST = 5,
  STATE_ANIM_R = 6,
  STATE_ANIM_B = 7,  
  };

int led_R = 0;
int led_B = 0;
int counter = 0;
E_STATE state = STATE_OFF;

//void interrupt();
void init_ISR();
void state_machine();
void animation();
void interrupt();

#ifdef _TINY85_
  ISR(INT0_vect)
#else
  void interrupt()
#endif

 { 
   switch (state) {
    case STATE_OFF:
    {
      state = STATE_ONLY_R;
      break;
    }
    case STATE_ONLY_R:
    {
      state = STATE_ONLY_B;
      break;
    }
    case STATE_ONLY_B:
    {
      state = STATE_BOTH_ON;
      break;
    }
    case STATE_BOTH_ON:
    {
      state = STATE_ANIM_SLOW;
      break;
    }
    case STATE_ANIM_SLOW:
    {
      state = STATE_ANIM_FAST;
      break;
    }
    case STATE_ANIM_FAST:
    {
      state = STATE_ANIM_R;
      break;
    }   
    case STATE_ANIM_R:
    {
      state = STATE_ANIM_B;
      break;
    }
    case STATE_ANIM_B:
    {
      state = STATE_OFF;
      break;
    }
    default:
    {
      state = STATE_OFF;
      break;
    }
  }//end_switch
}



void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED_R, 1);
  pinMode(PIN_LED_B, 1);
  pinMode(PIN_BTN,   0);
  
#ifndef  _TINY85_
 Serial.begin(9600);
 attachInterrupt(digitalPinToInterrupt(PIN_BTN), interrupt, RISING);
#else
  init_ISR();
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
  state_machine();
  delay(1);
}

#ifdef _TINY85_
  void init_ISR(){
      MCUCR |= 0x3;  // Rising EDGE
      GIMSK |= 1<<6;  // Activate the INT0
      sei(); 
  }
#endif


void state_machine()
{
  switch (state) {
    case STATE_OFF:
    {
      analogWrite(PIN_LED_R, LED_OFF);
      analogWrite(PIN_LED_B, LED_OFF);
      break;
    }
    case STATE_ONLY_R:
    {
      analogWrite(PIN_LED_R, LED_ON);
      analogWrite(PIN_LED_B, LED_OFF);
      break;
    }
    case STATE_ONLY_B:
    {
      analogWrite(PIN_LED_B, LED_ON);
      analogWrite(PIN_LED_R, LED_OFF);
      break;
    }
    case STATE_BOTH_ON:
    {
      analogWrite(PIN_LED_R, LED_ON);
      analogWrite(PIN_LED_B, LED_ON);
      break;
    }
    case STATE_ANIM_SLOW:
    {
      animation2();
      break;
    }
    case STATE_ANIM_FAST:
    {
      animation2();
      break;
    }
    case STATE_ANIM_R:
    {
      animationR();
      break;
    }
    case STATE_ANIM_B:
    {
      animationB();
      break;
    }
    default:
    {
      state = STATE_OFF;
      break;
    }
  }//end_switch
}



void animationR()
{
  delay(100);
  
  counter++;
  if(counter >300){counter = 0;}

  if(counter < 100){led_R = counter;}
  else if(counter >=100 && counter <200 ){led_R = 100 ;}
  else if(counter >=200 && counter <300 ){led_R = (300-counter);}

  if(led_R > 0 )
  {
    analogWrite(PIN_LED_R,   LED_CUT_OFF_VALUE + LED_PERCENT_TO_VALUE * led_R);
  }
   analogWrite(PIN_LED_B, 0);  

}

void animationB()
{
  delay(100);
  
  counter++;
  if(counter >300){counter = 0;}

  if(counter < 100){led_B = counter;}
  else if(counter >=100 && counter <200 ){led_B = 100 ;}
  else if(counter >=200 && counter <300 ){led_B = (300-counter);}

  if(led_R > 0 )
  {
    analogWrite(PIN_LED_B,   LED_CUT_OFF_VALUE + LED_PERCENT_TO_VALUE * led_B);
  }
   analogWrite(PIN_LED_R, 0);  

}


void animation2()
{
 if(state == STATE_ANIM_FAST)
 {
  delay(20);
 }
 if(state == STATE_ANIM_SLOW)
 {
  delay(100);
 }
  
  counter++;
  if(counter >300){counter = 0;}

  if(counter < 100){led_R = counter;}
  else if(counter >=100 && counter <200 ){led_R = (200-counter);}
  else if(counter >=200 && counter <300 ){led_R = 0 ;}

  if(counter < 100){led_B = 0;}
  else if(counter >=100 && counter <200 ){led_B = (counter-100);}
  else if(counter >=200 && counter <300 ){led_B = (300-counter);}


  if(led_R > 0 )
  {
    analogWrite(PIN_LED_R,   LED_CUT_OFF_VALUE + LED_PERCENT_TO_VALUE * led_R);
  }

  if(led_B > 0 )
  {
    analogWrite(PIN_LED_B,   LED_CUT_OFF_VALUE + LED_PERCENT_TO_VALUE * led_B);
  }

}


void animation()
{
 if(state == STATE_ANIM_FAST)
 {
  delay(20);
 }
 if(state == STATE_ANIM_SLOW)
 {
  delay(100);
 }
  
  counter++;
  if(counter >500){counter = 0;}

  if(counter < 100){led_R = counter;}
  else if(counter >=100 && counter <300 ){led_R = 100 ;}
  else if(counter >=300 && counter <400 ){led_R = (400-counter);}
  else if(counter >=400 && counter <500 ){led_R = 0 ;}

  if(counter < 100){led_B = 0;}
  else if(counter >=100 && counter <200 ){led_B = (counter-100);}
  else if(counter >=200 && counter <400 ){led_B = 100;}
  else if(counter >=400 && counter <500 ){led_B = (500-counter);}


  if(led_R > 0 )
  {
    analogWrite(PIN_LED_R,   LED_CUT_OFF_VALUE + LED_PERCENT_TO_VALUE * led_R);
  }

  if(led_B > 0 )
  {
    analogWrite(PIN_LED_B,   LED_CUT_OFF_VALUE + LED_PERCENT_TO_VALUE * led_B);
  }

}
