#include <stdio.h>
#include <string.h>
#include <ctype.h>


void draw_keyboard_pressed(char key) {

char keyboard[] = "\
 ___________________________________________________________________  \n\
|  | | | |  |  | | | | | |  |  | | | |  |  | | | | | |  |  | | | |  | \n\
|  | | | |  |  | | | | | |  |  | | | |  |  | | | | | |  |  | | | |  | \n\
|  |2| |3|  |  |5| |6| |7|  |  |9| |0|  |  |S| |D| |F|  |  |H| |J|  | \n\
|  |_| |_|  |  |_| |_| |_|  |  |_| |_|  |  |_| |_| |_|  |  |_| |_|  | \n\
|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   | \n\
| Q | W | E | R | T | Y | U | I | O | P | Z | X | C | V | B | N | M | \n\
|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___| \n";

char buf[1024];

strcpy(buf, keyboard);

char * pos = strchr(buf, toupper(key));
if (pos)  {
  printf("Found %c at %x\n", key, pos);
  *pos = '*'; // change to *
  printf("ok changed?\n");
}

printf("%s", buf);

}

void draw_keyboard() {
  draw_keyboard_pressed('%'); // no % present so no replacement
}


// { C0, Cs0, D0, Ds0, E0, F0, Fs0, G0, Gs0, A0, As0, B0 };

int key_to_note(char key) {

  switch(toupper(key)) {

    case 'Q':
	return 0; // C0
    case '2':
        return 1; // Cs0
    case 'W':
        return 2; // D0
    case '3':
        return 3; // Ds0
    case 'E':
        return 4; // E0
    case 'R':
        return 5; // F0
    case '5':
        return 6; // Fs0
    case 'T':
        return 7; // G0
    case '6':
        return 8; // Gs0
    case 'Y':
        return 9; // A0
    case '7':
        return 10; // As0
    case 'U':
        return 11; // B0
    case 'I':
        return 12; // C1
    case '9':
        return 13; // Cs1
    case 'O':
        return 14; // D1
    case '0':
        return 15; // Ds1
    case 'P':
        return 16; // E1
    case 'Z':
        return 17; // F1
    case 'S':
        return 18; // Fs1
    case 'X':
        return 19; // G1
    case 'D':
        return 20; // Gs1
    case 'C':
        return 21; // A1
    case 'F':
        return 22; // As1
    case 'V':
        return 23; // B1
    case 'B':
        return 24; // C2
    case 'H':
        return 25; // Cs2
    case 'N':
        return 26; // D2
    case 'J':
        return 27; // Ds2
    case 'M':
        return 28; // E2

    default:
        return -1;

  }

}

char note_to_key(int note) {
  switch(note) { 
    case 0:
      return 'Q';
    case 1:
      return '2';
    case 2:
      return 'W';
    case 3:
      return '3';
    case 4: 
      return 'E';
    case 5:
      return 'R';
    case 6:
      return '5';
    case 7:
      return 'T';
    case 8: 
      return '6';
    case 9:
      return 'Y';
    case 10:
      return '7';
    case 11:
      return 'U';
    case 12:
      return 'I';
    case 13:
      return '9';
    case 14:
      return 'O';
    case 15:
      return '0';
    case 16:
      return 'P';
    case 17:
      return 'Z';
    case 18:
      return 'S';
    case 19:
      return 'X';
    case 20:
      return 'D';
    case 21:
      return 'C';
    case 22:
      return 'F';
    case 23:
      return 'V';
    case 24:
      return 'B';
    case 25:
      return 'H';
    case 26:
      return 'N';
    case 27:
      return 'J';
    case 28:
      return 'M';

    default:
      return '%';
  }

  

}

void draw_keyboard_midi(int note) {
  printf("Drawing keyboard: note %d, modnote %d\n", note, note % 28);
  draw_keyboard_pressed(note_to_key(note % 29));
}
