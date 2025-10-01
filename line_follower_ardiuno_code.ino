#include <BeelineSensorPro.h>

#define M1 4
#define M1pwm 5
#define M2 7
#define M2pwm 6

BeelineSensorPro sensor = 
BeelineSensorPro((unsigned char[]) {
    A0, A1, A2, A3, A4, A5, A6, A7
}, LINE_BLACK);

void mdrive(int m1, int m2); // Function declaration

void setup() {
    pinMode(M1, OUTPUT);
    pinMode(M1pwm, OUTPUT);
    pinMode(M2, OUTPUT);
    pinMode(M2pwm, OUTPUT);
    Serial.begin(115200);
    delay(2000);
    for (int i=0;i<350;i++){
        sensor.calibrate();
        mdrive(120,-120);
    }
    mdrive(0,0);
    delay(2000);
}

float kP=0.18;
float kD=1.4;
int last_value;

void loop() {
    int err=sensor.readSensor();
    for(int i=0;i<8;i++){
        Serial.print(sensor.values[i]);
        Serial.print('\t');
    }
    Serial.println(err);
    int m1=255;
    int m2=255;

    int diff=err*kP + (err - last_value)*kD;

    last_value=err;
    mdrive(m1+diff,m2-diff);
}

void mdrive(int m1, int m2) {
    // Motor 1 control
    if (m1 > 0) {
        if (m1 > 255) {
            m1 = 255;
        }
        // Forward
        digitalWrite(M1, HIGH);
        analogWrite(M1pwm, 255 - m1);
    } else {
        if (m1 < -255) {
            m1 = -255;
        }
        // Backward
        digitalWrite(M1, LOW);
        analogWrite(M1pwm, -m1);
    }

    // Motor 2 control
    if (m2 > 0) {
        if (m2 > 255) {
            m2 = 255;
        }
        // Forward
        digitalWrite(M2, HIGH);
        analogWrite(M2pwm, 255 - m2);
    } else {
        if (m2 < -255) {
            m2 = -255;
        }
        // Backward
        digitalWrite(M2, LOW);
        analogWrite(M2pwm, -m2);
    }
}