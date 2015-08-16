#define LED_GND 8
#define LED_R 9
#define LED_G 10
#define LED_B 11

#define RIGHT_POT A0
#define MIDDLE_POT A1
#define LEFT_POT A3
#define POT_GND A2
#define POT_VCC A4

#define BUTTON 6
#define BUTTON_GND 7

void on() {
    unsigned char r, g, b;
    unsigned char h = random(255);
    unsigned char s = 255;
    unsigned char v = 255;
    unsigned char region, fpart, p, q, t;

    if(s == 0) {
        /* color is grayscale */
        r = g = b = v;
        return;
    }

    /* make hue 0-5 */
    region = h / 43;
    /* find remainder part, make it from 0-255 */
    fpart = (h - (region * 43)) * 6;

    /* calculate temp vars, doing integer multiplication */
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * fpart) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;

    /* assign temp vars based on color cone region */
    switch(region) {
        case 0:
            r = v; 
            g = t; 
            b = p; 
            break;
        case 1:
            r = q; 
            g = v; 
            b = p; 
            break;
        case 2:
            r = p; 
            g = v; 
            b = t; 
            break;
        case 3:
            r = p; 
            g = q; 
            b = v; 
            break;
        case 4:
            r = t; 
            g = p; 
            b = v; 
            break;
        default:
            r = v; 
            g = p; 
            b = q; 
            break;
    }

    analogWrite(LED_R, r);
    analogWrite(LED_G, g);
    analogWrite(LED_B, b);
}

void off() {
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 0);
}

unsigned long last_on = 0;
unsigned long off_delay = 42;
unsigned long on_delay = 42;

void reschedule() {
    next_off = last_on + off_delay;
    next_on = last_on + on_delay;
}

void setup() {                
    pinMode(8, OUTPUT);
    digitalWrite(8, LOW); 

    pinMode(A0, INPUT); // rightmost
    pinMode(A1, INPUT); // middle
    pinMode(A2, OUTPUT);
    digitalWrite(A2, LOW);
    pinMode(A3, INPUT);
    pinMode(A4, OUTPUT);
    digitalWrite(A4, HIGH);

    pinMode(LED_R, OUTPUT);
    digitalWrite(LED_R, LOW);
    pinMode(LED_G, OUTPUT);
    digitalWrite(LED_G, LOW);
    pinMode(LED_B, OUTPUT);
    digitalWrite(LED_B, LOW);

    pinMode(TAP_TEMPO_BUTTON, INPUT_PULLUP);

    reschedule();
}

void read_control() {
    static last_
    const float duty = analogRead(DUTY_POT) / 1024.0;
    off_delay = (unsigned long)(delay * duty);
    reschedule();

    static unsigned long last_tap = 0;
    static bool last_tap_tempo = 0;
    bool tap_tempo = digitalRead(TAP_TEMPO_BUTTON);
    if (tap_tempo < last_tap_tempo) {
        unsigned long delay = millis() - last_tap;
        last_tap = millis();
        on_delay = delay;
        off_delay = (unsigned long)(delay * duty);
        reschedule();
    }
    last_tap_tempo = 


}

void loop() {
    unsigned long t = micros();

    if (millis() > next_on) {
        on();
        last_on = millis();
    } else if (millis() > next_off) {
        off();
    } else {
        read_control();
    }

    delay(1);
}
