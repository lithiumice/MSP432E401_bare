
/******************************************************************************
 * 使用MSP432E4演示OLED128x64屏的基本显示。
 * 本项目使用的板卡为 MSP_EXP432E401Y ，使用的CCS IDE版本为 CCS 10.1.1
 * 程序下载后按板载的 USR_SW1 按鍵切换显示内容
 * 本程序使用的是SSD1306主控的OLED128x64屏，使用软件SPI方式驱动
 * 引脚连接如下所示，OLED引脚的配置在oled.h文件中
 *                MSP432E401Y
 *             ------------------
 *         /|\|               PE4|--> OLED_SCLK
 *          | |               PC4|--> OLED_DAT
 *          --|RST            PC5|--> OLED_RES
 *            |               PC6|--> OLED_DC
 *            |               PE5|--> OLED_CS
 *            |                  |
 *            |                  |
 *            |               PJ0|<-- USR_SW1
 *            |                  |
 *            |               PN0|--> LED D2
 *            |               PN1|--> LED D1
 *******************************************************************************/

#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "oled.h"
#include "bmp.h"

volatile uint32_t gpioState = 0;
uint8_t Trg = 0;

void GPIOInit(void);
void SysTick_Handler(void);
void GPIOJ_IRQHandler(void);
int main(void)
{
    uint32_t systemClock;
    /* 将系统时钟配置为16 MHz */
    systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
    SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                         16000000);
    /* 初始化GPIO */
    GPIOInit();

    /* 启用SysTick计时器以每1/20秒生成一次中断，用于控制LED2亮灭 */
    MAP_SysTickPeriodSet(systemClock / 20);
    MAP_SysTickIntEnable();
    MAP_SysTickEnable();

    OLED_Init(); //初始化OLED
    OLED_Clear();//OLED清屏

    while (1)
    {
        OLED_Clear();
        OLED_ShowString(48,2,"2021");
        while (!Trg); //等待中断触发，按键按下
        Trg = 0;

        OLED_Clear();
        OLED_ShowString(48,2,"8.4");
        while (!Trg);
        Trg = 0;

        OLED_Clear();
        OLED_ShowString(16,2,"NUEDC Contest");
        while (!Trg);
        Trg = 0;

        OLED_Clear();
        OLED_ShowCHinese(0,2,0);      //全
        OLED_ShowCHinese(18,2,1);     //国
        OLED_ShowCHinese(36,2,2);     //大
        OLED_ShowCHinese(54,2,3);     //学
        OLED_ShowCHinese(72,2,4);     //生
        OLED_ShowCHinese(90,2,5);     //电
        OLED_ShowCHinese(108,2,6);    //子
        OLED_ShowCHinese(0,4,7);      //设
        OLED_ShowCHinese(18,4,8);     //计
        OLED_ShowCHinese(36,4,9);     //竞
        OLED_ShowCHinese(54,4,10);    //赛
        OLED_ShowCHinese(72,4,11);    //培
        OLED_ShowCHinese(90,4,12);    //训
        OLED_ShowCHinese(108,4,13);   //网
        while (!Trg);
        Trg = 0;

        OLED_Clear();
        OLED_DrawBMP(0,0,128,8,BMP1);  //1电赛徽章图片显示
        while (!Trg);
        Trg = 0;

        OLED_Clear();
        OLED_DrawBMP(0,0,128,8,BMP4);  //4TI标志图片显示
        while (!Trg);
        Trg = 0;

        OLED_Clear();
        OLED_ShowCHinese(0,2,14);      //欢
        OLED_ShowCHinese(18,2,15);     //迎
        OLED_ShowCHinese(36,2,16);     //大
        OLED_ShowCHinese(54,2,17);     //家
        OLED_ShowCHinese(72,2,18);     //报
        OLED_ShowCHinese(90,2,19);     //名
        OLED_ShowCHinese(0,4,20);      //参
        OLED_ShowCHinese(18,4,21);     //加
        OLED_ShowCHinese(36,4,22);     //电
        OLED_ShowCHinese(54,4,23);     //赛
        OLED_ShowCHinese(72,4,24);     //！
        while (!Trg);
        Trg = 0;

        OLED_Clear();
        OLED_ShowString(8,0,"https://www.");
        OLED_ShowString(8,2,"nuedc-training.");
        OLED_ShowString(8,4,"com.cn/");
        while (!Trg);
        Trg = 0;

    }
}
//Gpio初始化程序
void GPIOInit(void)
{
    /* 启用GPIO端口的时钟并等待其准备就绪 */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while (!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)))
        ;
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)))
        ;
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while (!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)))
        ;

    /*启用OLED引脚连接的端口的时钟并等待其准备就绪*/
    MAP_SysCtlPeripheralEnable(GPIO_OLED_CS_PORT_SYSCTL);
    while (!(SysCtlPeripheralReady(GPIO_OLED_CS_PORT_SYSCTL)))
        ;

    MAP_SysCtlPeripheralEnable(GPIO_OLED_DC_PORT_SYSCTL);
    while (!(SysCtlPeripheralReady(GPIO_OLED_DC_PORT_SYSCTL)))
        ;

    MAP_SysCtlPeripheralEnable(GPIO_OLED_RES_PORT_SYSCTL);
    while (!(SysCtlPeripheralReady(GPIO_OLED_RES_PORT_SYSCTL)))
        ;

    MAP_SysCtlPeripheralEnable(GPIO_OLED_DAT_PORT_SYSCTL);
    while (!(SysCtlPeripheralReady(GPIO_OLED_DAT_PORT_SYSCTL)))
        ;

    MAP_SysCtlPeripheralEnable(GPIO_OLED_SCLK_PORT_SYSCTL);
    while (!(SysCtlPeripheralReady(GPIO_OLED_SCLK_PORT_SYSCTL)))
        ;

    /* 配置板载LED指示灯GPIO PIN作为输出 */
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, (GPIO_PIN_0 | GPIO_PIN_1));
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, (GPIO_PIN_0 | GPIO_PIN_1), 0);
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_4));
    MAP_GPIOPinWrite(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_4), 0);

    /*将OLED的引脚配置为输出*/
    MAP_GPIOPinTypeGPIOOutput(GPIO_OLED_CS_PORT_BASE, GPIO_OLED_CS_PIN);
    MAP_GPIOPinWrite(GPIO_OLED_CS_PORT_BASE, GPIO_OLED_CS_PIN, 0);

    MAP_GPIOPinTypeGPIOOutput(GPIO_OLED_DC_PORT_BASE, GPIO_OLED_DC_PIN);
    MAP_GPIOPinWrite(GPIO_OLED_DC_PORT_BASE, GPIO_OLED_DC_PIN, 0);

    MAP_GPIOPinTypeGPIOOutput(GPIO_OLED_RES_PORT_BASE, GPIO_OLED_RES_PIN);
    MAP_GPIOPinWrite(GPIO_OLED_RES_PORT_BASE, GPIO_OLED_RES_PIN, 0);

    MAP_GPIOPinTypeGPIOOutput(GPIO_OLED_DAT_PORT_BASE, GPIO_OLED_DAT_PIN);
    MAP_GPIOPinWrite(GPIO_OLED_DAT_PORT_BASE, GPIO_OLED_DAT_PIN, 0);

    MAP_GPIOPinTypeGPIOOutput(GPIO_OLED_SCLK_PORT_BASE, GPIO_OLED_SCLK_PIN);
    MAP_GPIOPinWrite(GPIO_OLED_SCLK_PORT_BASE, GPIO_OLED_SCLK_PIN, 0);

    //MAP_GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, (GPIO_PIN_0 | GPIO_PIN_1));

    /* 将GPIO PJ0配置为输入并启用内部上拉功能。 配置PJ0进行双向中断检测*/
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);
    GPIOJ->PUR |= GPIO_PIN_0;
    MAP_GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_BOTH_EDGES);
    MAP_GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
    MAP_IntEnable(INT_GPIOJ); //开启GPIOJ中断
}
//系统定时器中断处理程序，用于控制LED2指示灯
void SysTick_Handler(void)
{
    if (gpioState != 3)
    {
        gpioState++;
    }
    else
    {
        gpioState = 0;
    }
    /* 将GPIO的新值写入端口引脚。 API将生成位带区地址，以写入特定的端口引脚*/
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, gpioState); //LED2亮灭

}
//GPIOJ中断处理，按键SW1触发
void GPIOJ_IRQHandler(void)
{
    uint32_t getIntStatus;
    /* 从GPIO获取中断状态并清除状态 */
    getIntStatus = MAP_GPIOIntStatus(GPIO_PORTJ_BASE, true);
    if ((getIntStatus & GPIO_PIN_0) == GPIO_PIN_0)
    {
        MAP_GPIOIntClear(GPIO_PORTJ_BASE, getIntStatus);
        /* 根据引脚的当前状态置位Trg*/
        if (MAP_GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) == GPIO_PIN_0)
            Trg = 1;
    }
}
