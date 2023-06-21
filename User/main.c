/*

 Matshushita JR-100 Emulator for CH32V203
 You must use HSE 96MHz clock.
 (May be work on 48MHz by change SPI/Timer prescaler value)

 TIM1CH4:  (PA11) Sync signal
 TIM1CH3:  (NC)  Video out timing interrupt
 SPI1MOSI: (PA7) Video signal

 PC4 -- R1 --+
 PC6 -- R2 --+---> Video

 R1: 560 ohm
 R2: 240 ohm

 TIM4CH4: (PB9)  Sound
 Key input: USART2_RX ��PA3��

 */

#include "debug.h"
#include "jr100rom.h"
#include "jr100guldus.h"
#include "cpuintrf.h"
#include "m6800.h"
#include "ch32v20x_usb.h"
#include "usb_host_config.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Global define */

#define NTSC_COUNT 3050 // = 63.56 us / 48MHz
#define NTSC_HSYNC 225  // =  4.7  us / 48MHz
#define NTSC_VSYNC 2825 // = NTSC_COUNT - NTSC_HSYNC
#define NTSC_SCAN_DELAY 0 // Delay for video signal generation
#define NTSC_SCAN_START 40 // Display start line

#define NTSC_X_PIXELS 256
#define NTSC_Y_PIXELS 192

#define NTSC_PRESCALER SPI_BaudRatePrescaler_16

#define NTSC_X_CHARS (NTSC_X_PIXELS/8)
#define NTSC_Y_CHARS (NTSC_Y_PIXELS/8)

//#define USE_USB_KEYBOARD

/* Global Variable */

volatile uint16_t ntsc_line;
volatile uint8_t ntsc_blank = 0;
volatile uint8_t run_emulation = 0;

uint8_t *vram;
uint8_t *scandata[2];
uint8_t keymatrix[9];
uint8_t via_reg[16];

// 700 bytes left
//16KB
#define MEM_SIZE 16384
const uint8_t memmap[] = { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 3, 3, 3, 3 }; // Memory map 1=RAM,2=VRAM,3=ROM,4=IO

//14KB
//#define MEM_SIZE (14336+768)
//const uint8_t memmap[] = { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//        0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 3, 3, 3, 3 }; // Memory map 1=RAM,2=VRAM,3=ROM,4=IO

// keymap
const uint8_t keymap[] = { 0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 0xff, 0,
        0, // scanline , key ,modifyer(shift/ctrl)
        0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 8, 16, 2, 0xff, 0, 0,
        0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 8, 8, 0, 0xff, 0, 0, 0xff, 0,
        0, // 0x0D = CR
        0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0,
        0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 0, 16, 2,
        0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 0xff, 0, 0, 8, 2, 0, 3, 1, 1, 3,
        2, // 0x1B (ESC = break (Ctrl+C)
        1, 3, 4,
        1, // 0x20
        3, 8, 1, 3, 16, 1, 4, 1, 1, 4, 2, 1, 4, 4, 1, 4, 8, 1, 8, 4, 1, 6, 16,
        1, 7, 16, 0, 8, 16, 0, 8, 1, 0, 6, 8, 1, 4, 16, 0, 3, 1, 0, 3, 2, 0, 3,
        4,
        0, // 0x30
        3, 8, 0, 3, 16, 0, 4, 1, 0, 4, 2, 0, 4, 4, 0, 4, 8, 0, 8, 4, 0, 6, 16,
        0, 7, 16, 1, 8, 16, 1, 8, 1, 1, 6, 4, 1, 5, 1, 1, 1, 1, 0, 7, 2, 0, 0,
        16,
        0, // 0x40
        1, 4, 0, 2, 4, 0, 1, 8, 0, 1, 16, 0, 6, 1, 0, 5, 4, 0, 6, 2, 0, 6, 4, 0,
        6, 8, 0, 7, 8, 0, 7, 4, 0, 5, 8, 0, 5, 16, 0, 2, 1, 0, 2, 8, 0, 1, 2,
        0, // 0x50
        2, 16, 0, 5, 2, 0, 7, 1, 0, 2, 2, 0, 0, 8, 0, 5, 1, 0, 0, 4, 0, 5, 8, 1,
        5, 4, 1, 5, 16, 1, 4, 16, 1, 7, 8, 1 };

// general global variables
//extern uint8_t RAM[];            // any reads or writes to ports or vectors are trapped in SW
uint8_t *RAM; // any reads or writes to ports or vectors are trapped in SW

//uint8_t cursor_x, cursor_y = 0;

// TVout for CH32V203

void video_init() {

    TIM_OCInitTypeDef TIM_OCInitStructure = { 0 };
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = { 0 };
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };
    SPI_InitTypeDef SPI_InitStructure = { 0 };
    NVIC_InitTypeDef NVIC_InitStructure = { 0 };

    RCC_APB2PeriphClockCmd(
    RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_TIM1, ENABLE);

    // PC4:Sync

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure);

    // PC6: Video (SPI1)

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure);

    // Initalize TIM1

    TIM_TimeBaseInitStructure.TIM_Period = NTSC_COUNT;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1;                // Presclaer = 0
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = NTSC_HSYNC;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init( TIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = NTSC_HSYNC * 2 - NTSC_SCAN_DELAY; // 9.4usec - delay
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init( TIM1, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_OC3PreloadConfig( TIM1, TIM_OCPreload_Disable);
    TIM_OC4PreloadConfig( TIM1, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig( TIM1, ENABLE);
    TIM_Cmd( TIM1, ENABLE);

    // Initialize SPI1

    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = NTSC_PRESCALER; // 6MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init( SPI1, &SPI_InitStructure);

    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
    SPI_Cmd(SPI1, ENABLE);

    // NVIC

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);

    // Init VRAM

//    vram = malloc(NTSC_X_CHARS * NTSC_Y_CHARS);
    vram = malloc(1024);

    scandata[0] = malloc(NTSC_X_CHARS + 1);
    scandata[1] = malloc(NTSC_X_CHARS + 1);

    scandata[0][NTSC_X_CHARS] = 0;
    scandata[1][NTSC_X_CHARS] = 0;

    //

}

static inline void video_cls() {
    memset(vram + 0x100, 0, (NTSC_X_CHARS * NTSC_Y_CHARS));
}
/*
 static inline void video_scroll() {

 memmove(vram + 0x100, vram + 0x100 + NTSC_X_CHARS,
 NTSC_X_CHARS * (NTSC_Y_CHARS - 1));

 memset(vram + 0x100 + NTSC_X_CHARS * (NTSC_Y_CHARS - 1), 0, NTSC_X_CHARS);

 }

 static inline void video_print(uint8_t *string) {

 int len;

 len = strlen(string);

 for (int i = 0; i < len; i++) {
 vram[cursor_x + cursor_y * NTSC_X_CHARS + 0x100] = string[i];
 cursor_x++;
 if (cursor_x >= NTSC_X_CHARS) {
 cursor_x = 0;
 cursor_y++;
 if (cursor_y >= NTSC_Y_CHARS) {
 video_scroll();
 cursor_y = NTSC_Y_CHARS - 1;
 }
 }
 }

 }
 */
void video_wait_vsync() {

    while(ntsc_blank==1);
    while(ntsc_blank==0);

}
/*
 void video_debug(uint16_t addr, uint8_t op) {

 vram[0x100] = ((addr >> 12) & 0xf) + 0x10;
 vram[0x101] = ((addr >> 8) & 0xf) + 0x10;
 vram[0x102] = ((addr >> 4) & 0xf) + 0x10;
 vram[0x103] = ((addr & 0x0f)) + 0x10;

 vram[0x105] = ((op >> 4) & 0xf) + 0x10;
 vram[0x106] = ((op & 0x0f)) + 0x10;

 if (vram[0x100] > 0x19)
 vram[0x100] += 7;
 if (vram[0x101] > 0x19)
 vram[0x101] += 7;
 if (vram[0x102] > 0x19)
 vram[0x102] += 7;
 if (vram[0x103] > 0x19)
 vram[0x103] += 7;
 if (vram[0x105] > 0x19)
 vram[0x105] += 7;
 if (vram[0x106] > 0x19)
 vram[0x106] += 7;

 }
 */

/*********************************************************************
 * @fn      DMA_Tx_Init
 *
 * @brief   Initializes the DMAy Channelx configuration.
 *
 * @param   DMA_CHx - x can be 1 to 7.
 *          ppadr - Peripheral base address.
 *          memadr - Memory base address.
 *          bufsize - DMA channel buffer size.
 *
 * @return  none
 */
void DMA_Tx_Init(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr,
        u16 bufsize) {
    DMA_InitTypeDef DMA_InitStructure = { 0 };

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);

    DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = bufsize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &DMA_InitStructure);

}

void TIM4_PWMOut_Init(u16 arr, u16 psc, u16 ccp) {
    TIM_OCInitTypeDef TIM_OCInitStructure = { 0 };
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = { 0 };

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM4, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init( TIM4, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM4, ENABLE);
    TIM_OC4PreloadConfig( TIM4, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig( TIM4, ENABLE);
    TIM_Cmd( TIM4, ENABLE);
}

// Emulator related

void exec6522(uint16_t cycles) {

    uint16_t timer1_count, timer2_count, timer1_control;

    // run Timer 1

    timer1_count = via_reg[4] + (via_reg[5] << 8);

    //   if( timer1_count !=0 ) {          // check timer is running
    if (timer1_count > cycles) {
        timer1_count -= cycles;
    } else {
        //         if((via_reg[0xb]&0x40)==0) {  // One shot
        timer1_count = 0;
        //         } else {                 // Continuous
        timer1_control = via_reg[6] + (via_reg[7] << 8);
        timer1_count = timer1_control - timer1_count - cycles;
        //         }

        if ((via_reg[0xb] & 0x80) != 0) {  // check to toggle PB7
            if ((via_reg[0] & 0x80) == 0) {
                via_reg[0] |= 0xc0;
//                    GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
            } else {
                via_reg[0] &= 0x3f;
//                    GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);

                // run timer 2 (PB7 count mode)

                timer2_count = via_reg[8] + (via_reg[9] << 8);
                if (timer2_count > 0) {
                    timer2_count--;
                    if (timer2_count == 0) {
                        via_reg[0xd] |= 0x20;     // set T2IL
                    }
                }
                via_reg[8] = timer2_count & 0xff;
                via_reg[9] = (timer2_count >> 8) & 0xff;
            }
        }
        via_reg[0xd] |= 0x40;      // set T1IL
    }
//    }

    via_reg[4] = timer1_count & 0xff;
    via_reg[5] = (timer1_count >> 8) & 0xff;

}

void cpu_writemem16(unsigned short addr, unsigned char bdat) { // RAM access is managed here to allow memory-mapped I/O access via traps

    uint16_t timer_control;

    switch (memmap[addr >> 11]) {

    case 1: // RAM
        RAM[addr & 0x3fff] = bdat;
        break;
    case 2: // VRAM
        vram[addr & 0x3ff] = bdat;
        break;
    case 4: // IO
        switch (addr & 0xf) {
        case 4:
        case 6:
            via_reg[4] = bdat;
            via_reg[6] = bdat;
            break;
        case 5:
        case 7:
            via_reg[5] = bdat;
            via_reg[7] = bdat;
            via_reg[0xd] &= 0xbf; // clear T1IL
            if ((via_reg[0xb] & 0xc0) == 0xc0) {
                timer_control = via_reg[6] + (via_reg[7] << 8);
                TIM4_PWMOut_Init(timer_control * 2, 107, timer_control);
            }
            break;
        case 8:
            via_reg[8] = bdat;
            break;
        case 9:
            via_reg[9] = bdat;
            via_reg[0xd] &= 0xdf; // clear T2IL
            break;
        case 0xb:
            if ((bdat & 0xc0) != 0xc0) {
                TIM4_PWMOut_Init(0, 107, 0);
            }
            via_reg[0xb] = bdat;
            break;
        default:
            via_reg[addr & 0xf] = bdat;
        }
    }

}
//
unsigned char cpu_readmem16(unsigned short addr) { // to allow for memory-mapped I/O access

    switch (memmap[addr >> 11]) {

    case 1: // RAM
        return (RAM[addr & 0x3fff]);

    case 2: // VRAM
        return (vram[addr & 0x3ff]);

    case 3: // ROM
        return (rom[addr & 0x1fff]);
    case 4: // IO
        switch (addr & 0xf) {
        case 0:
            return ((keymatrix[via_reg[1] & 0xf]) | (via_reg[0] & 0xc0));
        case 4:
            via_reg[0xf] &= 0xfd; // clear T1IL
            return via_reg[4];
        case 8:
            via_reg[0xf] &= 0xfb; // clear T2IL
            return via_reg[8];
        default:
            return via_reg[addr & 0xf];
        }

    }

    return 0xff;         // traps go above here

}

void beep_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };
    /*
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_Init(GPIOA, &GPIO_InitStructure);
     */

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM4_PWMOut_Init(0, 107, 0);

}

//unsigned char cpu_readop(uint16_t addr) {return cpu_readmem16(addr); };
//unsigned char cpu_readop_arg(uint16_t addr) {return cpu_readmem16(addr); };

// init UART2 for key input

void USART_CFG(void) {
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };
    USART_InitTypeDef USART_InitStructure = { 0 };

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* USART2 TX-->A.2   RX-->A.3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
    USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);

}

void USART_Getkey() {

    static uint8_t ch, pressed;
    static uint8_t col, row;

    if (pressed == 0) { // no modifier keys pressed
        for (int i = 0; i < 9; i++) {
            keymatrix[i] = 0xff;
        }

        if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
            ch = toupper(USART_ReceiveData(USART2));
            if (ch == '|') { // load test game to memory
                memcpy(RAM + 0x1000, jr100guldus, 0x1600);
            }
            //   toupper(ch);
            if (keymap[ch * 3] < 9) {
                if (keymap[ch * 3 + 2] == 1) { // use modifier (Shift)
                    keymatrix[0] &= ~(2);
                    col = keymap[ch * 3];
                    row = keymap[ch * 3 + 1];
                    pressed = 1;
                } else if (keymap[ch * 3 + 2] == 2) { // Control
                    keymatrix[0] &= ~(1);
                    col = keymap[ch * 3];
                    row = keymap[ch * 3 + 1];
                    pressed = 1;
                } else {
                    keymatrix[keymap[ch * 3]] &= ~(keymap[ch * 3 + 1]);
                }
            }
        }
    } else {
        pressed++;
        if (pressed > 2) {
            keymatrix[col] &= ~(row);
            pressed = 0;
        }
    }

}

//

void TIM1_CC_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void TIM1_CC_IRQHandler(void) {

    TIM_ClearFlag(TIM1, TIM_FLAG_CC3);
    uint8_t char_x, char_y, slice_y, ch;
    uint8_t clocks, clocks2;
#ifdef USE_USB_KEYBOARD
    uint8_t index;
    uint8_t hub_port;
    uint8_t intf_num, in_num;
#endif

    //  uint8_t render_line;

    ntsc_line++;

    // VSYNC/HSYNC slection for next scanline

    if ((ntsc_line == 3) || (ntsc_line == 4) || (ntsc_line == 5)) { // VSYNC : ntsc_line : 4-6
        TIM_SetCompare4(TIM1, NTSC_VSYNC);
        //    TIM1->CH4CVR = NTSC_VSYNC;
    } else {
        TIM_SetCompare4(TIM1, NTSC_HSYNC);
        //    TIM1->CH4CVR = NTSC_HSYNC;
    }

    // Video Out

    if ((ntsc_line >= NTSC_SCAN_START)
            && (ntsc_line < (NTSC_SCAN_START + NTSC_Y_PIXELS))) { // video out
        ntsc_blank = 0;
        DMA_Tx_Init(DMA1_Channel3, (u32) (&SPI1->DATAR + 1),
                (u32) scandata[ntsc_line % 2], NTSC_X_CHARS + 1);
        DMA_Cmd(DMA1_Channel3, ENABLE);
    } else {
        ntsc_blank = 1;
    }

    // Redner fonts for next scanline

    if ((ntsc_line >= NTSC_SCAN_START - 1)
            && (ntsc_line < (NTSC_SCAN_START + NTSC_Y_PIXELS - 1))) {

        char_y = (ntsc_line + 1 - NTSC_SCAN_START) / 8;
        slice_y = (ntsc_line + 1 - NTSC_SCAN_START) % 8;

        if ((via_reg[0] & 0x20) == 0) {         // PCG not used
            for (char_x = 0; char_x < NTSC_X_CHARS; char_x++) {
                ch = vram[char_x + char_y * NTSC_X_CHARS + 0x100];
                if (ch > 0x7f) {
                    scandata[(ntsc_line + 1) % 2][char_x] = ~rom[(ch & 0x7f) * 8
                            + slice_y];
                } else {
                    scandata[(ntsc_line + 1) % 2][char_x] =
                            rom[ch * 8 + slice_y];
                }
            }
        } else {                          // use PCG
            for (char_x = 0; char_x < NTSC_X_CHARS; char_x++) {
                ch = vram[char_x + char_y * NTSC_X_CHARS + 0x100];
                if (ch > 0x7f) {
                    scandata[(ntsc_line + 1) % 2][char_x] = vram[(ch & 0x7f) * 8
                            + slice_y];
                } else {
                    scandata[(ntsc_line + 1) % 2][char_x] =
                            rom[ch * 8 + slice_y];
                }
            }
        }
    }

    if (ntsc_line > 262) {
        ntsc_line = 0;
    }

    // Execute m6800
    // 63.5us/894KHz~57clocks

    if (run_emulation != 0) {

        clocks2 = 0;
        while(clocks2<57) {
            clocks=m6800_execute(1);
            clocks2+=clocks;
            exec6522(clocks);
        }
    }

#ifdef USE_USB_KEYBOARD

        /* USB HID Device Input Endpoint Timing */
        if( RootHubDev.bStatus >= ROOT_DEV_SUCCESS )
        {
            index = RootHubDev.DeviceIndex;
            if( RootHubDev.bType == USB_DEV_CLASS_HID )
            {
                for( intf_num = 0; intf_num < HostCtl[ index ].InterfaceNum; intf_num++ )
                {
                    for( in_num = 0; in_num < HostCtl[ index ].Interface[ intf_num ].InEndpNum; in_num++ )
                    {
                        HostCtl[ index ].Interface[ intf_num ].InEndpTimeCount[ in_num ]++;
                    }
                }
            }
            else if( RootHubDev.bType == USB_DEV_CLASS_HUB )
            {
                HostCtl[ index ].Interface[ 0 ].InEndpTimeCount[ 0 ]++;
                for( hub_port = 0; hub_port < RootHubDev.bPortNum; hub_port++ )
                {
                    if( RootHubDev.Device[ hub_port ].bStatus >= ROOT_DEV_SUCCESS )
                    {
                        index = RootHubDev.Device[ hub_port ].DeviceIndex;

                        if( RootHubDev.Device[ hub_port ].bType == USB_DEV_CLASS_HID )
                        {
                            for( intf_num = 0; intf_num < HostCtl[ index ].InterfaceNum; intf_num++ )
                            {
                                for( in_num = 0; in_num < HostCtl[ index ].Interface[ intf_num ].InEndpNum; in_num++ )
                                {
                                    HostCtl[ index ].Interface[ intf_num ].InEndpTimeCount[ in_num ]++;
                                }
                            }
                        }
                    }
                }
            }
        }

#endif


}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void) {



//  Peripheral setup

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    USART_CFG();
    beep_init();

    video_init();
    video_cls();

#ifdef USE_USB_KEYBOARD
    USBFS_RCC_Init();
    USBFS_Host_Init(ENABLE);
    memset(&RootHubDev.bStatus, 0, sizeof(ROOT_HUB_DEVICE));
    memset(
            &HostCtl[ DEF_USBFS_PORT_INDEX * DEF_ONE_USB_SUP_DEV_TOTAL].InterfaceNum,
            0, DEF_ONE_USB_SUP_DEV_TOTAL * sizeof(HOST_CTL));
#endif

//  Emulator setup

    RAM = malloc(MEM_SIZE);
    for (int i = 0; i < MEM_SIZE; i++) {
        RAM[i] = 0;
    }   // zeroes execute as NOP (as do all undefined instructions)

    m6800_init();
    m6800_reset();

    Delay_Init();


     for (int xx = 0; xx < NTSC_X_CHARS; xx++) {
     cpu_writemem16(xx, 0xaa);
     if (cpu_readmem16(xx) != 0xaa)
     vram[xx + 0x100] = 0x20;
     Delay_Ms(10);
     }


    run_emulation = 1;

    while(1)
    {

        video_wait_vsync();
        USART_Getkey();

#ifdef USE_USB_KEYBOARD

  USBH_MainDeal();

#endif


    }
}