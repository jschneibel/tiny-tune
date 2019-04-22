/*
Copyright (c) 2019 Jonathan Schneibel
MIT License
GitHub: https://github.com/jschneibel/tiny-tune

Edit getTuneData() to change the sample tune or add your own tunes.
*/

#include "pitches.h"
#include <avr/pgmspace.h>

// Read the note and note duration at specified index from program memory and store them in global variables.
void getTuneData(byte index)
{
  /////////////////////////////////////////////////////////////////////////////////////////
  // The notes and their durations are stored in PROGMEM (program memory aka flash memory):
  const static uint16_t sample_tune[] PROGMEM = {
      NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6,
      NOTE_B5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5};
  const static byte sample_tempo[] PROGMEM = {
      8, 8, 8, 8, 8, 8, 8, 8,
      8, 8, 8, 8, 8, 8, 8};
  /////////////////////////////////////////////////////////////////////////////////////////

  // The currentNote and currentNoteDuration are global buffer variables so that loading the notes and their durations of the tune
  // won't use up all our SRAM (in case the tune is very long):
  currentNote = pgm_read_word_near(sample_tune + index);  // Read current note from program memory.
  currentNoteDuration = 1500/pgm_read_byte_near(sample_tempo + index);  // Read current note duration from program memory and convert to milliseconds.

  // Read max index of array in program memory.
  maxNoteIndex = sizeof(sample_tune) / sizeof(sample_tune[0]) - 1;
}


// Start playing the tune.
void playTune() {
  cancelTune();
  
  currentNote = 0;
  currentNoteIndex = 0;
  maxNoteIndex = 8;
  currentNoteDuration = 100;
  pauseBetweenNotes = 100;
  playPauseNext = false;

  tuneThread.enabled = true;
}


// Cancel tune that is playing at the moment.
void cancelTune() {
  noTone(BUZZER_PIN);
  //tuneThread.setInterval(1000);
  tuneThread.enabled = false;
}


// Play a single note or pause of the tune.
void playCurrentNote() {
  if(playPauseNext == false) {	// if a note should be played
      // Read note and note duration from program memory and store them in global variables.
      getTuneData(currentNoteIndex);

      // There has to be a short pause between notes, otherwise the tune will not play smoothly.
      // Feel free to experiment with this.
      pauseBetweenNotes = currentNoteDuration * 0.30;

      // Play note (the code will keep executing without delay).
      tone(BUZZER_PIN, currentNote, currentNoteDuration);

      // Repeat tune from the beginning after maxNoteIndex (end of tune) has been reached.
      if (currentNoteIndex == maxNoteIndex) currentNoteIndex = 0;
      else  currentNoteIndex++;

      // Call tuneThread again when the current note has finished playing.
      tuneThread.setInterval(currentNoteDuration);

      // After the current note, a pause will be played.
      playPauseNext = true;
  }
  else {	// if a pause should be played
      noTone(BUZZER_PIN);

      // Call tuneThread again when the current pause has finished playing.
      tuneThread.setInterval(pauseBetweenNotes);

      // After the current pause, a note will be played.
      playPauseNext = false;
  }
}
