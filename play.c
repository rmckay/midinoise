#include <stdio.h>
#include <linux/soundcard.h>
#include <X11/Xlib.h>
#include "play.h"
#include "notes.h"

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

int play_note(double note)
{
  play_note_fraction(note, 1000);
}

int play_silence(int fraction) {
  char buf[4410000];
  memset(buf, 0, sizeof(buf));
  write(dsp, buf, 1000);
}

int play_notes(char *playing) {

  char buf[4410000];
  int rate = 44100;
  int x=0;
  int c=0;
  char by;
  int y=0;
  int fraction = 80; // just cause

  for (int i=0;i<128;i++) {
    int np = playing[i];
    if (np!=False) {
      double freq = np / 2;
      double rep = (rate / freq);
      y = 0;
      for (x = 0; x < 65536; x += (65536.0 / rep)) {
        buf[y++] = 2 ^ x - 1;
      }
    }

  }



  int z= 0;
  for (z=0;z<(rate/fraction);z++)
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

  for (x=0;x<65536;x+=(65536.0/rep))
  {
    buf[y++]=2^x-1;
  }

  int z= 0;
    for (z=0;z<(freq/fraction);z++)
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
