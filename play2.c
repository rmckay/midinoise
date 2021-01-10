#include <stdio.h>
#include <linux/soundcard.h>
#include <math.h>
#include "play.h"
#include "notes.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>


int init_sound()
{

  dsp = open("/dev/dsp", 0666);

  int chan = 1;
  int rate = 44100;
  int bits = 16;

  ioctl(dsp, SOUND_PCM_WRITE_CHANNELS, &chan); // mono!

  ioctl(dsp, SOUND_PCM_WRITE_BITS, &bits); // 8bit samples

  ioctl(dsp, SOUND_PCM_WRITE_RATE, &rate); // 8000 samples/sec

// oh yea, i'm just gonna assume all that worked 'cause I'm not a pussy.

}

float pure_tone(float time, float freq_hz) {
    return sin(time * freq_hz * 2 / 44100);
}

// https://en.wikipedia.org/wiki/Phase_modulation
float modulated_tone(float time, float freq_hz, float mod_hz, float amt) {
    return sin((time * freq_hz * 2 / 44100) + (amt * M_PI/2 * sin(mod_hz * time) ));
}


//int play_note_fraction(double note, int fraction) {
//
//    unsigned short bbuf[44100];
//    signed short min = 32768;
//    signed short max = 0;
//    float decay = 1;
//    for (float i=0;i<44100/2;i+=1) {
//        float fsample;
//        if (i<3000) {
////    fsample = pure_tone(i, note) * i * 5;
//            fsample = modulated_tone(i, note, 300, i/3000) * i * 5;
//        } else {
////    fsample = pure_tone(i, note) * 32768;
//            fsample = modulated_tone(i, note, 300, decay) * 32768;
//            decay-=0.00002;
//        }
//
//        signed short sample = (signed short)(fsample - 32768);
//
//        // convert sine wave to square wave
//        // sample = ((((sample > 0) - (sample < 0 ))+1) * 65530) - 32768;
//
//        if (sample>max) { max = sample; }
//        if (sample<min) { min = sample; }
//
//        bbuf[(int)i]=sample;
//
//    }
//    printf("min: %d, max: %d\n", min, max);
//    write(dsp, &bbuf, sizeof(bbuf)/2);
//
//}

int play_note(double note)
{
  play_note_fraction(note, 1000);
}

int play_silence(int fraction) {
  char buf[4410000];
  memset(buf, 0, sizeof(buf));
  write(dsp, buf, 1000);
}

int play_notes(char playing[]) {

  char buf[4410000] = {0};
  int rate = 44100;
  int x=0;
  int c=0;
  char by;
  int y=0;
  float fsample;
  //int fraction = 100; // just cause

  int first=1;
  for (int i=0;i<128;i++) {
    int np = playing[i];
    if (np!=0) {
      double freq = notes[i];
      double rep = (rate / freq);
      float decay = 1;
      y = 0;
      for (x=0;x<65536;x+=(65536.0/rep))
      {

          if (x<3000) {
              fsample = modulated_tone(x, freq, 300, i / 3000) * i * 5;
          } else {
              fsample = modulated_tone(x, freq, 300, decay) * 32768;
              decay-=0.00002;
          }
          //fsample = pure_tone(x, freq) * 256;
          if (first) {
              //buf[y]=sin(freq * x) * 256;
              buf[y]=fsample;
              first=0;
          } else {
              //buf[y] = (buf[y] + (sin(freq * x) * 256)) / 2;
              buf[y] = ((buf[y] + fsample) / 2);
          }
          y++;
      }
    }

  }



  int z= 0;
  for (z=0;z<1;z++)
  {
    write(dsp,buf, y);
  }

}



int play_note_fraction(double note, int fraction)
{

  char buf[4410000];
  int rate = 44100;
  int x=0;
  int c=0;
  char by;

  double freq = note/2;
  double rep = (rate/freq);

  int y=0;

  printf("rep = %f for freq = %f\n", rep, freq);

//  for (x=0;x<65536;x+=(65536.0/rep))
//  {
//    buf[y++]=2^x-1;
    buf[y++]=x/256;
//      int prim = sin(x/1000)*1000;
//      prim+=rand()*100;
//      buf[y++]=sin(prim/1)*1000;
//  }

  for (x=0;x<65536;x+=(65536.0/rep))
  {
    buf[y++]=sin(freq * x) * 256;
  }


/*
  for (x=0;x<8000;x++) {
    buf[y++]=sin(x/note)*256.0;
  }
  write(dsp,buf,y);
*/

  int z= 0;
/*
    for (z=0;z<(freq/fraction);z++)
    {
      write(dsp,buf, y);
    }
*/
    for (z=0;z<800;z++)
    {
      write(dsp,buf, y);
    }


}

int play_midi_fraction(int note, int fraction)
{
  play_note_fraction(notes[note], fraction);
}

int play_midi(int note)
{
  play_note(notes[note]);
}
