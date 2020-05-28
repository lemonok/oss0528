void delay();
main()
{
   unsigned int i;
   
    SCON = 0x50;
   while(1)
   {
   SBUF = 65;
   for(i=0;i<1000;i++)
   delay();            // Call delay
   }
}

void delay()               // Delay generation using Timer 0 mode 1
{
   TMOD = 0x01;            // Mode1 of Timer0

   TH0= 0xFC;              // FC66 evaluated hex value for 1millisecond delay
   TL0 = 0x66;
   
   TR0_bit = 1;               // Start Timer
   while(TF0_bit == 0);          // Using polling method
   TR0_bit = 0;             // Stop Timer
   TF0_bit = 0;             // Clear flag
}