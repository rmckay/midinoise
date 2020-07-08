#include <stdio.h>
#include <math.h>
#include "play.h"
#include "notes.h"

double scale[] = { C0, Cs0, D0, Ds0, E0, F0, Fs0, G0, Gs0, A0, As0, B0 };

int
init_midi_tables()
{
  int note=0;
  int scales=0;



  for (scales=0;scales<=10;scales++)
  {

  // print out one scale
    for(note=0;note<12;note++)
    {
      double curnote = scale[note]*pow(2,scales);
      printf("adding %d * %d = %d note\n", scales,note,scales*note);
      int addnote = (scales*12)+note;
      notes[addnote]=curnote;
      printf("Note = %d Scale = %d Note %d = %f\n", addnote, scales, note, curnote);
//      play_note(curnote);
//      sleep(1);
    }

  }


}

