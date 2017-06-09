#include <stdio.h>
#include <time.h>

void showTm(struct tm info){
   printf("tm.tm_year   = %d\n", info.tm_year + 1900);
   printf("tm.tm_gmtoff = %d\n", info.tm_gmtoff);
   printf("tm.tzone     = %s\n", info.tm_zone);      
   printf("Current local time and date: %s", asctime(&info));
}

int main ()
{
   time_t rawtime;
   time_t rawtime2;
   struct tm *info;
   struct tm *info2;
   char buffer[80];

   time( &rawtime );

   printf("rawtime = %d\n", rawtime);

   info = localtime( &rawtime );
   showTm(*info);

   rawtime2 = 0;
   info2 = localtime(&rawtime2);
   showTm(*info2);

   printf("--------------------\n\n");
   rawtime2 = time(NULL);
   printf("rawtime2 = %d\n", rawtime2);
   info2 = localtime(&rawtime2);
   showTm(*info2);

   
   printf("--------------------\n\n");
   rawtime2 = 1496843850;
   printf("rawtime2 = %d\n", rawtime2);
   info2 = localtime(&rawtime2);
   showTm(*info2);

   printf("x = %d", sizeof(struct tm));
     
   return(0);
}
