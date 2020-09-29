;ALEX LEMA
;LAB 2-A    DATE: 9/13/2019
    
  .include "xc.inc" ; required "boiler-plate" (BP)
;the next two lines set up the actual chip for operation - required
config __CONFIG2, POSCMOD_EC & I2C1SEL_SEC & IOL1WAY_OFF & OSCIOFNC_ON &FCKSM_CSECME & FNOSC_FRCPLL & SOSCSEL_LPSOSC & WUTSEL_FST & IESO_OFF
config __CONFIG1, WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & BKBUG_ON &GWRP_ON & GCP_ON & JTAGEN_OFF
 .bss ; put the following labels in RAM
counter:
 .space 2 ; a variable that takes two bytes (we won?t use
 ; it for now, but put here to make this a generic
 ; template to be used later).
stack:
 .space 32 ; this will be our stack area, needed for func calls
.text ; BP (put the following data in ROM(program memory))

;because we are using the C compiler to assemble our code, we need a "_main" label
;somewhere. (There's a link step that looks for it.)
.global _main ;BP
 
_main:
    
 bclr CLKDIV,#8 ;BP
 nop
 ;; --- Begin your program below here ---
 
 
 mov #0x9fff,w0
 mov w0,AD1PCFG ; Set all pins to digital mode
 mov #0b1111111111111110,w0
 mov w0,TRISA ; set pin RA0 to output
 mov #0x0001,w0
 mov w0,LATA ; set pin RA0 high
 
 ;call foreverLoop
 ;call lem_wait_100us

 ;mov #0x0001, w0
 ;mov w0, LATA
 
 ;foreverLoop:
 ;nop
 ;bra foreverLoop
 ;nop
 ;.end ; this doesn?t actually end anything. Does not translate to assembly
 ; code. Just a way to tell the compiler we are done with this fil
 
;Do nothing PAGE 8
;foreverLoop
;nop
;bra foreverLoop
;nop


 foreverLoop:
    ;call lem_wait_1ms   ; 16000 cycles; /////1cycle = 62.5ns////
    clr LATA  
    call lem_wait_100us   ; 16000 cycles; /////1cycle = 62.5ns////
    nop  ;  1 cycle
    call write_1 ;1   ;24 CALLS
    call write_1 ;2  
    call write_1 ;3
    call write_1 ;4
 
    call write_0 ;5
    call write_0 ;6
    call write_0 ;7
    call write_0 ;8
    
    call write_0 ;9
    call write_0 ;10
    call write_0;11
    call write_0 ;12
 
    call write_0 ;13
    call write_0 ;14
    call write_0 ;15
    call write_0 ;16
    
    call write_1 ;17
    call write_1 ;18
    call write_1 ;19
    call write_1 ;20
 
    call write_0 ;21
    call write_0 ;22
    call write_0 ;23
    call write_0 ;24
    
    bra foreverLoop
    nop
    
    
 ;void write_bit_stream(void) {}
 write_0:            ;;2 calling
    bset LATA, #0    ; Start the write cycle; Bit set in Ws  ;1 cycle
    repeat #3        ;1 cycle to load and prep
    nop              ;3+1=19 cycles to execute 
    clr LATA          ;1 cycle
    repeat #8       ;1 cycle to load and prep
    nop               ;8+1=28 cycles to execute
    return	     ;3 cycles for the retur
    
    ;22 cycles/////1cycle = 62.5ns////

 write_1:                ;;2 calling
    bset LATA, #0        ; Start the write cycle; Bit set in Ws;1 cycle
    repeat #8		 ;1 cycle to load and prep
    nop			 ;8+1=9 cycles to execute 
    clr LATA        ; Start the write cycle, Bit set in Ws;1 cycle
    repeat #3        ;1 cycle to load and prep
    nop                  ;1+3=4
    return		;3 cycles for the retur
    
   ;24 cycles/////1cycle = 62.5ns////  
   
 lem_wait_100us:   ;2 calling
    repeat #1593    ;1 cycle to load and prep
    nop             ;1593+1 cycles to execute NOP 9 times
    return          ;3 cycles for the return
;1600
 lem_wait_1ms:      ;;2 calling
    repeat #15993    ;1 cycle to load and prep
    nop            ;15995+1 cycles to execute NOP 9 times
    return         ;3 cycles for the return
 ;16000
 
    .end
