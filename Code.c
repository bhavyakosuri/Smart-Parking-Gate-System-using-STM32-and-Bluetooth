#include "stm32f4xx.h"                   // Device header

// Global Variables
uint32_t countIN = 0;   // Count for objects detected by sensor 0 (PA0)
uint32_t countOUT = 0;  // Count for objects detected by sensor 1 (PA1)
uint32_t adc_value = 0; // Variable to hold ADC value

void GPIO_Config(void);
void Timer4_PWM_Init(void);
void Set_Servo_Angle(uint8_t angle);
void Delay_ms(uint32_t ms);
void ADC_Config(void);
uint32_t ADC_Read(uint8_t channel);
void GPIOC_Init(void);
void GPIOA_Init(void);
void USART2_Init(void);
char USART2_Read(void);
void USART2_Write(char c);
void USART2_SendString(char *str);

int main(void) {
    GPIO_Config();    
    Timer4_PWM_Init();
    ADC_Config(); // Configure ADC
    GPIOC_Init(); // Initialize GPIO for LED
    GPIOA_Init(); // Initialize GPIO for USART2 TX and RX
    USART2_Init(); // Initialize USART2 for Bluetooth

    uint8_t current_angle = 0; // Track current servo angle
    uint8_t sensor0_state = (GPIOA->IDR & GPIO_IDR_ID0) ? 1 : 0; // Initialize to current state of PA0
    uint8_t sensor1_state = (GPIOA->IDR & GPIO_IDR_ID1) ? 1 : 0; // Initialize to current state of PA1
    uint8_t active_sensor = 0; // 0 for PA0, 1 for PA1

    while (1) {
        // Read the state of each sensor
        uint8_t new_sensor0_state = (GPIOA->IDR & GPIO_IDR_ID0) ? 1 : 0; // Read PA0
        uint8_t new_sensor1_state = (GPIOA->IDR & GPIO_IDR_ID1) ? 1 : 0; // Read PA1

        // Check the state for sensor 0 (PA0)
        if (active_sensor == 0 && new_sensor0_state != sensor0_state) { // Only act on state change
            sensor0_state = new_sensor0_state; // Update the state

            if (sensor0_state) { // If sensor 0 detects motion
                if (countIN < 4) {
                    countIN++; // Increment countIN
                    USART2_SendString("Car is entering the parking lot\r\n"); // Send message via Bluetooth
                    if (countIN == 4) {
                        // If countIN reaches 4, switch to sensor 1
                        active_sensor = 1;
                        countOUT = 0; // Reset countOUT
                        USART2_SendString("Parking is now full\r\n"); // Indicate parking is full
                    }
                } else {
                    USART2_SendString("Parking is full, entry not allowed\r\n"); // Send message if parking is full
                }
                if (current_angle != 90) {
                    Set_Servo_Angle(90); // Move to 90 degrees
                    current_angle = 90;
                }
            } else { // If no motion detected by sensor 0
                if (current_angle != 0) {
                    Set_Servo_Angle(0); // Move back to 0 degrees
                    current_angle = 0;
                }
            }
        }

        // Check the state for sensor 1 (PA1)
        if (active_sensor == 1 && new_sensor1_state != sensor1_state) { // Only act on state change
            sensor1_state = new_sensor1_state; // Update the state

            if (sensor1_state) { // If sensor 1 detects motion
                if (countOUT < 4) { // Ensure countOUT does not exceed 4
                    countOUT++; // Increment countOUT
                    USART2_SendString("Car is exiting the parking lot\r\n"); // Send message via Bluetooth
                    if (countOUT == 4) {
                        // If countOUT reaches 4, switch back to sensor 0
                        active_sensor = 0;
                        countIN = 0; // Reset countIN
                    }
                }
                if (current_angle != 90) {
                    Set_Servo_Angle(90); // Move to 90 degrees
                    current_angle = 90;
                }
            } else { // If no motion detected by sensor 1
                if (current_angle != 0) {
                    Set_Servo_Angle(0); // Move back to 0 degrees
                    current_angle = 0;
                }
            }
        }

        // Read ADC value (from a connected analog sensor)
        adc_value = ADC_Read(0); // Read from ADC channel 0 (PA0 or any configured ADC channel)

        Delay_ms(100); // Small delay to prevent rapid toggling
    }
}

void GPIO_Config(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIOC clock
 
    // Configure PA0 and PA1 as input for IR sensors
    GPIOA->MODER &= ~GPIO_MODER_MODER0; // Set PA0 as input
    GPIOA->MODER &= ~GPIO_MODER_MODER1; // Set PA1 as input
	
    // Configure PC13 as output for LED indication
    GPIOC->MODER &= ~GPIO_MODER_MODER13; // Clear mode bits for PC13
    GPIOC->MODER |= GPIO_MODER_MODER13_0; // Set PC13 as output

    // Set PC13 as Push-Pull and No Pull-Up/Down
    GPIOC->OTYPER &= ~GPIO_OTYPER_OT13;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;

    // Configure PB6 as alternate function for PWM (TIM4_CH1)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB clock
    GPIOB->MODER &= ~GPIO_MODER_MODER6; // Clear mode bits for PB6
    GPIOB->MODER |= GPIO_MODER_MODER6_1; // Set PB6 to alternate function mode
    GPIOB->AFR[0] |= (2 << 24); // Set AF2 (TIM4) for PB6
}

void Timer4_PWM_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; // Enable TIM4 clock

    // Set prescaler and auto-reload values for 50Hz PWM frequency
    TIM4->PSC = 15; // Prescaler (for 1 MHz timer frequency)
    TIM4->ARR = 20000 - 1; // Auto-reload for 20 ms period (50Hz PWM)

    // Configure PWM mode for Channel 1 (PB6)
    TIM4->CCMR1 |= (6 << 4); // Set OC1M to PWM mode 1 (bits 5:4)
    TIM4->CCMR1 |= TIM_CCMR1_OC1PE; // Enable preload for channel 1

    // Enable output for channel 1
    TIM4->CCER |= TIM_CCER_CC1E; // Enable output for channel 1

    TIM4->CR1 |= TIM_CR1_CEN; // Enable TIM4
}

void ADC_Config(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // Enable ADC1 clock

    // Configure ADC1
    ADC1->CR1 &= ~ADC_CR1_RES; // Set resolution to 12 bits
    ADC1->SQR3 = 0; // Set first conversion to channel 0 (PA0 or the channel you want to use)
    ADC1->CR2 |= ADC_CR2_ADON; // Enable ADC
    Delay_ms(1); // Delay for ADC stabilization
}

uint32_t ADC_Read(uint8_t channel) {
    ADC1->SQR3 = channel; // Set the desired channel
    ADC1->CR2 |= ADC_CR2_SWSTART; // Start the conversion

    while (!(ADC1->SR & ADC_SR_EOC)); // Wait for conversion to complete
    return ADC1->DR; // Return the ADC value
}

void Set_Servo_Angle(uint8_t angle) {
    uint32_t pulseWidth = (angle * 2000 / 180) + 500; // Calculate pulse width for servo (500-2500us)

    TIM4->CCR1 = pulseWidth; // Set duty cycle for servo
    Delay_ms(20); // Allow time for servo to reach position
}
	
void Delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        for (uint32_t j = 0; j < 2500; j++) { // Reduced from 4000 to 2500 for shorter delays
            __NOP(); // NOP instruction for delay
        }
    }
}

void GPIOC_Init(void) {
    RCC->AHB1ENR |= (1 << 2);  // Enable clock for GPIOC
    GPIOC->MODER &= ~(3 << (13 * 2));  // Clear mode bits for PC13
    GPIOC->MODER |= (1 << (13 * 2));   // Set PC13 to output mode
}

void GPIOA_Init(void) {
    RCC->AHB1ENR |= (1 << 0); // Enable GPIOA clock
    GPIOA->MODER &= ~(3 << 4); // Clear mode bits for PA2 and PA3
    GPIOA->MODER |= (2 << 4); // Set PA2 to alternate function mode
    GPIOA->MODER |= (2 << 6); // Set PA3 to alternate function mode
    GPIOA->AFR[0] |= (7 << 8) | (7 << 12); // Set PA2 and PA3 to AF7 (USART2)
}

void USART2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Enable USART2 clock
    USART2->BRR = 0x0683; // 9600 baud rate for 16 MHz clock
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // Enable TX, RX, USART
}

char USART2_Read(void) {
    while (!(USART2->SR & USART_SR_RXNE)); // Wait for RXNE to be set
    return USART2->DR; // Return received data
}

void USART2_Write(char c) {
    while (!(USART2->SR & USART_SR_TXE)); // Wait for TXE to be set
    USART2->DR = c; // Send character
}

void USART2_SendString(char *str) {
    while (*str) {
        USART2_Write(*str++);
    }
}