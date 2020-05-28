/*
** Copyright (c) 1997 by Oracle Corporation
**
** NAME
**   EXTERN.C 
**
** DESCRIPTION
**   Sample Windows NT External Procedure: find_max 
**
*/

#include <windows.h>

#define NullValue -1

/*
  This function simply returns the returns the larger of x and y.
*/

long __declspec(dllexport) find_max(long 	x, 
				    short 	x_indicator, 
                                    long 	y, 
			     	    short       y_indicator, 
				    short       *ret_indicator)
{
   /* It can be tricky to debug DLL's that are being called by a process
      that is spawned only when needed, as in this case.  
      Therefore try using the DebugBreak(); command.  
      This will start your debugger.  Uncomment the following line and
      you can step right into your code.
   */
   /* DebugBreak();  */

   /* first check to see if you have any nulls */
   /* Just return a null if either x or y is null */

   if ( x_indicator==NullValue || y_indicator==NullValue) {
      *ret_indicator = NullValue;   
      return(0);
   } else { 
      *ret_indicator = 0;        /* Signify that return value is not null */
      if (x >= y) return x;
      else return y;
   }
}
