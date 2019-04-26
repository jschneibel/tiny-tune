# tiny-tune
This is a sample program that plays a tune on an Arduino. The program has been optimized to consume 
as little SRAM (variable memory) as possible for long melodies. Furthermore, other code can
be executed while the tune is played, since no delays are used to play the tunes.

In order to run the program, compile and upload the following files to your Arduino:
- tiny-tune.ino
- tunes.ino
- pitches.h
- libraries/ArduinoThread (library by Ivan Seidel)

Use playTune() to start playing the tune.
Use cancelTune() to stop playing the tune.
Edit getTuneData() in tunes.ino to change the sample tune or add your own tunes.
