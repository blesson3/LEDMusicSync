# LEDMusicSync

*This project is under heavy development, not suitable for use beyond tinker at the moment*

## Goals
- Create an automated system where LEDs can be synced to music
- This system must have the following requirements...
  - Any arbitrary song can be played
  - Limited user interaction (preferrably all 'client' side)
  - Any song can also be pre-programmed using a set number of sequences
  - Has multiple sequences that can be turned on without the need for music
  - Can map the LEDs in different patterns depending on their physical shape/pattern

## Components
- LED Strip(s) (WS2811)
- Arduino UNO
  - FastLED
- Raspberry Pi (3rd gen) (communicating to the arduino through serial)
  - RPi -> Arduino LED protocol
- Client (the device playing the music) (my MacBook Pro for now)
  - Some sort of sync handshake between the client and raspberry pi

## (Current) Limitations
- Specific hardware dependence
- Potentially messy standup
- Songs need to be manually pre-programmed before the system can run through them
- ...

## Issues I've Faced
- The RPi does not do well playing music in general
  - Headphone jack seems to have very poor output (without any additional shields)
    - One problem was a lot of white-noise, I fixed some (not all) of those issues here:
      1. http://bit.ly/2A262ui
      2. http://bit.ly/2zEKROF
  - Built in music player (`omxplayer example.mp3`) played music slowly
  - Python specific music player (`pygame mixer`) output the following errors
      - `pygame.error: No available audio device`
      - `Home directory not accessible: Permission denied`
    - I made no progress with either of these issues
    - Albeit, it's likely I've messed with some configuration that caused these issues
