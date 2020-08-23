#include <termios.h>
#include <stdio.h>
#include "notes.h"
#include "play.h"

int set_terminal_canon(int on) {
  struct termios info;
  tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
  info.c_cc[VMIN] = 0;          /* don't wait for even one character before returning a read (for lazy single thread) */
  info.c_cc[VTIME] = 0;         /* no timeout */
  if (on) {
    info.c_lflag |= ICANON;     /* turn on canonical mode */
  } else {
    info.c_lflag &= ~ICANON;      /* disable canonical mode */
  }
  tcsetattr(0, TCSANOW, &info); /* set immediately */
}

int x=0;
int main(int argc, char argv[]) {

  init_midi_tables();
  //init_sound();
  init_sound_sox();

  draw_keyboard();

  set_terminal_canon(0);

  char ch = 'a';
  while((ch = getchar()) != 'Q') {
    if (ch>0) {
      int note = key_to_note(ch);
      note+=58;
//      int note = x++;
      ch = note_to_key(note);
      if (note>=0) {
        printf("got char: %c\n", ch);
        printf("Doing stuff.. %c note=%d\n", ch, note);
        draw_keyboard_midi(note);
        play_midi_fraction(note, 2);
      }
    }
  }

  set_terminal_canon(1);

}
