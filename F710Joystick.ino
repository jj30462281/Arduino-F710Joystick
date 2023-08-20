#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

#include "F710DataParser.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <SPI.h>

USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
JoystickEvents JoyEvents;
JoystickReportParser Joy(&JoyEvents);

volatile byte cache = 0x00;  //暫存資料

volatile byte F710Slider = 0x08;  // 遙控資料
byte F710SliderLeft = 0x00;       // 方向鍵
byte F710SliderRight = 0x00;      // 右側按鈕
byte F710SliderChanges = 0x00;

volatile byte F710Buttons = 0x00;  // 上、前側按鈕
byte F710Changes = 0x00;

void setup() {
  Serial.begin(9600);

#if !defined(__MIPSEL__)
  while (!Serial)
    ;  // 等待Serial啟用 (Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection)
#endif

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay(200);
  if (!Hid.SetReportParser(0, &Joy))
    ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1);
  Serial.println("Start!");
}

void loop() {
  Usb.Task();

  //方向鍵
  F710SliderLeft = F710Slider & 0x0F;
  if (F710SliderLeft == 0x00)  // 上鍵
  {
    Serial.println("UP");
  } else if (F710SliderLeft == 0x01)  // 上 + 右
  {
    Serial.println("UP + RIGHT");
  } else if (F710SliderLeft == 0x02)  // 右
  {
    Serial.println("RIGHT");
  } else if (F710SliderLeft == 0x03)  // 下 + 右
  {
    Serial.println("DOWN + RIGHT");
  } else if (F710SliderLeft == 0x04)  // 下
  {
    Serial.println("DOWN");
  } else if (F710SliderLeft == 0x05)  // 下 + 左
  {
    Serial.println("DOWN + LEFT");
  } else if (F710SliderLeft == 0x06)  // 左
  {
    Serial.println("LEFT");
  } else if (F710SliderLeft == 0x07)  // 上 + 左
  {
    Serial.println("UP + LEFT");
  } else if (F710SliderLeft == 0x08)  //無按鈕
  {
  }

  // A(綠) B(紅) X(藍) Y(黃) 按鈕
  F710SliderChanges = F710Slider ^ F710SliderRight;
  F710SliderRight = F710Slider & 0xF0;
  if (F710SliderRight & 0x10)  // X(藍)
  {

    if (F710SliderChanges & 0x10)  //當按鈕被點擊(Click)
    {
      Serial.println("Blue Click!");
    }
  }

  if (F710SliderRight & 0x20)  // A(綠)
  {
    Serial.println("Green Checked!");
    if (F710SliderChanges & 0x20)  //當按鈕被點擊(Click)
    {
      Serial.println("Green Click!");
    }
  }

  if (F710SliderRight & 0x40)  // B(紅)
  {
    if (F710SliderChanges & 0x40)  //當按鈕被點擊(Click)
    {
      Serial.println("Red Click!");
    }
  }

  if (F710SliderRight & 0x80)  // Y(黃)
  {

    if (F710SliderChanges & 0x80)  //當按鈕被點擊(Click)
    {
      Serial.println("Yellow Click!");
    }
  }

  //LB RB LT RT BK ST JL JR 鍵
  if (F710Buttons & 0x01)  // LB
  {

    if (F710Changes & 0x01)  //當按鈕被點擊(Click)
    {
      Serial.println("LB Click!");
    }
  }

  if (F710Buttons & 0x02)  // RB
  {

    if (F710Changes & 0x02)  //當按鈕被點擊(Click)
    {
      Serial.println("RB Click!");
    }
  }

  if (F710Buttons & 0x04)  // LT
  {

    if (F710Changes & 0x04)  //當按鈕被點擊(Click)
    {
      Serial.println("LT Click!");
    }
  }

  if (F710Buttons & 0x08)  // RT
  {

    if (F710Changes & 0x08)  //當按鈕被點擊(Click)
    {
      Serial.println("RT Click");
    }
  }

  if (F710Buttons & 0x10)  // BK
  {

    if (F710Changes & 0x10)  //當按鈕被點擊(Click)
    {
      Serial.println("BK Click!");
    }
  }

  if (F710Buttons & 0x20)  // ST
  {

    if (F710Changes & 0x20)  //當按鈕被點擊(Click)
    {
      Serial.println("ST Click!");
    }
  }

  if (F710Buttons & 0x40)  // JL
  {

    if (F710Changes & 0x40)  //當按鈕被點擊(Click)
    {
      Serial.println("JL Click");
    }
  }

  if (F710Buttons & 0x80)  // JR
  {

    if (F710Changes & 0x80)  //當按鈕被點擊(Click)
    {
      Serial.println("JR Click!");
    }
  }

  F710Changes = 0x00;
}

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt) {
  Serial.print("ID: ");
  PrintHex<uint8_t>(evt->ID, 0x80);

  Serial.print(" Axis Left X: ");
  PrintHex<uint8_t>(evt->AxisL.X, 0x80);

  Serial.print(" Axis Left Y: ");
  PrintHex<uint8_t>(evt->AxisL.Y, 0x80);

  Serial.print(" Axis Right X: ");
  PrintHex<uint8_t>(evt->AxisR.X, 0x80);

  Serial.print(" Axis Right Y: ");
  PrintHex<uint8_t>(evt->AxisR.Y, 0x80);

  Serial.print(" Slider: ");
  F710Slider = evt->Slider;
  PrintHex<uint8_t>(F710Slider, 0x80);

  Serial.print(" Buttons: ");
  cache = evt->Buttons;
  F710Changes = cache ^ F710Buttons;
  F710Buttons = cache;
  PrintHex<uint8_t>(F710Buttons, 0x80);

  Serial.println("");
}
