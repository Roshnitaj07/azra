 #include <stdio.h>
struct time
 {
  int h,m,s;
}stime;

int  time_to_sec(struct time t1);
int sec_to_time(int sec)
{
 
	stime.h = (sec/3600); 
	stime.m = (sec-(3600*stime.h))/60;
	stime.s = (sec -(3600*stime.h)-(stime.m*60));
	printf("HH:MM:SS - %d:%d:%d\n",stime.h,stime.m,stime.s);
	return 0;
}
 
  
  int  time_to_sec(struct time stime1)
{
        float timeinsec = stime1.s + (stime1.m * 60) + (stime1.h * 60 * 60);
        printf("Total seconds in %dH:%dM:%dS is %f\n", stime1.h, stime1.m, stime1.s, timeinsec);
        return 0;
  }
int main()
{
 struct time t2;
 int sec=10;
 printf("enter values for time in HH:MM:SS format\n");
scanf("%d:%d:%d",&t2.h,&t2.m,&t2.s);
   if((t2.h>=0&&t2.h<24)&&(t2.m>=0&&t2.m<60)&&(t2.s>=0&&t2.s<60))
        printf("Valid\n");

      else
          printf("Invalid\n");

 time_to_sec(t2);
 sec_to_time(sec);

return 0;
             

}
