    // Set the CLC1 to the options selected in the User Interface

    // LC1G1POL not_inverted; LC1G2POL not_inverted; LC1G3POL not_inverted; LC1G4POL not_inverted; LC1POL not_inverted; 
    CLC1POL = 0x00;

    // LC1D1S SPI1 SCK OUT; 
    CLC1SEL0 = 0x2C;

    // LC1D2S SPI1 SS OUT; 
    CLC1SEL1 = 0x2D;

    // LC1D3S CLCIN0 (CLCIN0PPS); 
    CLC1SEL2 = 0x00;

    // LC1D4S CLCIN0 (CLCIN0PPS); 
    CLC1SEL3 = 0x00;

    // LC1G1D3N disabled; LC1G1D2N disabled; LC1G1D4N disabled; LC1G1D1T enabled; LC1G1D3T disabled; LC1G1D2T disabled; LC1G1D4T disabled; LC1G1D1N disabled; 
    CLC1GLS0 = 0x02;

    // LC1G2D2N disabled; LC1G2D1N disabled; LC1G2D4N disabled; LC1G2D3N disabled; LC1G2D2T disabled; LC1G2D1T disabled; LC1G2D4T disabled; LC1G2D3T disabled; 
    CLC1GLS1 = 0x00;

    // LC1G3D1N disabled; LC1G3D2N disabled; LC1G3D3N enabled; LC1G3D4N disabled; LC1G3D1T disabled; LC1G3D2T disabled; LC1G3D3T disabled; LC1G3D4T disabled; 
    CLC1GLS2 = 0x10;

    // LC1G4D1N disabled; LC1G4D2N disabled; LC1G4D3N disabled; LC1G4D4N disabled; LC1G4D1T disabled; LC1G4D2T disabled; LC1G4D3T disabled; LC1G4D4T disabled; 
    CLC1GLS3 = 0x00;

    // LC1EN enabled; INTN disabled; INTP disabled; MODE SR latch; 
    CLC1CON = 0x83;

    CLC2POL = 0x00;

    // LC2D1S SPI1 SDO OUT; 
    CLC2SEL0 = 0x2B;

    // LC2D2S PWM5_OUT; 
    CLC2SEL1 = 0x18;

    // LC2D3S PWM6_OUT; 
    CLC2SEL2 = 0x19;

    // LC2D4S CLCIN0 (CLCIN0PPS); 
    CLC2SEL3 = 0x00;

    // LC2G1D3N disabled; LC2G1D2N disabled; LC2G1D4N disabled; LC2G1D1T enabled; LC2G1D3T disabled; LC2G1D2T disabled; LC2G1D4T disabled; LC2G1D1N disabled; 
    CLC2GLS0 = 0x02;

    // LC2G2D2N disabled; LC2G2D1N disabled; LC2G2D4N disabled; LC2G2D3N disabled; LC2G2D2T enabled; LC2G2D1T disabled; LC2G2D4T disabled; LC2G2D3T disabled; 
    CLC2GLS1 = 0x08;

    // LC2G3D1N enabled; LC2G3D2N disabled; LC2G3D3N disabled; LC2G3D4N disabled; LC2G3D1T disabled; LC2G3D2T disabled; LC2G3D3T disabled; LC2G3D4T disabled; 
    CLC2GLS2 = 0x01;

    // LC2G4D1N disabled; LC2G4D2N disabled; LC2G4D3N disabled; LC2G4D4N disabled; LC2G4D1T disabled; LC2G4D2T disabled; LC2G4D3T enabled; LC2G4D4T disabled; 
    CLC2GLS3 = 0x20;

    // LC2EN enabled; INTN disabled; INTP disabled; MODE AND-OR; 
    CLC2CON = 0x80;

    CLC3POL = 0x08;

    // LC3D1S CLC1_OUT; 
    CLC3SEL0 = 0x24;

    // LC3D2S CLC2_OUT; 
    CLC3SEL1 = 0x25;

    // LC3D3S CLCIN0 (CLCIN0PPS); 
    CLC3SEL2 = 0x00;

    // LC3D4S CLCIN0 (CLCIN0PPS); 
    CLC3SEL3 = 0x00;

    // LC3G1D3N disabled; LC3G1D2N disabled; LC3G1D4N disabled; LC3G1D1T enabled; LC3G1D3T disabled; LC3G1D2T disabled; LC3G1D4T disabled; LC3G1D1N disabled; 
    CLC3GLS0 = 0x02;

    // LC3G2D2N disabled; LC3G2D1N disabled; LC3G2D4N disabled; LC3G2D3N disabled; LC3G2D2T enabled; LC3G2D1T disabled; LC3G2D4T disabled; LC3G2D3T disabled; 
    CLC3GLS1 = 0x08;

    // LC3G3D1N enabled; LC3G3D2N disabled; LC3G3D3N disabled; LC3G3D4N disabled; LC3G3D1T disabled; LC3G3D2T disabled; LC3G3D3T disabled; LC3G3D4T disabled; 
    CLC3GLS2 = 0x01;

    // LC3G4D1N disabled; LC3G4D2N disabled; LC3G4D3N disabled; LC3G4D4N disabled; LC3G4D1T disabled; LC3G4D2T disabled; LC3G4D3T disabled; LC3G4D4T disabled; 
    CLC3GLS3 = 0x00;

    // LC3EN enabled; INTN disabled; INTP disabled; MODE AND-OR; 
    CLC3CON = 0x80;