
#include <cla_shared_cpu1.h>
#include <F28x_Project.h>
#include <F2837xD_Ipc_drivers.h>
#include <stdint.h>

#include "UART_Driver.h"
#include "AIC23.h"
#include "AIC23_Init.h"
#include "LED_PB_SW_Driver.h"

#define SYS_CLK_MHZ 200

#define UART_START_BYTE 0xFF
#define UART_ACK_BYTE 'A'

#define HASS_DELAY_LINE_SIZE 2401 // 48000 * (50/1000) + 1, max of 50ms delay
float hass_delay_line[HASS_DELAY_LINE_SIZE];
uint16_t hass_delay_line_idx = 0;
uint16_t hass_delay_offset = 0;
float hass_duration_ms = 0;

#pragma DATA_SECTION(h1,"CLAData")
IIR_FILTER h1 = {.taps = {{0.000008, 0.000016, 0.000008, 1.000000, -1.998173, 0.998206},
                               {0.000008, 0.000016, 0.000008, 1.000000, -1.994770, 0.994803},
                               {0.000008, 0.000016, 0.000008, 1.000000, -1.991884, 0.991917},
                               {0.000008, 0.000016, 0.000008, 1.000000, -1.989792, 0.989825},
                               {0.000008, 0.000016, 0.000008, 1.000000, -1.988694, 0.988727}}};

#pragma DATA_SECTION(h2,"CLAData")
IIR_FILTER h2 = {.taps = {{0.002913, 0.000000, -0.002913, 1.000000, -1.997491, 0.997623},
                               {0.002913, 0.000000, -0.002913, 1.000000, -1.998741, 0.998776},
                               {0.002909, 0.000000, -0.002909, 1.000000, -1.994195, 0.994300},
                               {0.002909, 0.000000, -0.002909, 1.000000, -1.996243, 0.996287},
                               {0.002908, 0.000000, -0.002908, 1.000000, -1.994117, 0.994185}}};

#pragma DATA_SECTION(h3,"CLAData")
IIR_FILTER h3 = {.taps = {{0.005776, 0.000000, -0.005776, 1.000000, -1.994771, 0.995290},
                               {0.005776, 0.000000, -0.005776, 1.000000, -1.997435, 0.997572},
                               {0.005759, 0.000000, -0.005759, 1.000000, -1.988313, 0.988722},
                               {0.005759, 0.000000, -0.005759, 1.000000, -1.992472, 0.992646},
                               {0.005753, 0.000000, -0.005753, 1.000000, -1.988228, 0.988495}}};

#pragma DATA_SECTION(h4,"CLAData")
IIR_FILTER h4 = {.taps = {{0.011530, 0.000000, -0.011530, 1.000000, -1.988533, 0.990603},
                               {0.011530, 0.000000, -0.011530, 1.000000, -1.994600, 0.995150},
                               {0.011465, 0.000000, -0.011465, 1.000000, -1.975944, 0.977571},
                               {0.011465, 0.000000, -0.011465, 1.000000, -1.984653, 0.985345},
                               {0.011440, 0.000000, -0.011440, 1.000000, -1.976062, 0.977120}}};

#pragma DATA_SECTION(h5,"CLAData")
IIR_FILTER h5 = {.taps = {{0.022978, 0.000000, -0.022978, 1.000000, -1.973066, 0.981304},
                               {0.022978, 0.000000, -0.022978, 1.000000, -1.988127, 0.990322},
                               {0.022720, 0.000000, -0.022720, 1.000000, -1.949213, 0.955647},
                               {0.022720, 0.000000, -0.022720, 1.000000, -1.968149, 0.970896},
                               {0.022624, 0.000000, -0.022624, 1.000000, -1.950566, 0.954752}}};

#pragma DATA_SECTION(h6,"CLAData")
IIR_FILTER h6 = {.taps = {{0.045623, 0.000000, -0.045623, 1.000000, -1.930444, 0.963025},
                               {0.045623, 0.000000, -0.045623, 1.000000, -1.971986, 0.980716},
                               {0.044632, 0.000000, -0.044632, 1.000000, -1.888132, 0.913282},
                               {0.044632, 0.000000, -0.044632, 1.000000, -1.931749, 0.942572},
                               {0.044270, 0.000000, -0.044270, 1.000000, -1.895096, 0.911461}}};

#pragma DATA_SECTION(h7,"CLAData")
IIR_FILTER h7 = {.taps = {{0.089920, 0.000000, -0.089920, 1.000000, -1.800973, 0.927945},
                               {0.089920, 0.000000, -0.089920, 1.000000, -1.927131, 0.961639},
                               {0.086242, 0.000000, -0.086242, 1.000000, -1.738240, 0.834239},
                               {0.086242, 0.000000, -0.086242, 1.000000, -1.845942, 0.887928},
                               {0.084952, 0.000000, -0.084952, 1.000000, -1.767558, 0.830095}}};

#pragma DATA_SECTION(h8,"CLAData")
IIR_FILTER h8 = {.taps = {{0.174589, 0.000000, -0.174589, 1.000000, -1.389359, 0.865057},
                               {0.174589, 0.000000, -0.174589, 1.000000, -1.789258, 0.923448},
                               {0.161910, 0.000000, -0.161910, 1.000000, -1.627206, 0.784649},
                               {0.161910, 0.000000, -0.161910, 1.000000, -1.348507, 0.697201},
                               {0.157748, 0.000000, -0.157748, 1.000000, -1.456221, 0.684504}}};

#pragma DATA_SECTION(h9,"CLAData")
IIR_FILTER h9 = {.taps = {{0.329196, 0.000000, -0.329196, 1.000000, -1.345449, 0.841947},
                               {0.329196, 0.000000, -0.329196, 1.000000, -0.188873, 0.777890},
                               {0.291067, 0.000000, -0.291067, 1.000000, -1.047034, 0.588664},
                               {0.291067, 0.000000, -0.291067, 1.000000, -0.360327, 0.499053},
                               {0.279645, 0.000000, -0.279645, 1.000000, -0.686184, 0.440711}}};

#pragma DATA_SECTION(h10,"CLAData")
IIR_FILTER h10 = {.taps = {{0.590011, 0.000000, -0.590011, 1.000000, 1.877351, 0.908749},
                               {0.590011, 0.000000, -0.590011, 1.000000, -0.123640, 0.582463},
                               {0.502106, 0.000000, -0.502106, 1.000000, 1.708580, 0.742893},
                               {0.502106, 0.000000, -0.502106, 1.000000, 0.014155, 0.144786},
                               {0.477582, 0.000000, -0.477582, 1.000000, 0.857492, 0.044836}}};

#pragma DATA_SECTION(gains_cla,"CLAData");
float gains_cla[NUM_BANDS];
#pragma DATA_SECTION(output_sample_left,"CLAData");
float output_sample_left;
#pragma DATA_SECTION(input_sample_left,"CLAData");
float input_sample_left;

// stores gain values
#pragma DATA_SECTION(gains, "GainVals");
float gains[NUM_BANDS] = {1,1,1,1,1,1,1,1,1,1};

// right channel input, to be processed on CPU2
#pragma DATA_SECTION(input_sample_right,"SampleToCPU2");
float input_sample_right;

// right channel output, already processed by CPU2
#pragma DATA_SECTION(output_sample_right,"SampleFromCPU2");
float output_sample_right;

typedef enum Mode { OFF, RUNNING, SCOPE } Mode;
volatile Mode mode = RUNNING;

bool equalizer_running = true;

interrupt void McBSPbRxISR(void) {
    EALLOW;

    // sample the left and right channel
    int16_t left = McbspbRegs.DRR2.all;
    int16_t right = McbspbRegs.DRR1.all;

    // handle left channel processing by CLA1
    input_sample_left = (float)left;
    memcpy((uint32_t *)&gains_cla, (uint32_t *)&gains,
           (uint32_t)NUM_BANDS * sizeof(gains[0]));
    Cla1ForceTask1();

    // handle right channel processing by delegating to CPU2
    input_sample_right = (float)right;
    IpcRegs.IPCSET.bit.IPC0 = 1;

    PieCtrlRegs.PIEACK.all |=
        PIEACK_GROUP6;  // allow PIE group 6 interrupt to fire again
}

interrupt void Cla1Task1CompleteISR() {
    // drive GPIO32 high
    GpioDataRegs.GPBSET.bit.GPIO32 = 1;

    // Wait for IPC31 from CPU2
    while (!IpcRegs.IPCSTS.bit.IPC31);
    IpcRegs.IPCACK.bit.IPC31 = 1;

    if (equalizer_running) {
        hass_delay_line[hass_delay_line_idx] = output_sample_right;
    } else {
        hass_delay_line[hass_delay_line_idx] = input_sample_right;
    }

    uint16_t hass_offset_idx = hass_delay_line_idx - hass_delay_offset;
    // If the number overflows 0->BIG
    if (hass_offset_idx > HASS_DELAY_LINE_SIZE) {
        hass_offset_idx = HASS_DELAY_LINE_SIZE - (0xFFFF - hass_offset_idx) - 1;
    }

    int16_t filter_output_left = (int16_t)output_sample_left;
    int16_t filter_output_right = (int16_t)hass_delay_line[hass_offset_idx];

    if (!equalizer_running) {
        filter_output_left = (int16_t)input_sample_left;
    }

    hass_delay_line_idx = (hass_delay_line_idx + 1) % HASS_DELAY_LINE_SIZE;

    // output filtered samples to DAC
    McbspbRegs.DXR2.all = filter_output_left;
    McbspbRegs.DXR1.all = filter_output_right;

    // drive GPIO32 low
    GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;

    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP11;
}

void CLA_configClaMemory(void) {
    extern uint32_t Cla1funcsRunStart, Cla1funcsLoadStart, Cla1funcsLoadSize;

    EALLOW;
#ifdef _FLASH
    // Copy over code from FLASH to RAM
    memcpy((uint32_t *)&Cla1funcsRunStart, (uint32_t *)&Cla1funcsLoadStart,
           (uint32_t)&Cla1funcsLoadSize);
#endif //_FLASH

    // Enable CPU1 clocking at the sys clock level
    CpuSysRegs.PCLKCR0.bit.CLA1 = 1;

    // Initialize and wait for CLA1ToCPUMsgRAM
    MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

    // Initialize and wait for CPUToCLA1MsgRAM
    MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

    // Select RAMLS5 as the programming space for the CLA
    MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1;

    // Select RAMLS4 as the data space for the CLA
    MemCfgRegs.LSxMSEL.bit.MSEL_LS4 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS4 = 0;
}

void CLA_initCpuXCla1(void) {
    EALLOW;

    // Enable CPU1.CLA1
    DevCfgRegs.DC1.bit.CPU1_CLA1 = 1;

    // Enable CPU2.CLA1
    DevCfgRegs.DC1.bit.CPU2_CLA1 = 1;

    //Send flag 5 to CPU2
   IpcSync(5);
}

void CLA_initCpu1Cla1(void) {
    EALLOW;
    Cla1Regs.MVECT1 = (uint16_t)(&Cla1Task1);

    // Enable the IACK instruction to start a task on CLA in software
    // Enable CLA1 Task1
    Cla1Regs.MCTL.bit.IACKE = 1;
    Cla1Regs.MIER.all = 0x0001;

    PieVectTable.CLA1_1_INT = &Cla1Task1CompleteISR;

    // Enable CLA interrupts at the group and subgroup levels
    PieCtrlRegs.PIEIER11.all = 0xFFFF;
    IER |= M_INT11;
}

void main(void) {
    // initialize system clocks and get board speed running at 200 MHz
    InitSysCtrl();

#define _STANDALONE

#ifndef _STANDALONE
#ifdef _FLASH
//
// Send boot command to allow the CPU2 application to begin execution
//
    IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
#else
//
// Send boot command to allow the CPU2 application to begin execution
//
    IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_RAM);
#endif
#endif

    InitPieCtrl(); // Initialize PIE (disable PIE and clear PIE registers)
    IER = 0x0000; // clear CPU interrupt register
    IFR = 0x0000; // clear CPU interrupt flag register
    InitPieVectTable(); // initialize PIE vector table to known state

    InitSPIA();
    InitMcBSPb(DSP_MODE);
    InitAIC23(DSP_MODE, KHZ_48);

    EALLOW;
    PieVectTable.MCBSPB_RX_INT =
        &McBSPbRxISR;                   // assign McBSPb ISR to PIE vector table
    PieCtrlRegs.PIEIER6.bit.INTx7 = 1;  // enable INT 6.7 in PIE
    IER |= M_INT6;                      // enable INT6 interrupt in CPU

    CLA_initCpuXCla1(); // Enable CLA1 on CPU1 and CPU2

    CLA_configClaMemory();
    CLA_initCpu1Cla1();

    EnableInterrupts();                 // enable PIE and CPU interrupts
    ERTM;  // Enable higher priority real-time debug events

    EALLOW;
    // GPIO32 GMUX and MUX
    GpioCtrlRegs.GPBGMUX1.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;
    // GPIO32 Output
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
    // GPIO32 pull-up off
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 1;
    // GPIO32 off by default
    GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;

    uart_init(921600);

    // Give CPU2 control over RAMGS1
    MemCfgRegs.GSxMSEL.bit.MSEL_GS1 = 1;

    while (true) {
        if (uart_recv_block() == UART_START_BYTE) {
            uint16_t command = uart_recv_block();
            if (command <= 9) {
                // change equalizer band gain
                gains[command] = uart_recv_float_block();
            }
            else if (command == 10) {
                // change hass effect duration
                hass_duration_ms = uart_recv_float_block();
                hass_delay_offset = ((float)hass_duration_ms / 1000) * 48000;
            }
            else if (command == 20) {
                // turn on/off equalizer
                equalizer_running = uart_recv_block() > 0;
            }
            else if (command == 30) {
                // output equalizer band multiply ratio (converted back into dB by Python GUI)
                uart_send_float_array(gains, NUM_BANDS);
            }
            else if (command == 40) {
                // output equalizer on/off status
                uart_send(equalizer_running);
            }
            else if (command == 50) {
                uart_send_float(hass_duration_ms);
            }
            // command == 255 is reserved for checking connection
            uart_send(UART_ACK_BYTE);
        }
    }
}
