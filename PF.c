// tratamento do bounce
int cont=0;
int cont1=0;

//comunicação serial
int alarme=0;
int var=1;
int valor_baud=51;
unsigned int i1=0;
unsigned char m2;
unsigned char y;
unsigned char string_r[100];  // string recebida
unsigned char string_a[100]; // string atual

//Comparação de strings recebidas
int c0=1; // ligar lampada 1
int c1=1; // desliga lmapada 1
int c2=1; // ligar lampada 1
int c3=1; // desliga lmapada 1
int c4=1; // ligar lampada 1
int c5=1; // desliga lmapada 1
int c6=1; // ligar lampada 1
int c7=1; // desliga lmapada 1
int c8=1; // desativar alarme

//Leitura Módulo RTC
int sgsi=0, minsi=0, hrsi=0, ssi=0, ddi=0, mmi=0, yyi=0;
unsigned short sgs, mins, hrs, ss, dd, mm, yy;
int dados[7]={0x00,0x15,0x00,0x01,0x1F,0x03,0x13};
char varb[7]; //rtc to serial
char string_rtc[]="00:00:00";
char string_data[]="00/00/00";

//Configuraçõa da hora
int lcdon=0;
int hora=0;
int minuto=0;
int hora2=0;
int minuto2=0;
char varc[4];
int push=0;



sbit LCD_RS at LATE0_bit;
sbit LCD_EN at LATE1_bit;
sbit LCD_D4 at LATE2_bit;
sbit LCD_D5 at LATE3_bit;
sbit LCD_D6 at LATE4_bit;
sbit LCD_D7 at LATE5_bit;

sbit LCD_RS_Direction at TRISE0_bit;
sbit LCD_EN_Direction at TRISE1_bit;
sbit LCD_D4_Direction at TRISE2_bit;
sbit LCD_D5_Direction at TRISE3_bit;
sbit LCD_D6_Direction at TRISE4_bit;
sbit LCD_D7_Direction at TRISE5_bit;


void receber() iv IVT_ADDR_U2RXINTERRUPT ics ICS_AUTO {
     IFS1bits.U2RXIF=0;
     m2 = U2RXREG ;
     if(m2!='\n')
     {
     string_r[i1]=m2;
     i1++;
     }
     else{
     var=0;
     i1=0;
     memcpy(string_a,string_r,strlen(string_r));
     memset(string_r,0,strlen(string_r));
     }
}

 void tempo1() iv IVT_ADDR_T1INTERRUPT ics ICS_AUTO {
     IFS0bits.T1IF = 0;
     cont++;
     cont1++;
     if(alarme==1 & cont>10){
     //liga e desliga buzzer
     LATEbits.LATE8=~LATEbits.LATE8;
     LATBbits.LATB4=~LATBbits.LATB4;
     LATBbits.LATB5=~LATBbits.LATB5;
     LATBbits.LATB6=~LATBbits.LATB6;
     LATBbits.LATB7=~LATBbits.LATB7;
     cont=0;
     }
}

void INIT_UART2()
{   U2BRG = valor_baud;
    /*Configuramos a UART, 8 bits de dados, 1 bit de parada, sem paridade */
    U2MODE = 0x0000; //Ver tabela para saber as outras configurações
    U2STA = 0x0000;
    //IPC2 = 0x0440; //A faixa de prioridade média, não é urgente.
    IFS1bits.U2TXIF = 0; //Zerar o flag de interrupção de Tx.
    IEC1bits.U2TXIE = 0; //Habilita interrupção de Tx.
    IFS1bits.U2RXIF = 0; //Zerar o flag de interrupção de Rx.
    IEC1bits.U2RXIE = 1; //Habilita interrupção de Rx.
    U2MODEbits.USIDL = 1;
    U2MODEbits.UARTEN = 1; //E liga a UART
    U2STAbits.UTXEN = 1;
}



unsigned char INCHR_UART2(){
  unsigned char c;
  while (!U2STAbits.URXDA); // espera enquanto o buffer de Rx está VAZIO.
  c = U2RXREG ; // recebe caractere.
  return c;
}

void OUTCHR_UART2(unsigned char c[]){
  while (U2STAbits.UTXBF); // espera enquanto o buffer de Tx está cheio.
  U2TXREG = c; // escreve caractere.
}

void enviar(char s[]){
int tam;
int i;
tam=strlen(s);
for(i=0;i<tam;i++){

OUTCHR_UART2(s[i]);
}
OUTCHR_UART2('\n');

}

void verhora(){
      I2C1_Start();              // issue I2C start signal
      I2C1_Write(0xD0);          // send byte via I2C  (device address + W)
      I2C1_Write(0);             // send byte (data address)
      I2C1_Restart();            // issue I2C signal repeated start
      I2C1_Write(0xD1);          // send byte (device address + R)
      sgs = I2C1_Read(0);      // Read the data (NO acknowledge)
      mins = I2C1_Read(0);
      hrs = I2C1_Read(0);
      ss = I2C1_Read(0);
      dd = I2C1_Read(0);
      mm = I2C1_Read(0);
      yy = I2C1_Read(1);
      I2C1_Stop();               // issue I2C stop signal


      sgsi = Bcd2Dec(sgs);
      minsi = Bcd2Dec(mins);
      hrsi = Bcd2Dec(hrs);
      ssi = Bcd2Dec(ss);
      ddi = Bcd2Dec(dd);
      mmi = Bcd2Dec(mm);
      yyi = Bcd2Dec(yy);

      IntToStr(hrsi,varb);
      memcpy(string_rtc, varb+4, 2);
      memset(varb,0,strlen(varb));

      IntToStr(minsi,varb);
      memcpy(string_rtc+3,varb+4, 2);
      memset(varb,0,strlen(varb));

      IntToStr(sgsi,varb);
      memcpy(string_rtc+6,varb+4, 2);
      memset(varb,0,strlen(varb));


      IntToStr(ddi,varb);
      memcpy(string_data,varb+4, 2);
      memset(varb,0,strlen(varb));

      IntToStr(mmi,varb);
      memcpy(string_data+3,varb+4, 2);
      memset(varb,0,strlen(varb));

      IntToStr(yyi,varb);
      memcpy(string_data+6,varb+4, 2);
      memset(varb,0,strlen(varb));

      //enviar(string_rtc);

}

void LCDalarme(int x){

   Lcd_Cmd(_LCD_CLEAR); // Clear display
   Lcd_Cmd(_LCD_CURSOR_OFF); // Cursor off

   if(x==1){Lcd_Out(1,1,"A: 1");}
   if(x==2){Lcd_Out(1,1,"A: 2");}
   if(x==3){Lcd_Out(1,1,"A: 3");}
   if(x==4){Lcd_Out(1,1,"A: 4");}

   Lcd_Out(1,7,string_rtc);
   Lcd_Out(2,7,string_data);
}


void acesso(){
   verhora();
   if(PORTBbits.RB0==1 & cont1>2){
   verhora();
   cont1=0;

   if((hora-1)<=hrsi & hrsi<=(hora2-1)){  //& (minuto-1)<=minsi<=(minuto2-1)
   alarme=1;
   enviar("rx1");
   LCDalarme(1);}
   }
    if(PORTBbits.RB1==1 & cont1>2){
   cont1=0;
   if((hora-1)<=hrsi & hrsi<=(hora2-1)){
   alarme=1;
   enviar("rx2");
   verhora();
   LCDalarme(2);}
   }
    if(PORTBbits.RB2==1 & cont1>2){
   cont1=0;
   if((hora-1)<=hrsi & hrsi<=(hora2-1)){
   alarme=1;
   enviar("rx3");
   verhora();
   LCDalarme(3);}
   }
    if(PORTBbits.RB3==1 & cont1>2){
   cont1=0;
   if((hora-1)<=hrsi & hrsi<=(hora2-1)){
   alarme=1;
   enviar("rx4");
   verhora();
   LCDalarme(4);}
   }
}
/************* Programa Principal *************************/
int main (void){

    ADPCFG = 0xFFFF; //configura a porta B (PORTB) como entradas/saidas digitais

    TRISBbits.TRISB0=1; //acesso 1
    TRISBbits.TRISB1=1; //acesso 2
    TRISBbits.TRISB2=1; //acesso 3
    TRISBbits.TRISB3=1; //acesso 4

    TRISBbits.TRISB4=0; //luz 1
    TRISBbits.TRISB5=0; //luz 2
    TRISBbits.TRISB6=0; //luz 3
    TRISBbits.TRISB7=0; //luz 4

    TRISBbits.TRISB8=0;
    TRISFbits.TRISF0=1; // botão de conf.
    TRISFbits.TRISF1=1; // botão de conf.

    TRISEbits.TRISE8=0; //buzzer
    //LATEbits.LATE8=0;

    //-----Temporizador--------
    IFS0 = 0;
    IEC0 =0x0008;
    PR1 = 6250;
    T1CON = 0x8030;

    INIT_UART2(); //Inicializa com 19200 bps
    I2C1_Init(100000);

    Lcd_Init(); // Initialize LCD
    Lcd_Cmd(_LCD_CLEAR); // Clear display
    Lcd_Cmd(_LCD_CURSOR_OFF); // Cursor off
    Lcd_Out(1,1,"Sist. de Segurança"); // Write text in first row
    Lcd_Out(2,1,"Residencial");

  while (1)
     {

          if(var==0){

            c0=strcmp(string_a,"tx1l");    // ligar lampada 1
            c1=strcmp(string_a,"tx1d");    // desligar lampada 1
            c2=strcmp(string_a,"tx2l");    // ligar lampada 2
            c3=strcmp(string_a,"tx2d");    // desligar lampada 2
            c4=strcmp(string_a,"tx3l");    // ligar lampada 3
            c5=strcmp(string_a,"tx3d");    // desligar lampada 3
            c6=strcmp(string_a,"tx4l");    // ligar lampada 4
            c7=strcmp(string_a,"tx4d");    // desligar lampada 4
            c8=strcmp(string_a,"off");     // desligar Alarme

            if(c0==0){
             LATBbits.LATB4=1;
            }

            if(c1==0){
            LATBbits.LATB4=0;
            }

            if(c2==0){
            LATBbits.LATB5=1;
            }

            if(c3==0){
            LATBbits.LATB5=0;
            }

            if(c4==0){
            LATBbits.LATB6=1;
            }

            if(c5==0){
            LATBbits.LATB6=0;
            }

            if(c6==0){
            LATBbits.LATB7=1;
            }

            if(c7==0){
            LATBbits.LATB7=0;
            }

            if(c8==0){
            alarme=0;
            Lcd_Cmd(_LCD_CLEAR); // Clear display
            LATBbits.LATB4=0;
            LATBbits.LATB5=0;
            LATBbits.LATB6=0;
            LATBbits.LATB7=0;
            LATEbits.LATE8=0;
            }

            var=1;
            memset(string_a,0,strlen(string_a));
           }

          acesso();
          //delay_ms(100);
          if(PORTFbits.RF0==1 & cont1>2){

          cont1=0;
          if(lcdon==0){
          Lcd_Cmd(_LCD_CLEAR); // Clear display
          Lcd_Cmd(_LCD_CURSOR_OFF); // Cursor off
          Lcd_Out(1,1,"Selec. Horario"); // Write text in first row
          Lcd_Out(2,1,"p/ Alarme");
          delay_ms(3000);
          Lcd_Cmd(_LCD_CLEAR); // Clear display

          Lcd_Out(1,1,"Periodo de Inic."); // Write text in first row
          Lcd_Out(2,1,"Hora:");
          IntToStr(hora,varc);
          Lcd_Out(2,9,varc);
          memset(varc,0,strlen(varc));
          lcdon=1;
          push=1;
          }

          if(lcdon==1 & push==0){lcdon=3; push=1;}

          /*if(lcdon==2 & push==1){
          Lcd_Cmd(_LCD_CLEAR); // Clear display
          Lcd_Cmd(_LCD_CURSOR_OFF); // Cursor off
          Lcd_Out(1,1,"Periodo de Inic."); // Write text in first row
          Lcd_Out(2,1,"Minutos:");
          IntToStr(minuto,varc);
          memset(varc,0,strlen(varc));
          Lcd_Out(2,9,varc);
          }*/

          //if(lcdon==2 & push==0){lcdon=3;push=1;}

          if(lcdon==3 & push==1){
          Lcd_Cmd(_LCD_CLEAR); // Clear display
          Lcd_Out(1,1,"Periodo de Term."); // Write text in first row
          Lcd_Out(2,1,"Hora:");
          IntToStr(hora2,varc);
          Lcd_Out(2,9,varc);
          memset(varc,0,strlen(varc));
          }

          if(lcdon==3 & push==0){lcdon=5;push=1;}

         /*if(lcdon==4 & push==1){
          Lcd_Cmd(_LCD_CLEAR); // Clear display
          Lcd_Cmd(_LCD_CURSOR_OFF); // Cursor off
          Lcd_Out(1,1,"Periodo de Term."); // Write text in first row
          Lcd_Out(2,1,"Minutos:");
          IntToStr(minuto2,varc);
          memset(varc,0,strlen(varc));
          Lcd_Out(2,9,varc);
          }*/

          //if(lcdon==4 & push==0){lcdon=5;}

          if(push==0){push=1;}
          if(push==1){push=0;}

          if(lcdon==5){
          Lcd_Cmd(_LCD_CLEAR); // Clear display
          Lcd_Cmd(_LCD_CURSOR_OFF); // Cursor off
          Lcd_Out(1,1,"Alarme config."); // Write text in first row
          Lcd_Out(2,1,"com Sucesso");
          delay_ms(3000);
          Lcd_Cmd(_LCD_CLEAR);
          push=0;
          lcdon=0;
          }

          }

          if(PORTFbits.RF1==1 & cont1>2){
          cont1=0;

          if(lcdon==1){
          IntToStr(hora,varc);
          Lcd_Out(2,9,varc);
          memset(varc,0,strlen(varc));
          hora=hora++;
          if(hora>24){hora=0;}
          }

          /*if(lcdon==2){
          IntToStr(minuto,varc);
          Lcd_Out(2,9,varc);
          memset(varc,0,strlen(varc));
          minuto=minuto++;
          if(minuto>60){minuto=0;}
          }*/

          if(lcdon==3){
          IntToStr(hora2,varc);
          Lcd_Out(2,9,varc);
          memset(varc,0,strlen(varc));
          hora2=hora2++;
          if(hora2>24){hora2=0;}
          }

          /*if(lcdon==4){
          IntToStr(minuto2,varc);
          Lcd_Out(2,9,varc);
          memset(varc,0,strlen(varc));
          minuto2=minuto2++;
          if(minuto2>60){minuto2=0;}
          }*/

          }

          //sensor();

 }      }