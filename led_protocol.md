# Fading Serial LED Protocol

## Purpose
- For the Raspberry Pi to drive the high level LED patterns

### Why the hell are you using an Arduino to drive the pixels and not the RPi itself?!?!
- For added complexity ;)
- (jk)
- I'm lazy, looking for a challenge, and can't find a reliable resource/tutorial to walk me through it :P
- (plus, I have to use C to program for arduino :D)

## The (probably inefficient) Protocol!

### Definitions

- Section
  - This is a textual-representation of a range
  - A section is made up of two components  
    1. an index
    2. length
  - Example:
    - `0,10` includes the indexes [0, 1, 2, 3...8, 9]
    - `57,3` includes the indexes [57, 58, 59]
  - Nothin' you haven't seen before
- Instruction (a little more vague)
  - This is the texual-representation of the command
  - One or more instructions convey a sequence
  - The `&` character determines the end of an instruction
  - Example:
    - `2|0,10|20,30|r|100|&`
  - The validity conditions for instructions are below...
- Color
  - The RGB value that is set to a given section of LEDs
  - This is currently a single character denoting primitive color 
  - Example:
    - `r` - red
    - `w` - white

### Requirements

- The protocol must support...
  - Multiple ranges of LEDs
  - Different fade timings for an instruction
  - Colors

### The Protocol

- The textual representation of the protocol is a pipe-delimited string ending in the character `&`
- The basic protocol is structured like the following:
  - `num_sections|sections...|color|fade_delta|&`
  - The protocol MUST end with `|&` in order to denote the end of each individual instruction

- Valid values:
  - `num_sections` must be greater than 0 and less than or equal to the total number of LEDs
  - `sections` must follow the following format `index,length` (see above)
    - `index` must be greater than or equal to 0 and less than the total number of LEDs
    - `length` must be positive
  - `color` must be one of the following values:
    - `r` - Red    (255, 0,   0  )
    - `g` - Green  (0,   255, 0  )
    - `b` - Blue   (0,   0,   255)
    - `w` - White  (255, 255, 255)
  - `fade_delta`
    - TBD
