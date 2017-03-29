#include <18f4620.h>
#use delay(clock=16M)
#fuses XT, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG

/*#INT_RB
void RB_isr(void){ //función de interrupción
   if(input(PIN_B4)){
      outputResultado13b(input_c() + input_d());
   } else
   if(input(PIN_B5)){
      outputResultado13b(input_c() - input_d());
   } else
   if(input(PIN_B6)){
      outputResultado13b(input_c() * input_d());
   } else
   if(input(PIN_B7)){
      if(input_d() != 0){
         outputResultado13b(input_c() / input_d());
      }         
    }
}*/
//int16 resultado = 1;

void outputResultado13b(int16 res);
void cambioEstadoPuertoA();
void cambioEstadoPuertoB();
void cambioEstadoPuertoE();

/*#INT_RB
void RB_isr(void){ //función de interrupción
//int16 resultado;
   if(input(PIN_B4)){
      resultado = input_c() + input_d();
      outputResultado13b(resultado);
      disable_interrupts(GLOBAL);  
      enable_interrupts(GLOBAL);
      //INTCONbits.RBIF=0;
   } else
   if(input(PIN_B5)){
      resultado = input_c() - input_d();
      outputResultado13b(resultado);
      disable_interrupts(GLOBAL); 
      enable_interrupts(GLOBAL);
   } else
   if(input(PIN_B6)){
      resultado = input_c() * input_d(); 
      outputResultado13b(resultado);
      disable_interrupts(GLOBAL); 
      enable_interrupts(GLOBAL);
   } else
   if(input(PIN_B7)){
      if(input_d() != 0){
         resultado = input_c() / input_d();
         outputResultado13b(resultado);
         disable_interrupts(GLOBAL);  
         enable_interrupts(GLOBAL);
      }         
    }
}*/

void main(void){
      
      setup_oscillator(OSC_normal);
      setup_adc(NO_ANALOGS);
      
      unsigned int8 numeroUno = 0;    // Se puede reemplazar esto al leerlo directamente y operarlo    
      unsigned int8 numeroDos = 0;  
      int16 resultado = 0;
      int banderasOperaciones = 0;
    
      #bit banderaError = banderasOperaciones.2;
      #bit banderaContador = banderasOperaciones.3;
      #bit suma = banderasOperaciones.4;
      #bit resta = banderasOperaciones.5;
      #bit multiplicacion = banderasOperaciones.6;
      #bit division = banderasOperaciones.7;
      
      int contador = 50;
      /*int1 opSuma = 0;
      int1 opResta = 0;
      int1 opMultiplicacion = 0;
      int1 opDivision = 0;  */    
      
      SET_TRIS_A(0x00);
      SET_TRIS_E(0x08);
      SET_TRIS_B(0xF0); //SET_TRIS_B(0x78) 3 a 6
      SET_TRIS_C(0xFF);
      SET_TRIS_D(0xFF);
      
      
      
      //port_a_pullups(TRUE);
      //EXT_INT_EDGE(H_TO_L);
      //EXT_INT_EDGE(L_TO_H);
      //enable_interrupts(INT_RB);//habilita interrupción particular
      //enable_interrupts(GLOBAL);//habilita interrupción global       

      
      
      while(true){
         banderaError = 0;  //Inicializacion bandera error
         numeroUno = input_c();
         numeroDos = input_d();
         
         
         /*opSuma = input(PIN_B4);
         opResta = input(PIN_B5);
         opMultiplicacion = input(PIN_B6);
         opDivision = input(PIN_B7);*/
         
         if(input(PIN_B4)){
            suma = 1;
            resta = 0;
            multiplicacion = 0;
            division = 0;
         } else
         if(input(PIN_B5)){
            suma = 0;
            resta = 1;
            multiplicacion = 0;
            division = 0;
         } else
         if(input(PIN_B6)){
            suma = 0;
            resta = 0;
            multiplicacion = 1;
            division = 0;           
         } else
         if(input(PIN_B7)){
             suma = 0;
            resta = 0;
            multiplicacion = 0;
            division = 1;
         }
      //}  
         if(suma){
            resultado = (long)numeroUno + (long)numeroDos;
            outputResultado13b(resultado);
         } else
         if(resta){
            resultado = (long)numeroUno - (long)numeroDos;
            outputResultado13b(resultado);
         } else
         if(multiplicacion){
            resultado = (long)numeroUno * (long)numeroDos;     
            outputResultado13b(resultado);
         } else
         if(division){
             if(numeroDos != 0){
               resultado = (long)numeroUno / (long)numeroDos;
               outputResultado13b(resultado);
               
            } 
            else{
               banderaError = 1;  //Bandera error Activa
               delay_ms(10);
               contador ++;
               if(contador == 50){
                    /* cambioEstadoPuertoA();
                     cambioEstadoPuertoB();
                     cambioEstadoPuertoE();
                     */
                     if(banderaContador == 1){
                        outputResultado13b(8191);
                        contador =0;
                        banderaContador = 0;
                     }
                     else{
                        outputResultado13b(0);
                        banderaContador = 1;
                        contador = 0;
                     }       
                }       
            }
         }
         if(banderaError == 0)
            outputResultado13b(resultado);
        
            
        
   }
         //outputResultado13b(8191);
}
       


void outputResultado13b(int16 res){
   OUTPUT_A(res);
   OUTPUT_B(res>>6);
   OUTPUT_E(res>>10);
}
void cambioEstadoPuertoA(){
   output_toggle(PIN_A0);
   output_toggle(PIN_A1);
   output_toggle(PIN_A2);
   output_toggle(PIN_A3);
   output_toggle(PIN_A4);
   output_toggle(PIN_A5);
}
void cambioEstadoPuertoB(){
   output_toggle(PIN_B0);
   output_toggle(PIN_B1);
   output_toggle(PIN_B2);
   output_toggle(PIN_B3);
}
void cambioEstadoPuertoE(){
   output_toggle(PIN_E0);
   output_toggle(PIN_E1);
   output_toggle(PIN_E2);
}
