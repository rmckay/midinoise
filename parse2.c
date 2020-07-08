#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "notes.h"
#include "loadmid.h"

struct mfhead {
 char head[4];
 long size;
 unsigned short format;
 unsigned short length;
 unsigned short deltatime;
};

void
printHeader(struct mfhead *header)
{

  short format = ((header->format)>>8)|((header->format)<<8);
  short length = ((header->length)>>8)|((header->length)<<8);
  short deltatime = ((header->deltatime)>>8)|((header->deltatime)<<8);

  printf("Format: %d\n", format);
  printf("Number of tracks: %d\n", length);
  printf("Delta time ticks: %d\n", deltatime);

}

int last_vtime = 0;

int
main(int argc, char *argv[])
{

  int f;
  int playing;
  struct mfhead *header;
  unsigned char note;
  unsigned char vol;
  time_t now_sec;
  suseconds_t now_usec;

  struct timeval tv;

  init_midi_tables();
  init_sound();

  loadmid(argv[1]);
  unsigned char *ptr = midi_start;
  printHeader(midi_start);

      gettimeofday(&tv, NULL);
      now_sec = tv.tv_sec;
      now_usec = tv.tv_usec;
      last_vtime = 0;


//  ptr+=sizeof(struct mfhead);
  ptr+=121;

  unsigned char c;
  while(1)
  {
    c=*(ptr++);
    printf("ptr = %x val = %x offset = %d\n",ptr, *ptr, (int)(((void *)ptr)-midi_start));

    // track_event = <v_time> + <midi_event> | <meta_event> | <sysex_event>

    // so.. first we read the v_time.

    int deltat = parse_vtime(&ptr);
    printf("Delta time parsed: %d\n", deltat);

    if (c >= 128)
    {
      int cmd = c>>4;
      int chan = c & 0x0F;

      // We have a command. Let's see if we're there yet?
      gettimeofday(&tv, NULL);
      now_sec = tv.tv_sec;
      now_usec = tv.tv_usec;
      printf("Seen command at %d and %d\n", tv.tv_sec, tv.tv_usec);

      printf("Start of command byte... %d for chan %d\n", c, chan);

      // Optional filter channel argument
      printf("argc is :%d\n", argc);
      if (argc>2) {
        int filter = atoi(argv[2]);
        if (chan != filter) {
          printf("filtering %d\n", chan);
  	continue;
        }
      }

  
      switch (cmd)
      {
         case 8: printf("Note stop command %d\n", last_vtime);
                 playing = 0;
                 break;
         case 9: 
		 if (*ptr >= 128) continue;
		 note = *ptr; 
		 ptr++;
		 vol = *ptr; 
		 ptr++;

		 printf("Note start command: %d vol: %d vtime: %d\n", note, vol, last_vtime);
                 printf("The note %d is %f\n", note, notes[note]);
                 printf("The vtime for this note is: %d\n", last_vtime);

                 if (vol==0)
                 { 
                   printf("Skipping this note, because it's a mute\n");
                   playing = 0;
                   break;
                 }

                 //play_midi(note);
//                 play_midi_fraction(note, 1);
//                 sleep(1);
                 playing = 1;
                 break;
         case 10: printf("Key after-touch\n");
                  break;
         case 11: printf("Control Change\n");
                  break;
         case 12: printf("Program (patch) change\n");
                  break;
         case 13: printf("Channel after-touch\n");
                  break;
         case 14: printf("Pitch wheel change (2000H is normal or no change)");
                  break;
         default: printf("Unhandled command!?\n");
                  break;
       }
    }
    else
    {
      printf("Data byte... %x\n", *ptr);
    }


   
   printf("time is: %d sec and %d microsec\n", tv.tv_sec, tv.tv_usec);


   while (1)
   {
     gettimeofday(&tv, NULL);

     time_t delta_sec = tv.tv_sec - now_sec;
     suseconds_t delta_usec = tv.tv_usec - now_usec;

     long long int delta_total = (delta_sec*1000000) + delta_usec;

     if (delta_total < (last_vtime*5))
     {
       printf("Sleeping... %lld < %lld\n", delta_total, (last_vtime*5));
//       usleep(50000);
     }
     else
       break;

     /* Play some notes? */
     if(playing==1)
     {
       play_midi_fraction(note, 1);
     } else {
       play_silence(1);
     }

   }


  

  }



}

// Parse a vtime
int
parse_vtime(void **ptr)
{

  last_vtime = parse_vval(ptr);
  return last_vtime;

}

// Parse a Variable Length Value starting at pointer and return it as an int
// advances pointer to after the value.
int
parse_vval(void **ptr)
{

    int accum=0; // accumulator

    unsigned char tmp =*(unsigned char *)(*ptr);

    while(tmp & (1<<8))
    {
      tmp &= ~(1 << 8); // strip top bit
      accum+=tmp;
      *ptr++; // still advance pointer
      tmp =*(char *)(*ptr);
    }

    accum+=tmp;

    return (int)accum;
}
