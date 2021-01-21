#ifndef AIC23_INIT_H
#define AIC23_INIT_H

#include <stdbool.h>

typedef enum OperatingMode {I2S_MODE, DSP_MODE} OperatingMode;
typedef enum SampleRate {KHZ_8, KHZ_32, KHZ_48} SampleRate;


inline void CodecSPI_Set_CS(Uint16 val) {
    EALLOW;
    if (val) GpioDataRegs.GPASET.bit.GPIO19 = 1;
    else GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;
}

void InitSPIA(void) {
    /* Init GPIO pins for SPIA */

    // enable pullups for each pin
    // set to async qualification
    // configure each mux

    /*
     *SPISTEA -> GPIO19
     *SPISIMOA -> GPIO58
     *SPICLKA -> GPIO18
     */

    EALLOW;

    // enable pullups
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0;

    // configure GMUX
    GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 0;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 0;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO58 = 3;

    // configure MUX
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 3;

    // async qualification
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3;

    /* Init SPI peripheral */
    SpiaRegs.SPICCR.all = 0x5F; // CLKPOL = 0, SOMI = SIMO (loopback), 16 bit characters
    SpiaRegs.SPICTL.all = 0x06; // master mode, enable transmissions
    SpiaRegs.SPIBRR.all = 50; // gives baud rate of approx 850 kHz

    SpiaRegs.SPICCR.bit.SPISWRESET = 1;
    SpiaRegs.SPIPRI.bit.FREE = 1;
}

static inline Uint16 SPIA_Transmit(Uint16 transmit_data) {
    // transmit 16-bit data
    SpiaRegs.SPITXBUF = transmit_data;
    // wait until data transmitted
    while (!SpiaRegs.SPISTS.bit.INT_FLAG);
    return SpiaRegs.SPIRXBUF; // reset flag
}

void InitAIC23(OperatingMode om, SampleRate sr) {
    CodecSPI_Set_CS(0);

    SmallDelay();
    SPIA_Transmit(reset());
    SmallDelay();
    SPIA_Transmit(softpowerdown()); // power down everything except device and clocks
    SmallDelay();
    SPIA_Transmit(linput_volctl(LIV)); // unmute left line input and maintain default volume
    SmallDelay();
    SPIA_Transmit(rinput_volctl(RIV)); // unmute right line input and maintain default volume
    SmallDelay();
    SPIA_Transmit(lhp_volctl(LHV)); // left headphone volume control
    SmallDelay();
    SPIA_Transmit(rhp_volctl(RHV)); // right headphone volume control
    SmallDelay();
    SPIA_Transmit(nomicaaudpath()); // turn on DAC, mute mic
    SmallDelay();
    SPIA_Transmit(digaudiopath()); // disable DAC mute, add de-emph
    SmallDelay();


    switch (om) {
        case I2S_MODE: {
            // I2S
            SPIA_Transmit(I2Sdigaudinterface());
            break;
        }
        case DSP_MODE: {
            // DSP
            SPIA_Transmit(DSPdigaudinterface());
            break;
        }
        default:
            break;
    }

    SmallDelay();
    switch (sr) {
        case KHZ_8: {
            SPIA_Transmit(CLKsampleratecontrol(SR8));
            break;
        }
        case KHZ_32: {
            SPIA_Transmit(CLKsampleratecontrol(SR32));
            break;
        }
        case KHZ_48: {
            SPIA_Transmit(CLKsampleratecontrol(SR48));
            break;
        }
        default:
            break;
    }
    SmallDelay();

    SPIA_Transmit(digact()); // activate digital interface
    SmallDelay();
    SPIA_Transmit(nomicpowerup()); // turn everything on except mic

    CodecSPI_Set_CS(1);
}

void InitMcBSPb(OperatingMode om) {
    /* Init McBSPb GPIO Pins */

    //modify the GPxMUX, GPxGMUX, GPxQSEL
    //all pins should be set to async qualification

    /*
     * MDXB -> GPIO24
     * MDRB -> GPIO25
     * MCLKRB -> GPIO60
     * MCLKXB -> GPIO26
     * MFSRB -> GPIO61
     * MFSXB -> GPIO27
     */

    EALLOW;

    // MDXB -> GPIO24 (GPIOA)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO24 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 3;

    // MDRB -> GPIO25 (GPIOA)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO25 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 3;

    // MFSRB -> GPIO61 (GPIOB)
    GpioCtrlRegs.GPBGMUX2.bit.GPIO61 = 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 1;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3;

    // MFSXB -> GPIO27 (GPIOA)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO27 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 3;

    // MCLKRB -> GPIO60 (GPIOB)
    GpioCtrlRegs.GPBGMUX2.bit.GPIO60 = 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 1;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3;

    // MCLKXB -> GPIO26 (GPIOA)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO26 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 3;

    /* Init McBSPb for I2S mode */
    EALLOW;
    McbspbRegs.SPCR2.all = 0; // Reset FS generator, sample rate generator & transmitter
    McbspbRegs.SPCR1.all = 0; // Reset Receiver, Right justify word
    McbspbRegs.SPCR1.bit.RJUST = 2; // left-justify word in DRR and zero-fill LSBs
    McbspbRegs.MFFINT.all=0x0; // Disable all interrupts
    McbspbRegs.SPCR1.bit.RINTM = 0; // McBSP interrupt flag - RRDY
    McbspbRegs.SPCR2.bit.XINTM = 0; // McBSP interrupt flag - XRDY
    // Clear Receive Control Registers
    McbspbRegs.RCR2.all = 0x0;
    McbspbRegs.RCR1.all = 0x0;
    // Clear Transmit Control Registers
    McbspbRegs.XCR2.all = 0x0;
    McbspbRegs.XCR1.all = 0x0;
    if (om == I2S_MODE) {
        // Set Receive/Transmit to 32-bit operation
        McbspbRegs.RCR2.bit.RWDLEN2 = 5;
        McbspbRegs.RCR1.bit.RWDLEN1 = 5;
        McbspbRegs.XCR2.bit.XWDLEN2 = 5;
        McbspbRegs.XCR1.bit.XWDLEN1 = 5;
        McbspbRegs.RCR2.bit.RPHASE = 1; // Dual-phase frame for receive
        McbspbRegs.RCR1.bit.RFRLEN1 = 0; // Receive frame length = 1 word in phase 1
        McbspbRegs.RCR2.bit.RFRLEN2 = 0; // Receive frame length = 1 word in phase 2
        McbspbRegs.XCR2.bit.XPHASE = 1; // Dual-phase frame for transmit
        McbspbRegs.XCR1.bit.XFRLEN1 = 0; // Transmit frame length = 1 word in phase 1
        McbspbRegs.XCR2.bit.XFRLEN2 = 0; // Transmit frame length = 1 word in phase 2
        // I2S mode: R/XDATDLY = 1 always
        McbspbRegs.RCR2.bit.RDATDLY = 1;
        McbspbRegs.XCR2.bit.XDATDLY = 1;
    }
    else if (om == DSP_MODE) {
        // Set Receive/Transmit to 32-bit operation
        McbspbRegs.RCR2.bit.RWDLEN2 = 0;
        McbspbRegs.RCR1.bit.RWDLEN1 = 5;
        McbspbRegs.XCR2.bit.XWDLEN2 = 0;
        McbspbRegs.XCR1.bit.XWDLEN1 = 5;
        McbspbRegs.RCR2.bit.RPHASE = 0; // Single-phase frame for receive
        McbspbRegs.RCR1.bit.RFRLEN1 = 0; // Receive frame length = 1 word in phase 1
        McbspbRegs.RCR2.bit.RFRLEN2 = 0; // Receive frame length = 1 word in phase 2
        McbspbRegs.XCR2.bit.XPHASE = 0; // Single-phase frame for transmit
        McbspbRegs.XCR1.bit.XFRLEN1 = 0; // Transmit frame length = 1 word in phase 1
        McbspbRegs.XCR2.bit.XFRLEN2 = 0; // Transmit frame length = 1 word in phase 2
        // DSP mode: R/XDATDLY = 0 always
        McbspbRegs.RCR2.bit.RDATDLY = 0;
        McbspbRegs.XCR2.bit.XDATDLY = 0;
    }

    // Frame Width = 1 CLKG period, CLKGDV must be 1 as slave
    McbspbRegs.SRGR1.all = 0x0001;
    McbspbRegs.PCR.all=0x0000;
    // Transmit frame synchronization is supplied by an external source via the FSX pin
    McbspbRegs.PCR.bit.FSXM = 0;
    // Receive frame synchronization is supplied by an external source via the FSR pin
    McbspbRegs.PCR.bit.FSRM = 0;
    // Select sample rate generator to be signal on MCLKR pin
    McbspbRegs.PCR.bit.SCLKME = 1;
    McbspbRegs.SRGR2.bit.CLKSM = 0;
    // Receive frame-synchronization pulses are active low - (L-channel first)
    McbspbRegs.PCR.bit.FSRP = 1;
    // Transmit frame-synchronization pulses are active low - (L-channel first)
    McbspbRegs.PCR.bit.FSXP = 1;
    // Receive data is sampled on the rising edge of MCLKR
    McbspbRegs.PCR.bit.CLKRP = 1;
    // Transmit data is sampled on the rising edge of CLKX
    McbspbRegs.PCR.bit.CLKXP = 1;
    // The transmitter gets its clock signal from MCLKX
    McbspbRegs.PCR.bit.CLKXM = 0;
    // The receiver gets its clock signal from MCLKR
    McbspbRegs.PCR.bit.CLKRM = 0;
    // Enable Receive Interrupt
    McbspbRegs.MFFINT.bit.RINT = 1;
    // Ignore unexpected frame sync
    //McbspbRegs.XCR2.bit.XFIG = 1;
    McbspbRegs.SPCR2.all |=0x00C0; // Frame sync & sample rate generators pulled out of reset

    DELAY_US(5E3);

    McbspbRegs.SPCR2.bit.XRST=1; // Enable Transmitter
    McbspbRegs.SPCR1.bit.RRST=1; // Enable Receiver
}

#endif // AIC23_INIT_H
