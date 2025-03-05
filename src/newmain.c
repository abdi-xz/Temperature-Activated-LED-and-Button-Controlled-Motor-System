#include <xc.h>

#define _XTAL_FREQ 8000000 

// Define pin connections
#define PB1 PORTBbits.RB0      
#define PB2 PORTBbits.RB1      
#define PB3 PORTBbits.RB2     
#define MOTOR_DIR PORTDbits.RD0 
#define MOTOR_DIR_2 PORTDbits.RD1 
#define MOTOR_PWM PORTCbits.RC2
#define LED_YELLOW PORTDbits.RD2
#define LED_GREEN PORTDbits.RD3
#define LED_RED PORTDbits.RD4


void init_config();
unsigned int read_adc(unsigned char channel);
void set_motor_speed(unsigned char duty_cycle);
void delay_ms(unsigned int milliseconds);

void main(void) {
    unsigned int temp_adc;
    float temperature;

    init_config(); 

    while (1) {

        if (PB1) {
            MOTOR_DIR = 1;       
            set_motor_speed(30);  
        } else if (PB2) {
            MOTOR_DIR = 1;        
            set_motor_speed(80);  
        } else if (PB3) {
            MOTOR_DIR = 0;       
            MOTOR_DIR_2 = 1;
            set_motor_speed(30); 
        } else {
            set_motor_speed(0);  
        }

        // Temperature Monitoring
        temp_adc = read_adc(0);
        temperature = (float)((temp_adc * 5.0 / 1023.0) * 100.0); // Convert ADC to °C

        if (temperature < 15) {
            LED_YELLOW = 1; 
            LED_GREEN = 0;
            LED_RED = 0;
        } else if (temperature <= 35) {
            LED_YELLOW = 0;
            LED_GREEN = 1; 
            LED_RED = 0;
        } else {
            LED_YELLOW = 0;
            LED_GREEN = 0;
            LED_RED = 1;  
        }

        delay_ms(100); 
    }
}

// Initialization function
void init_config() {
    TRISB = 0x07;  
    TRISD = 0x00;  
    TRISC = 0x00;  
    ADCON1 = 0x80; 
    ADCON0 = 0x01; 

    // PWM setup
    PR2 = 0xFF;             
    CCP1CON = 0x0C;         
    T2CON = 0x04;          
    CCPR1L = 0x00;         
}

// ADC read function
unsigned int read_adc(unsigned char channel) {
    ADCON0 = (channel << 3) | 0x01; 
    __delay_ms(30);                
    GO_nDONE = 1;                  
    while (GO_nDONE);               
    return ((ADRESH << 8) | ADRESL); 
}

// Set motor speed using PWM
void set_motor_speed(unsigned char duty_cycle) {
    CCPR1L = (duty_cycle * 255) / 100; 
}

// Millisecond delay function
void delay_ms(unsigned int milliseconds) {
    while (milliseconds--) {
        __delay_ms(1); 
    }
}
