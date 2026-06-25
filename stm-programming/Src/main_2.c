/*
 * Name: Andru Alexis Gomez Serna
 * ID Number: 1038415682
 *
 * DOCUMENTATION
 *
 * COLOR CODE
 * - Segments (A-G): Blue, Gray, White
 * - Digit Enables (COM1-COM4): Blue, White
 * - Optical Sensors (PC0, PC1): Black / Orange
 * - Power 3.3V: Red
 * - GND: Black, Green
 *
 * FUNCTIONALITY & HARDWARE MAPPING
 * - Optical Sensors: 2 inputs with active internal Pull-Up, falling-edge detection.
 * - Status LED (Blinky): PH1, toggles state automatically.
 * - 7-Segment Display: 4 digits, Common Cathode.
 * - Segment Mapping (A-G): PC6, PC8, PC9, PB8, PB9, PA5, PA6
 * - Digit Mapping (COM1-COM4): PB12, PA11, PA12, PC5 (Transistor controlled)
 * - TIM2: Timer handling the Blinky LED (PH1) every 200ms.
 * - TIM3: Timer for dynamic display multiplexing every 2ms (125Hz refresh rate).
 * - EXTI0 & EXTI1: External interrupts assigned to sensors for counter control.
 * - Display Value: Cyclic counting from 0000 to 4096 with automatic rollover at both ends.
 */

#include "stm32f4xx.h"

// GLOBAL VARIABLES & BUFFERS

volatile int16_t contador = 0;                    // Core counter variable
volatile uint8_t display_data[4] = {0, 0, 0, 0};  // Screen buffer [Thousands, Hundreds, Tens, Units]
volatile uint8_t digit_actual = 0;                // Active digit pointer for multiplexing

// FUNCTION PROTOTYPES

void GPIO_Init(void);
void Timers_Init(void);
void EXTI_Init(void);
void Limpiar_Seg(void);
void Prender_Seg(uint8_t seg);
void Set_Seg(uint8_t number);
void Actualizar_Display_Data(void);

// MAIN APPLICATION ENTRY POINT

int main(void)
{
    /* Initialize system peripherals */
    GPIO_Init();
    EXTI_Init();
    Timers_Init();

    /* Initialize display buffer with default values */
    Actualizar_Display_Data();

    /* Super loop - Execution is fully interrupt-driven */
    while(1)
    {
        // Idle loop. All critical tasks are handled by ISRs (Interrupts)
    }

    return 0;
}


// PERIPHERAL INITIALIZATION FUNCTIONS


void GPIO_Init(void) {
    /* 1. Enable AHB1 peripheral clocks for GPIO Ports */
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOHEN);

    /* 2. Configure General Purpose Outputs (MODER = 01) */
    // GPIOA: PA5, PA6, PA11, PA12
    GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE11 | GPIO_MODER_MODE12);
    GPIOA->MODER |= (GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE11_0 | GPIO_MODER_MODE12_0);

    // GPIOB: PB8, PB9, PB12
    GPIOB->MODER &= ~(GPIO_MODER_MODE8 | GPIO_MODER_MODE9 | GPIO_MODER_MODE12);
    GPIOB->MODER |= (GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0 | GPIO_MODER_MODE12_0);

    // GPIOC: PC5, PC6, PC8, PC9
    GPIOC->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE8 | GPIO_MODER_MODE9);
    GPIOC->MODER |= (GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0);

    // GPIOH: PH1 (Heartbeat LED)
    GPIOH->MODER &= ~GPIO_MODER_MODE1;
    GPIOH->MODER |= GPIO_MODER_MODE1_0;

    /* 3. Configure Output Type as Push-Pull (OTYPER = 0) */
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 | GPIO_OTYPER_OT11 | GPIO_OTYPER_OT12);
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9 | GPIO_OTYPER_OT12);
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 | GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);
    GPIOH->OTYPER &= ~GPIO_OTYPER_OT1;

    /* 4. Set GPIO Output Speed to High Speed (OSPEEDR = 10) */
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR5_1 | GPIO_OSPEEDER_OSPEEDR6_1 | GPIO_OSPEEDER_OSPEEDR11_1 | GPIO_OSPEEDER_OSPEEDR12_1);
    GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8_1 | GPIO_OSPEEDER_OSPEEDR9_1 | GPIO_OSPEEDER_OSPEEDR12_1);
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR5_1 | GPIO_OSPEEDER_OSPEEDR6_1 | GPIO_OSPEEDER_OSPEEDR8_1 | GPIO_OSPEEDER_OSPEEDR9_1);
}

void EXTI_Init(void) {
    /* Enable System Configuration Controller Clock */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /* Enable Internal Pull-Up Resistors for PC0 and PC1 */
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1);
    GPIOC->PUPDR |= (GPIO_PUPDR_PUPD0_0 | GPIO_PUPDR_PUPD1_0);

    /* Route EXTI Lines 0 and 1 to GPIOC Port */
    SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0 | SYSCFG_EXTICR1_EXTI1);
    SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PC | SYSCFG_EXTICR1_EXTI1_PC);

    /* Set Trigger Source to Falling-Edge Only */
    EXTI->FTSR |= (EXTI_FTSR_TR0 | EXTI_FTSR_TR1);
    EXTI->RTSR &= ~(EXTI_RTSR_TR0 | EXTI_RTSR_TR1);

    /* Unmask EXTI Interrupt Lines */
    EXTI->IMR |= (EXTI_IMR_MR0 | EXTI_IMR_MR1);

    /* Register EXTI Handlers in NVIC */
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);
}

void Timers_Init(void) {
    /* Enable APB1 Peripheral Timer Clocks */
    RCC->APB1ENR |= (RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN);

    /* -- TIM2 Configuration (200ms Timebase for Blinky) -- */
    TIM2->PSC = 16000 - 1;       // Clock down to 1kHz
    TIM2->ARR = 200 - 1;         // Period = 200ms
    TIM2->CR1 |= TIM_CR1_ARPE;   // Auto-reload preload enabled
    TIM2->DIER |= TIM_DIER_UIE;  // Update Interrupt Enabled
    TIM2->EGR |= TIM_EGR_UG;     // Force register update
    TIM2->SR &= ~TIM_SR_UIF;     // Clear update flag
    TIM2->CR1 |= TIM_CR1_CEN;    // Start counter
    NVIC_EnableIRQ(TIM2_IRQn);

    /* -- TIM3 Configuration (2ms Timebase for multiplexing) -- */
    TIM3->PSC = 16000 - 1;
    TIM3->ARR = 2 - 1;
    TIM3->CR1 |= TIM_CR1_ARPE;
    TIM3->DIER |= TIM_DIER_UIE;
    TIM3->EGR |= TIM_EGR_UG;
    TIM3->SR &= ~TIM_SR_UIF;
    TIM3->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM3_IRQn);
}


// HARDWARE DRIVER & UTILITY FUNCTIONS


void Actualizar_Display_Data(void) {
    display_data[0] = (contador / 1000) % 10; // Thousands
    display_data[1] = (contador / 100) % 10;  // Hundreds
    display_data[2] = (contador / 10) % 10;   // Tens
    display_data[3] = contador % 10;          // Units
}

void Limpiar_Seg(void) {
    /* Clear all segment lines */
    GPIOC->BSRR = (GPIO_BSRR_BR6 | GPIO_BSRR_BR8 | GPIO_BSRR_BR9);
    GPIOB->BSRR = (GPIO_BSRR_BR8 | GPIO_BSRR_BR9);
    GPIOA->BSRR = (GPIO_BSRR_BR5 | GPIO_BSRR_BR6);
}

void Prender_Seg(uint8_t seg) {
    /* Set specific segment high */
    switch(seg) {
        case 0: GPIOC->BSRR = GPIO_BSRR_BS6; break; // Segment A
        case 1: GPIOC->BSRR = GPIO_BSRR_BS8; break; // Segment B
        case 2: GPIOC->BSRR = GPIO_BSRR_BS9; break; // Segment C
        case 3: GPIOB->BSRR = GPIO_BSRR_BS8; break; // Segment D
        case 4: GPIOB->BSRR = GPIO_BSRR_BS9; break; // Segment E
        case 5: GPIOA->BSRR = GPIO_BSRR_BS5; break; // Segment F
        case 6: GPIOA->BSRR = GPIO_BSRR_BS6; break; // Segment G
    }
}

void Set_Seg(uint8_t number) {
    if (number > 9) number = 0;
    Limpiar_Seg();

    /* Decode digit into 7-segment representation */
    switch (number) {
        case 0: Prender_Seg(0); Prender_Seg(1); Prender_Seg(2); Prender_Seg(3); Prender_Seg(4); Prender_Seg(5); break;
        case 1: Prender_Seg(1); Prender_Seg(2); break;
        case 2: Prender_Seg(0); Prender_Seg(1); Prender_Seg(3); Prender_Seg(4); Prender_Seg(6); break;
        case 3: Prender_Seg(0); Prender_Seg(1); Prender_Seg(2); Prender_Seg(3); Prender_Seg(6); break;
        case 4: Prender_Seg(1); Prender_Seg(2); Prender_Seg(5); Prender_Seg(6); break;
        case 5: Prender_Seg(0); Prender_Seg(2); Prender_Seg(3); Prender_Seg(5); Prender_Seg(6); break;
        case 6: Prender_Seg(0); Prender_Seg(2); Prender_Seg(3); Prender_Seg(4); Prender_Seg(5); Prender_Seg(6); break;
        case 7: Prender_Seg(0); Prender_Seg(1); Prender_Seg(2); break;
        case 8: for (int i=0; i<7; i++) Prender_Seg(i); break;
        case 9: Prender_Seg(0); Prender_Seg(1); Prender_Seg(2); Prender_Seg(3); Prender_Seg(5); Prender_Seg(6); break;
    }
}


// INTERRUPT SERVICE ROUTINES (ISRs)


void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR0)
    {
        EXTI->PR |= EXTI_PR_PR0; // Clear pending flag

        contador--;
        if (contador < 0) {
            contador = 4096; // Underflow wrap-around
        }
        Actualizar_Display_Data();
    }
}

void EXTI1_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR1)
    {
        EXTI->PR |= EXTI_PR_PR1; // Clear pending flag

        contador++;
        if (contador > 4096) {
            contador = 0; // Overflow wrap-around
        }
        Actualizar_Display_Data();
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;    // Clear update flag
        GPIOH->ODR ^= GPIO_ODR_OD1; // Toggle Heartbeat Pin
    }
}

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & TIM_SR_UIF)
    {
        TIM3->SR &= ~TIM_SR_UIF;    // Clear update flag

        /* 1. Disable all digit control pins (Blanking period to prevent ghosting) */
        GPIOB->BSRR = GPIO_BSRR_BR12;
        GPIOA->BSRR = (GPIO_BSRR_BR11 | GPIO_BSRR_BR12);
        GPIOC->BSRR = GPIO_BSRR_BR5;

        /* 2. Load next digit segment pattern */
        Set_Seg(display_data[digit_actual]);

        /* 3. Enable current active digit control pin */
        switch(digit_actual) {
            case 0: GPIOB->BSRR = GPIO_BSRR_BS12; break; // Digit 1 Control
            case 1: GPIOA->BSRR = GPIO_BSRR_BS11; break; // Digit 2 Control
            case 2: GPIOA->BSRR = GPIO_BSRR_BS12; break; // Digit 3 Control
            case 3: GPIOC->BSRR = GPIO_BSRR_BS5;  break; // Digit 4 Control
        }

        /* 4. Update pointer for the subsequent multiplexing cycle */
        digit_actual++;
        if (digit_actual > 3) {
            digit_actual = 0;
        }
    }
}
