/**
 * @name     YeniÇeri
 * @author   Batuhan AVLAYAN <iletisim@batuhanavlayan.com>
 * @version  V1.0
 */
 
#include <nRF24L01p.h>
#include <SPI.h>

/**
 * @brief Receiver pin numbers in Arduino
 */
#define CSN  7
#define CE   8

/**
 * @brief Motors pin numbers in Arduino
 */
#define ENABLE_LEFT_MOTOR_PIN   3
#define LEFT_MOTOR_IN1          2
#define LEFT_MOTOR_IN2          4
#define ENABLE_RIGHT_MOTOR_PIN  10
#define RIGHT_MOTOR_IN1         6
#define RIGHT_MOTOR_IN2         5

/**
 * @brief Move commands
 */
#define MOVE_FORWARD  "m"
#define MOVE_BACK     "b"
#define TURN_RIGHT    "r"
#define TURN_LEFT     "l"

/**
 * @brief Macro to print log message to serial monitor
 */
#define LOG_MESSAGE(X)         \
         Serial.println(X);    \

nRF24L01p receiver(CSN, CE);
String transmitter_message;

void setup() {
    Serial.begin(9600);
    initialize_receiver();
    initialize_motors_pin();
}

void loop() {
    get_message_by_tranmitter();
    start_car();
}

void start_car() {
    if(transmitter_message == MOVE_FORWARD) {
        start_motor();
        move_forward();
        LOG_MESSAGE("Move to forward");       
    }
    else if(transmitter_message == MOVE_BACK) {
        start_motor();
        move_back();
        LOG_MESSAGE("Move to back");       
    }
    else if(transmitter_message == TURN_RIGHT) {
        start_motor();
        turn_right();
        LOG_MESSAGE("turn to right");   
    }
    else if(transmitter_message == TURN_LEFT) {
        start_motor();
        turn_left();       
        LOG_MESSAGE("turn to left");
    }
    else {
        LOG_MESSAGE("Unrecognized character");
        stop_car();
    }
    transmitter_message = "";  
}

void initialize_receiver() {
    delay(150);
    LOG_MESSAGE("Receiver has been started.");
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    receiver.channel(90);
    receiver.RXaddress("bavlayan");
    receiver.init();       
}

void get_message_by_tranmitter() {
    if(receiver.available()) {
        receiver.read();
        receiver.rxPL(transmitter_message);
        if(transmitter_message.length() != 0) {
            LOG_MESSAGE(transmitter_message);      
        }
    } else {
        LOG_MESSAGE("Receiver isn't available");
    }
}

void initialize_motors_pin() {
    LOG_MESSAGE("Motor Pin has been initialized"); 
    
    //--Rigt motor initialize pins--
    pinMode(ENABLE_RIGHT_MOTOR_PIN, OUTPUT);
    digitalWrite(ENABLE_RIGHT_MOTOR_PIN, HIGH);
    pinMode(RIGHT_MOTOR_IN1, OUTPUT);
    pinMode(RIGHT_MOTOR_IN2, OUTPUT);

    //--Left motor initialize pins--
    pinMode(ENABLE_LEFT_MOTOR_PIN, OUTPUT);
    digitalWrite(ENABLE_LEFT_MOTOR_PIN, HIGH);
    pinMode(LEFT_MOTOR_IN1, OUTPUT);
    pinMode(LEFT_MOTOR_IN2, OUTPUT);
}

void move_forward() {
    LOG_MESSAGE("The car move to forward");
    digitalWrite(RIGHT_MOTOR_IN1, HIGH);
    digitalWrite(RIGHT_MOTOR_IN2, LOW);
    digitalWrite(LEFT_MOTOR_IN1,  HIGH);
    digitalWrite(LEFT_MOTOR_IN2,  LOW);   
}

void move_back() {
    LOG_MESSAGE("The car move to back");
    digitalWrite(RIGHT_MOTOR_IN1, LOW);
    digitalWrite(RIGHT_MOTOR_IN2, HIGH);
    digitalWrite(LEFT_MOTOR_IN2,  HIGH); 
    digitalWrite(LEFT_MOTOR_IN1,  LOW);
}

void turn_left() {
    LOG_MESSAGE("The car turn to right");
    digitalWrite(RIGHT_MOTOR_IN1, HIGH);
    digitalWrite(RIGHT_MOTOR_IN2, LOW);
    digitalWrite(LEFT_MOTOR_IN1,  LOW);
    digitalWrite(LEFT_MOTOR_IN2,  HIGH); 
}

void turn_right() {
    LOG_MESSAGE("The car move to right");
    digitalWrite(RIGHT_MOTOR_IN1, LOW);
    digitalWrite(RIGHT_MOTOR_IN2, HIGH);
    digitalWrite(LEFT_MOTOR_IN1,  HIGH);
    digitalWrite(LEFT_MOTOR_IN2,  LOW);   
}

void stop_car() {
   digitalWrite(ENABLE_RIGHT_MOTOR_PIN, LOW);
   digitalWrite(ENABLE_LEFT_MOTOR_PIN,  LOW);
}

void start_motor() {
   digitalWrite(ENABLE_RIGHT_MOTOR_PIN, HIGH);
   digitalWrite(ENABLE_LEFT_MOTOR_PIN,  HIGH);  
}

