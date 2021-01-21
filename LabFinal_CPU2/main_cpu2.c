#include <cla_shared_cpu2.h>
#include <F28x_Project.h>
#include <F2837xD_Ipc_drivers.h>
#include <stdint.h>


#define WAITSTEP     asm(" RPT #255 || NOP")

#define SYS_CLK_MHZ 200

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
#pragma DATA_SECTION(output_sample_right,"CLAData");
float output_sample_right;
#pragma DATA_SECTION(input_sample_right,"CLAData");
float input_sample_right;

// stores gain values
#pragma DATA_SECTION(gains, "GainVals");
float gains[NUM_BANDS];

// right channel input, to be processed on CPU2
#pragma DATA_SECTION(input_sample_right_from_cpu1,"SampleToCPU2");
float input_sample_right_from_cpu1;

// right channel output, already processed by CPU2
#pragma DATA_SECTION(output_sample_right_to_cpu1,"SampleFromCPU2");
float output_sample_right_to_cpu1;

interrupt void IPC0StartProcessISR() {
    // handle right channel processing by CLA1
    input_sample_right = input_sample_right_from_cpu1;
    memcpy((uint32_t *)&gains_cla, (uint32_t *)&gains,
           (uint32_t)NUM_BANDS * sizeof(gains[0]));
    Cla1ForceTask1();

    IpcRegs.IPCACK.bit.IPC0 = 1;

    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
}

interrupt void Cla1Task1CompleteISR() {
    // send output sample back to CPU1
    output_sample_right_to_cpu1 = output_sample_right;

    // Notify CPU1 that the processing is complete
    IpcRegs.IPCSET.bit.IPC31 = 1;

    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP11;
}

// CLA_configClaMemory - Configure CLA memory
void CLA_configClaMemory(void) {
    extern uint32_t Cla1funcsRunStart, Cla1funcsLoadStart, Cla1funcsLoadSize;

    // Wait for signal from CPU1 before configuring
    IpcSync(5);

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

void CLA_initCpu2Cla1(void) {
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

    InitPieCtrl(); // Initialize PIE (disable PIE and clear PIE registers)
    IER = 0x0000; // clear CPU interrupt register
    IFR = 0x0000; // clear CPU interrupt flag register
    InitPieVectTable(); // initialize PIE vector table to known state

    CLA_configClaMemory();
    CLA_initCpu2Cla1();

    // Enable IPC0 interrupt (from CPU1)
    PieVectTable.IPC0_INT = &IPC0StartProcessISR;
    PieCtrlRegs.PIEIER1.bit.INTx13 = 1;
    IER |= M_INT1;

    EnableInterrupts(); // Enable Global interrupts
    ERTM; // Enable Global realtime interrupt DBGM


    while (true) {}
}


