#include <stdio.h>
#include <linux/soundcard.h>
#include "play.h"
#include "notes.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <math.h>

int init_sound()
{

  dsp = open("/dev/dsp", O_RDWR);

  int chan = 1;
  int rate = 44100;
  int bits = 16;

  ioctl(dsp, SOUND_PCM_WRITE_CHANNELS, &chan); // mono!

  ioctl(dsp, SOUND_PCM_WRITE_BITS, &bits); // 8bit samples

  ioctl(dsp, SOUND_PCM_WRITE_RATE, &rate); // 8000 samples/sec

// oh yea, i'm just gonna assume all that worked 'cause I'm not a pussy.

}

int init_pcm(char *filename)
{

  printf("Creating %s\n", filename);
  dsp = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);

}

int play_note(double note)
{

  printf("Playing note: %d\n", note);
  play_note_fraction(note, 1000);
}

int play_silence(int fraction) {
  char buf[4410000];
  memset(buf, 0, sizeof(buf));
  write(dsp, buf, 441*2);
}

float pure_tone(float time, float freq_hz) {
  return sin(time * freq_hz * 2 / 44100);
}

// https://en.wikipedia.org/wiki/Phase_modulation
float modulated_tone(float time, float freq_hz, float mod_hz, float amt) {
  return sin((time * freq_hz * 2 / 44100) + (amt * M_PI/2 * sin(mod_hz * time) ));
}


int play_note_fraction(double note, int fraction) {
 
 unsigned short bbuf[44100];
 signed short min = 32768;
 signed short max = 0;
 float decay = 1;
 printf("Playing note fraction: %f\n", note);
 for (float i=0;i<44100/2;i+=1) {
   float fsample;
   if (i<3000) {
//    fsample = pure_tone(i, note) * i * 5;
//    fsample = modulated_tone(i, note, 300, i/3000) * i * 5;
    fsample = modulated_tone(i, note, 300, i/3000) * i * 5;
   } else {
//    fsample = pure_tone(i, note) * 32768;
    fsample = modulated_tone(i, note, 300, decay) * 32768;
    decay-=0.00002;
   }

   signed short sample = (signed short)(fsample - 32768);

   // convert sine wave to square wave
  // sample = ((((sample > 0) - (sample < 0 ))+1) * 65530) - 32768;

   if (sample>max) { max = sample; }
   if (sample<min) { min = sample; }

   bbuf[(int)i]=sample;

 }
 printf("min: %d, max: %d\n", min, max);
 write(dsp, &bbuf, sizeof(bbuf)/4);

}
int x_play_note_fraction(double note, int fraction)
{

  char buf[4410000];
  signed short bbuf[441000];
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
//  }
  
  // sine wave
//  for (x=0;x<65536;x+=(65536.0/rep))
//  {
//    buf[y++]=(25600*sin(freq + x));
//  }

    // square wave
//  for (x=0;x<65536;x+=(65536.0/rep))
//  {
//    buf[y++]=(25600*sin(freq + x));
//  }

  // square wave
  for (x=0;x<44100;x++)
  {
//    buf[y++]=(128*sin(freq * x)) >= 0 ? 0xFF : 0x00;
//    bbuf[x]=(12800*sin(freq * x)) >= 0 ? 0xFFFF : 0x0000;
    bbuf[x]=32768*sin(freq * x * 2 * M_PI);

///    short sample =(12800*sin(freq + x)) >= 0 ? 0xFFFF : 0x0000;
//      buf[y++]=sample /256;

//    short sample = 128*sin(freq +x);

//    buf[y++]=(char *)sample;
//    buf[y++]=(char *)sample;
//    buf[y++]=0x00;


  }

  write(dsp,bbuf,44100);


//  int z= 0;
  //  for (z=0;z<(freq/fraction);z++)
 //   if(1==1)
  //  {
   //   write(dsp,buf, y);
  //  }

}

int play_midi_fraction(int note, int fraction)
{
  play_note_fraction(notes[note], fraction);
}

int play_midi(int note)
{
  play_note(notes[note]);
}
