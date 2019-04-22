/*
Copyright (c) 2019 Jonathan Schneibel
MIT License
GitHub: https://github.com/jschneibel/tiny-tune

This is a sample program that plays a tune on an Arduino. The program has been optimized to consume 
as little SRAM (variable memory) as possible for long melodies. Furthermore, additional code can
be executed while the tune is played.

In order to run the program, compile and upload the following files to your Arduino:
- tiny-tune.ino
- tunes.ino
- pitches.h
- libraries/ArduinoThread (library by Ivan Seidel)

Use playTune() to start playing the tune.
Use cancelTune() to stop playing the tune.
Edit getTuneData() in tunes.ino to change the sample tune or add your own tunes.
*/

#include <Thread.h>
#include <ThreadController.h>

// global variables used for playing tunes
#define BUZZER_PIN 2
#define UPDATE_INTERVAL 1000
int currentNote = 0;  // buffer variable for notes (only one note gets loaded into SRAM at a time)
byte currentNoteIndex = 0;
byte maxNoteIndex = 1;
int currentNoteDuration = 100; // buffer variable for note durations (only one note duration gets loaded into SRAM at a time)
int pauseBetweenNotes = 100;
bool playPauseNext = false;

ThreadController threadController = ThreadController();
Thread tuneThread = Thread();

// Callback function for tuneThread.
void tuneCallback() {
  playCurrentNote();
}

void setup() {  
  // Configure threads.
  tuneThread.onRun(tuneCallback);
  tuneThread.setInterval(100);
  tuneThread.enabled = false;	// no tune playing on setup
  threadController.add(&tuneThread);

  // Start playing the tune in tunes.ino. The rest of the code will keep executing.
  playTune();

  // Additional code here.
}

void loop() {
  noInterrupts();
  // Run threads in threadController, in case their set intervals have passed.
  threadController.run();
  interrupts();

  // Additional code here.
}
