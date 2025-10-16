// SPDX-License-Identifier: Apache-2.0
// Copyright 2021 Ricardo Quesada
// http://retro.moe/unijoysticle2




#include "sdkconfig.h"
#include <Arduino.h>
#include <Bluepad32.h>
#include <uni.h>
#include "controller_callbacks.h"




#define IN1 0
#define IN2 4
#define IN3 2
#define IN4 15




extern ControllerPtr myControllers[BP32_MAX_GAMEPADS];




void foo(ControllerPtr myController) {
    while(1) {
        BP32.update();
        if(myController->x()) { //FORWARD
           
            digitalWrite(IN1, HIGH); // turn on motor
            digitalWrite(IN4, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN2, LOW);
           


        }




        else if(myController->b()) { // BACK
            digitalWrite(IN1, LOW); // turn on motor
            digitalWrite(IN4, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN2, HIGH);
        }




        else if(myController->a()) { // RIGHT
            digitalWrite(IN1, LOW); // turn on motor
            digitalWrite(IN4, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN2, HIGH);
        }




        else if(myController->y()) {
            digitalWrite(IN1, HIGH); // turn on motor
            digitalWrite(IN4, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN2, LOW);
        }




        else {
            digitalWrite(IN1, LOW); // turn on motor
            digitalWrite(IN4, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN2, LOW);
            Console.printf("Press button A!"); // Replace with whatever you want
        }
    }
}




void dumpGamepad(ControllerPtr ctl) {
    Console.printf(
        "DPAD: %d A: %d B: %d X: %d Y: %d LX: %d LY: %d RX: %d RY: %d L1: %d R1: %d L2: %d R2: %d\n",
        ctl->dpad(),         // D-pad
        ctl->a(),           // buttons
        ctl->b(),
        ctl->x(),
        ctl->y(),
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->l1(),
        ctl->r1(),
        ctl->l2(),
        ctl->r2()
    );
}




void setup() {
    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.forgetBluetoothKeys();
    esp_log_level_set("gpio", ESP_LOG_ERROR); // suppress info log spam from gpio_isr_service
    uni_bt_allowlist_set_enabled(true);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
   
   
}




void loop() {
    vTaskDelay(1); // ensures WDT does not get triggered when no controller is connected
    BP32.update();
    for (auto myController : myControllers) { // only execute code when controller is connected
        if (myController && myController->isConnected() && myController->hasData()) {        
            foo(myController);
           
            dumpGamepad(myController); // prints the gamepad state, delete or comment if don't need
        }
    }
}
