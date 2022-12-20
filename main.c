/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "keypad.h"

#define A 0
#define B 1
#define E 2
#define M 3
#define S 4
#define O 5
#define N 6
#define Y 7
#define G 8


char* morse[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--",
                    "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

int semaphoreRotationR[9] = {3200, 3200, 6250, 5500, 4300, 850, 4300, 5500, 4300}; 
int semaphoreRotationL[9] = {3300, 2300, 4800, 3300, 2300, 1300, 3300, 1300, 4800}; 

uint8_t rxData, prevRxData;
int flagLight = 0;
char text_rx[256];
char text_tx[256];
char save[256];
char copy[256];
char test[256];
uint8_t count = 0;
int timerCount = 0;
int f=2;
float fs = 100.0;
int sig[100];
float t;



// use the two semaphores for each letter with different angles
void movementSemaphore(char letter){
    if(letter == 'A' || letter == 'a'){
        PWM_WriteCompare(semaphoreRotationR[A]); 
        PWM_1_WriteCompare(semaphoreRotationL[A]);
    }
    else if(letter == 'B' || letter == 'b'){
        PWM_WriteCompare(semaphoreRotationR[B]); 
        PWM_1_WriteCompare(semaphoreRotationL[B]);
    }
    else if(letter == 'E' || letter == 'e'){
        PWM_WriteCompare(semaphoreRotationR[E]); 
        PWM_1_WriteCompare(semaphoreRotationL[E]);
    }
    else if(letter == 'M' || letter == 'm'){
        PWM_WriteCompare(semaphoreRotationR[M]); 
        PWM_1_WriteCompare(semaphoreRotationL[M]);
    }
    else if(letter == 'S' || letter == 's'){
        PWM_WriteCompare(semaphoreRotationR[S]); 
        PWM_1_WriteCompare(semaphoreRotationL[S]);
    }
    else if(letter == 'O' || letter == 'o'){
        PWM_WriteCompare(semaphoreRotationR[O]); 
        PWM_1_WriteCompare(semaphoreRotationL[O]);
    }
    else if(letter == 'N' || letter == 'n'){
        PWM_WriteCompare(semaphoreRotationR[N]); 
        PWM_1_WriteCompare(semaphoreRotationL[N]);
    }
    else if(letter == 'Y' || letter == 'y'){
        PWM_WriteCompare(semaphoreRotationR[Y]); 
        PWM_1_WriteCompare(semaphoreRotationL[Y]);
    }
    else if(letter == 'G' || letter == 'g'){
        PWM_WriteCompare(semaphoreRotationR[G]); 
        PWM_1_WriteCompare(semaphoreRotationL[G]);
    }
}


// convert letter to morse
char* char2morse(char letter){
    if (letter >= 65 && letter <= 90){
        return morse[letter-65];
    }else if (letter >= 97 && letter <= 122){
        return morse[letter-97];
    }
    return "none";
}

// this method prepare the wave for the sound 
void prepareToPlay (){
    // initialize values
    for(int i=0;i<100;i++){
        t = (float)i/fs;
        sig[i] = (int)(127.0*(sin(2*3.14*f*t)));
    }
}

void waitConversion(){
   while(!ADC_IsEndConversion(ADC_RETURN_STATUS)){
        CyDelay(1);}
}


// the audio for the sound 
void SignalAudio(){
    for(int i=0;i<250;i++){
        DAC_SetValue(i);         
    }
}

//the audio for short  sound 
void shortSignalAudio(){
    
    while(timerCount != 250){
        if(0x80 & Timer_DAC_ISR_ReadStatusRegister()){
            for(int i=0;i<250;i++){
                DAC_SetValue(i);         
            }
            timerCount++;            
        }
    }
    timerCount = 0;
    CyDelay(250);
}

//the audio for long sound 
void longSignalAudio(){
    while(timerCount != 750){
        if(0x80 & Timer_DAC_ISR_ReadStatusRegister()){
            for(int i=0;i<250;i++){
                DAC_SetValue(i);         
            }
            timerCount++;            
        }
    }
    timerCount = 0;
    CyDelay(250);
}
   

void ledsUp(){
    LED1_Write(1);
    LED2_Write(1);
    LED3_Write(1);
    LED4_Write(1);
}

void ledsDown(){
    LED1_Write(0);
    LED2_Write(0);
    LED3_Write(0);
    LED4_Write(0);
}

// turn the semaphore and use the sound if there is light or not  (photoresistor)
void decryptASCII(char* msg){
    int i=0;
    int j=0;
    
    while(msg[i] != '\0'){
        strcpy(save, char2morse(msg[i]));
        while (save[j] != '\0'){
            if (save[j] == '.'){
                if(!flagLight){
                    // Semaphore
                    movementSemaphore(msg[i]);
                }
                // Launch Audio
                shortSignalAudio();
            } 
            if (save[j] == '-'){
                if(!flagLight){
                    // Semaphore
                    movementSemaphore(msg[i]);
                }
                // Launch Audio
                longSignalAudio();
            }
            j++;
        }
        CyDelay(300);
        j = 0;
        i++;   
    }
}

void shortLight(){
    ledsUp(); 
    CyDelay(100); 
    ledsDown(); 
}


void longLight(){
    ledsUp();
    CyDelay(300);
    ledsDown();
}

// turn the light on if there is light or not (photoresistor)
void decryptLight(char* msg){
    if(flagLight){
    int j=0;
    int h=0;
    while(msg[j] != '\0'){
        strcpy(save, char2morse(msg[j]));
        while (save[h] != '\0'){
            if (save[h] == '.'){
                shortLight();
                CyDelay(100);
            } 
            if (save[h] == '-'){
                longLight();
                CyDelay(100);
            }
            h++;
        }
        UART_PutString(save);
        CyDelay(500);
        h = 0;
        j++;   
    }
    UART_PutString("\n");
    }
}

char *truncString(char *str, int pos)
{
    size_t len = strlen(str);

    if ((int)len > abs(pos)) {
        if (pos > 0){
            str = str + pos;
        }else{
            str[len + pos] = 0;
        }
    }

    return str;
}

void LcdDysplay(char* msg){
    char *msgcpy = (char *) malloc( strlen(msg) + 1 ); 
    strncpy( msgcpy, msg, strlen(msg) + 1 );
    size_t length = strlen(msgcpy);
    LCD_Char_ClearDisplay();
    LCD_Char_Position(0,0);
    LCD_Char_PrintString(msgcpy);
    if(length > 8){
        char* str = truncString(msgcpy,8);
        LCD_Char_Position(1,0);
        LCD_Char_PrintString(str);
    }
    free(msgcpy);
}

CY_ISR(isr_uart_Handler){

    uint8_t status = 0;

    do {
        CyDelay(50);
        status = UART_ReadRxStatus();
        
        if (( status & UART_RX_STS_PAR_ERROR ) |
        ( status & UART_RX_STS_STOP_ERROR ) |
        ( status & UART_RX_STS_BREAK ) |
        ( status & UART_RX_STS_OVERRUN ) ) {
            // Parity , framing , break or overrun error
            LcdDysplay("UART Error");
            UART_PutString("UART Error\n");

            
        }
        
        
        if(( status & UART_RX_STS_FIFO_NOTEMPTY) != 0 ){
            rxData = UART_ReadRxData();        
            text_rx[count] = rxData;

            count++;
            
            
        }else{
            LcdDysplay(text_rx);
            sprintf(test, "%d\n ",count);
            
            decryptASCII(text_rx);
            decryptLight(text_rx);
            for(int i=0; i<count; ++i){ // Convert received string to morse
                if (text_rx[i] >= 65 && text_rx[i] <= 90){
                    sprintf(text_tx, "%s%s ", text_tx, morse[text_rx[i]-65]);
                }else if (text_rx[i] >= 97 && text_rx[i] <= 122){
                    sprintf(text_tx, "%s%s ", text_tx, morse[text_rx[i]-97]);
                }
            }
            sprintf(text_tx, "%s\n", text_tx);
            
            memset(text_rx, 0, sizeof text_rx); // to empty
            memset(text_tx, 0, sizeof text_tx); // to empty
            count =0;
        }
    }while((status & UART_RX_STS_FIFO_NOTEMPTY) != 0 );
    UART_PutString("\n");

}


int checkPhotoresistor(uint16_t* val_adc){
    if(ADC_IsEndConversion(ADC_RETURN_STATUS)){
        *val_adc = ADC_GetResult32();
        if(*val_adc < 0x1500){
            flagLight = 0;   
        }
        else{
            flagLight = 1;
        }
    }
    return flagLight;
}

void getInput(uint8_t* firstInput,uint8_t* secondInput){
    uint8_t temp = keypadScan();
    if(*firstInput == 'z' || temp == *firstInput){
        *firstInput = temp;
    }
    if(temp != *firstInput){
        *secondInput = temp;
    }
}

void displayOrSignal(uint8_t* firstInput, uint8_t* secondInput) {
    
    if((*firstInput == '*' || *secondInput == '*')) {
        if ((*firstInput == '8' || *secondInput == '8')) {
            
            LCD_Char_ClearDisplay();
            LCD_Char_PrintString("SOS");
            decryptLight("SOS");
            decryptASCII("SOS");
        }
        
        if ((*firstInput == '5' || *secondInput == '5')) {
            
            LCD_Char_ClearDisplay();
            LCD_Char_PrintString("BEAMS");
            decryptLight("BEAMS");
            decryptASCII("BEAMS");
        }
        if((*firstInput != 'z' && *secondInput != 'z')){
            *firstInput = 'z';
            *secondInput = 'z';
            CyDelay(100);
        }
    }else if((*firstInput != 'z' && *secondInput != 'z')){
        *firstInput = 'z';
        *secondInput = 'z';
        CyDelay(100);
    }
    
}

int main(void)
{
    keypadInit();//initialize keypad
    
    prepareToPlay();
    int flag = 0;
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    
    Timer_DAC_ISR_Init();
    Timer_DAC_ISR_Start();
    
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    LCD_Char_Init();
    LCD_Char_Start();
    LCD_Char_Position(0,0);
    LCD_Char_PrintString("Welcome");
    
    
    ADC_Start();
    int val_CMP = 0;
    uint16_t val_adc = 0;

    DAC_Start();
    
    
    PWM_WritePeriod(4800);
    PWM_WriteCompare(val_CMP);
    PWM_Start();
    
    
    PWM_1_WritePeriod(4800);
    PWM_1_Start();
    
    AMux_1_Init();
    AMux_1_Start();
    
    uint8_t firstInput = 'z';
    uint8_t secondInput = 'z';
    
    
    UART_Start();   
    UART_PutString("\n START : \n");
    
    isr_UART_StartEx(isr_uart_Handler);
    
    
    for(;;)
    {
        
        getInput(&firstInput,&secondInput);
        displayOrSignal(&firstInput,&secondInput); 
        
        AMux_1_Select(0);
        ADC_StartConvert();
        waitConversion();
        if(ADC_IsEndConversion(ADC_RETURN_STATUS)){
            val_adc = ADC_GetResult16();
            val_CMP = (val_adc/10);
            if(!flagLight){
                if(flag && val_CMP < 5800){
                    PWM_WriteCompare(val_CMP); 
                }
                else if( val_CMP < 5800){
                    PWM_1_WriteCompare(val_CMP);
                }
            }
        }
        
        AMux_1_Select(1);
        ADC_StartConvert();
        waitConversion();
        
        checkPhotoresistor(&val_adc);
        
        if(SW1_Read()){
            while(SW1_Read()){
                if(flagLight){
                    ledsUp();
                }
                SignalAudio();
            }
        }else if(SW2_Read()){
            if(flagLight){
                ledsUp();
            }
            shortSignalAudio();   
        }else if(SW3_Read()){
            if(flagLight){
                ledsUp();
            }
            longSignalAudio();
        }else if(SW4_Read()){
            flag = !flag;
            CyDelay(150);
        }
        ledsDown();
    }
}
/* [] END OF FILE */
