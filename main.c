
/******************************************************************************
 * ʹ��MSP432E4��ʾOLED128x64���Ļ�����ʾ��
 * ����Ŀʹ�õİ忨Ϊ MSP_EXP432E401Y ��ʹ�õ�CCS IDE�汾Ϊ CCS 10.1.1
 * �������غ󰴰��ص� USR_SW1 ���I�л���ʾ����
 * ������ʹ�õ���SSD1306���ص�OLED128x64����ʹ�����SPI��ʽ����
 * ��������������ʾ��OLED���ŵ�������oled.h�ļ���
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
    /* ��ϵͳʱ������Ϊ16 MHz */
    systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
    SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                         16000000);
    /* ��ʼ��GPIO */
    GPIOInit();

    /* ����SysTick��ʱ����ÿ1/20������һ���жϣ����ڿ���LED2���� */
    MAP_SysTickPeriodSet(systemClock / 20);
    MAP_SysTickIntEnable();
    MAP_SysTickEnable();

    OLED_Init(); //��ʼ��OLED
    OLED_Clear();//OLED����

    while (1)
    {
        OLED_Clear();
        OLED_ShowString(48,2,"2021");
        while (!Trg); //�ȴ��жϴ�������������
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
        OLED_ShowCHinese(0,2,0);      //ȫ
        OLED_ShowCHinese(18,2,1);     //��
        OLED_ShowCHinese(36,2,2);     //��
        OLED_ShowCHinese(54,2,3);     //ѧ
        OLED_ShowCHinese(72,2,4);     //��
        OLED_ShowCHinese(90,2,5);     //��
        OLED_ShowCHinese(108,2,6);    //��
        OLED_ShowCHinese(0,4,7);      //��
        OLED_ShowCHinese(18,4,8);     //��
        OLED_ShowCHinese(36,4,9);     //��
        OLED_ShowCHinese(54,4,10);    //��
        OLED_ShowCHinese(72,4,11);    //��
        OLED_ShowCHinese(90,4,12);    //ѵ
        OLED_ShowCHinese(108,4,13);   //��
        while (!Trg);
        Trg = 0;

        OLED_Clear();
        OLED_DrawBMP(0,0,128,8,BMP1);  //1��������ͼƬ��ʾ
        while (!Trg);
        Trg = 0;

        OLED_Clear();
        OLED_DrawBMP(0,0,128,8,BMP4);  //4TI��־ͼƬ��ʾ
        while (!Trg);
        Trg = 0;

        OLED_Clear();
        OLED_ShowCHinese(0,2,14);      //��
        OLED_ShowCHinese(18,2,15);     //ӭ
        OLED_ShowCHinese(36,2,16);     //��
        OLED_ShowCHinese(54,2,17);     //��
        OLED_ShowCHinese(72,2,18);     //��
        OLED_ShowCHinese(90,2,19);     //��
        OLED_ShowCHinese(0,4,20);      //��
        OLED_ShowCHinese(18,4,21);     //��
        OLED_ShowCHinese(36,4,22);     //��
        OLED_ShowCHinese(54,4,23);     //��
        OLED_ShowCHinese(72,4,24);     //��
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
//Gpio��ʼ������
void GPIOInit(void)
{
    /* ����GPIO�˿ڵ�ʱ�Ӳ��ȴ���׼������ */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while (!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)))
        ;
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)))
        ;
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while (!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)))
        ;

    /*����OLED�������ӵĶ˿ڵ�ʱ�Ӳ��ȴ���׼������*/
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

    /* ���ð���LEDָʾ��GPIO PIN��Ϊ��� */
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, (GPIO_PIN_0 | GPIO_PIN_1));
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, (GPIO_PIN_0 | GPIO_PIN_1), 0);
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_4));
    MAP_GPIOPinWrite(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_4), 0);

    /*��OLED����������Ϊ���*/
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

    /* ��GPIO PJ0����Ϊ���벢�����ڲ��������ܡ� ����PJ0����˫���жϼ��*/
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);
    GPIOJ->PUR |= GPIO_PIN_0;
    MAP_GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_BOTH_EDGES);
    MAP_GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
    MAP_IntEnable(INT_GPIOJ); //����GPIOJ�ж�
}
//ϵͳ��ʱ���жϴ���������ڿ���LED2ָʾ��
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
    /* ��GPIO����ֵд��˿����š� API������λ������ַ����д���ض��Ķ˿�����*/
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, gpioState); //LED2����

}
//GPIOJ�жϴ�������SW1����
void GPIOJ_IRQHandler(void)
{
    uint32_t getIntStatus;
    /* ��GPIO��ȡ�ж�״̬�����״̬ */
    getIntStatus = MAP_GPIOIntStatus(GPIO_PORTJ_BASE, true);
    if ((getIntStatus & GPIO_PIN_0) == GPIO_PIN_0)
    {
        MAP_GPIOIntClear(GPIO_PORTJ_BASE, getIntStatus);
        /* �������ŵĵ�ǰ״̬��λTrg*/
        if (MAP_GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) == GPIO_PIN_0)
            Trg = 1;
    }
}
