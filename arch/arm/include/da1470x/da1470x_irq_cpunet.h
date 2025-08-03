#TODO

/* External interrupts (vectors ≥ 16) for DA1470x’s Sensor Node Controller (SNC) core */
/* Base offset for all SNC external IRQs */
#define DA1470X_SNC_IRQ_EXTINT      (16)

/* SNC-specific interrupt vectors, per IRQn_Type for Cortex-M0+ */
#define DA1470X_SNC_IRQ_SYS2SNC       (DA1470X_SNC_IRQ_EXTINT+0)   /* System → SNC interrupt */
#define DA1470X_SNC_IRQ_CMAC2SNC      (DA1470X_SNC_IRQ_EXTINT+1)   /* CMAC → SNC interrupt */
#define DA1470X_SNC_IRQ_PDC_SNC       (DA1470X_SNC_IRQ_EXTINT+2)   /* PDC → SNC wake-up interrupt */
#define DA1470X_SNC_IRQ_KEY_WKUP      (DA1470X_SNC_IRQ_EXTINT+3)   /* Key wake-up GPIO interrupt */
#define DA1470X_SNC_IRQ_GPIO_P0       (DA1470X_SNC_IRQ_EXTINT+4)   /* GPIO Port 0 interrupt */
#define DA1470X_SNC_IRQ_GPIO_P1       (DA1470X_SNC_IRQ_EXTINT+5)   /* GPIO Port 1 interrupt */
#define DA1470X_SNC_IRQ_GPIO_P2       (DA1470X_SNC_IRQ_EXTINT+6)   /* GPIO Port 2 interrupt */
#define DA1470X_SNC_IRQ_TIMER         (DA1470X_SNC_IRQ_EXTINT+7)   /* TIMER interrupt */
#define DA1470X_SNC_IRQ_TIMER3        (DA1470X_SNC_IRQ_EXTINT+8)   /* TIMER3 interrupt */
#define DA1470X_SNC_IRQ_TIMER4        (DA1470X_SNC_IRQ_EXTINT+9)   /* TIMER4 interrupt */
#define DA1470X_SNC_IRQ_TIMER5        (DA1470X_SNC_IRQ_EXTINT+10)  /* TIMER5 interrupt */
#define DA1470X_SNC_IRQ_TIMER6        (DA1470X_SNC_IRQ_EXTINT+11)  /* TIMER6 interrupt */
#define DA1470X_SNC_IRQ_RTC           (DA1470X_SNC_IRQ_EXTINT+12)  /* Real-Time Clock interrupt */
#define DA1470X_SNC_IRQ_RTC_EVENT     (DA1470X_SNC_IRQ_EXTINT+13)  /* RTC event interrupt */
#define DA1470X_SNC_IRQ_CAPTIMER      (DA1470X_SNC_IRQ_EXTINT+14)  /* Capture timer interrupt */
#define DA1470X_SNC_IRQ_ADC           (DA1470X_SNC_IRQ_EXTINT+15)  /* ADC interrupt */
#define DA1470X_SNC_IRQ_UART          (DA1470X_SNC_IRQ_EXTINT+16)  /* UART interrupt */
#define DA1470X_SNC_IRQ_UART2         (DA1470X_SNC_IRQ_EXTINT+17)  /* UART2 interrupt */
#define DA1470X_SNC_IRQ_UART3         (DA1470X_SNC_IRQ_EXTINT+18)  /* UART3 interrupt */
#define DA1470X_SNC_IRQ_SPI           (DA1470X_SNC_IRQ_EXTINT+19)  /* SPI interrupt */
#define DA1470X_SNC_IRQ_SPI2          (DA1470X_SNC_IRQ_EXTINT+20)  /* SPI2 interrupt */
#define DA1470X_SNC_IRQ_SPI3          (DA1470X_SNC_IRQ_EXTINT+21)  /* SPI3 interrupt */
#define DA1470X_SNC_IRQ_I2C           (DA1470X_SNC_IRQ_EXTINT+22)  /* I2C interrupt */
#define DA1470X_SNC_IRQ_I2C2          (DA1470X_SNC_IRQ_EXTINT+23)  /* I2C2 interrupt */
#define DA1470X_SNC_IRQ_I2C3          (DA1470X_SNC_IRQ_EXTINT+24)  /* I2C3 interrupt */
#define DA1470X_SNC_IRQ_I3C           (DA1470X_SNC_IRQ_EXTINT+25)  /* I3C interrupt */
#define DA1470X_SNC_IRQ_PCM           (DA1470X_SNC_IRQ_EXTINT+26)  /* PCM interrupt */
#define DA1470X_SNC_IRQ_SRC_IN        (DA1470X_SNC_IRQ_EXTINT+27)  /* SRC input interrupt */
#define DA1470X_SNC_IRQ_SRC_OUT       (DA1470X_SNC_IRQ_EXTINT+28)  /* SRC output interrupt */
#define DA1470X_SNC_IRQ_SRC2_IN       (DA1470X_SNC_IRQ_EXTINT+29)  /* SRC2 input interrupt */
#define DA1470X_SNC_IRQ_SRC2_OUT      (DA1470X_SNC_IRQ_EXTINT+30)  /* SRC2 output interrupt */
#define DA1470X_SNC_IRQ_VAD           (DA1470X_SNC_IRQ_EXTINT+31)  /* Voice Activity Detection interrupt */

#define DA1470X_SNC_IRQ_NEXTINT       (32)                         /* Number of defined external interrupts */
