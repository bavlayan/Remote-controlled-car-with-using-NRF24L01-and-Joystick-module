#include <nRF24L01p.h>
#include <SPI.h>

/**
 * @brief Transmitter CSN and CE pin numbers in Arduino
 */
#define CSN  7
#define CE   8

/**
 * @brief Move commands for sending to receiver
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


/**
 * @brief Joystick analog pin numbers
 * +X Value = 1023
 * -X Value = 0
 * +Y Value = 1023 
 * -Y Value = 0
 */
const int POSITION_X_PIN = A1;  
const int POSITION_Y_PIN = A0;
 
nRF24L01p transmitter(CSN, CE);

int Position_X_Value = 0;
int Position_Y_Value = 0;
String move_command;

void setup() {     
    Serial.begin(9600);
    initialize_transmitter();
}

void loop() {
    get_command_by_joystick();
    send_move_command();
}


void get_command_by_joystick() {  
    Position_X_Value = analogRead(POSITION_X_PIN);
    LOG_MESSAGE("X:" + String(Position_X_Value));
    
    Position_Y_Value = analogRead(POSITION_Y_PIN);
    LOG_MESSAGE("Y:" + String(Position_Y_Value));

    if(Position_X_Value == 0) {
        move_command = TURN_LEFT;
    }
    else if(Position_X_Value == 1023) {
        move_command = TURN_RIGHT;
    }
    else if(Position_Y_Value == 0) {
        move_command = MOVE_BACK;
    }
    else if(Position_Y_Value == 1023) {
        move_command = MOVE_FORWARD;
    }
    else {
        move_command = "";
    }
    LOG_MESSAGE("Current Command:" + move_command);
}

void send_move_command() {  
    transmitter.txPL(move_command);
    boolean state = transmitter.send(SLOW);
    LOG_MESSAGE("Sending command: " + move_command);
    if(state == true) {
        LOG_MESSAGE("Success");
    } else {
        LOG_MESSAGE("Fail");
    }    
}

void initialize_transmitter() {
    delay(150);
    LOG_MESSAGE("Transmitter bağlantısı başlatıldı.");
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    transmitter.channel(90);
    transmitter.TXaddress("ccc");
    transmitter.init(); 
}

