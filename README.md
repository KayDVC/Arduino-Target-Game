## Table of Contents
1. [Objective](#objective)
2. [Use](#use)
3. [Final Thoughts](#final-thoughts)


## Objective

The objective of this project was to develop utilize the Software Development Life Cycle (SDLC) to design and develop a game that a person may find at a local fair, carnival, or festival. This was completed as a group project in 2022.

**_WARNING: THIS IS NOT A TESTED OR FINISHED PRODUCT._**

[Click here](https://docs.google.com/presentation/d/17qe99xAeDvr7stO-mAHYxtit5gIiJAZH/edit#slide=id.g1a4fcf87d83_0_5) for the project's presentation. It's nicer on the eyes.

_In short_: 
The game is based off of a simple laser shooting game where a player can shoot multiple targets for a set period of time. Each target awards a player a random amount of points that is added to their 'score.' If the player's score is higher than some set amount at the end of the alloted time, the player wins; if not... boo hoo – cry about it.


_Some Neat Features the software already supports ... maybe_: 

- Using multiple shift registers for both input and output to increase the number of IO pins available for use.
- The number of sensors that will be considered 'active'(awards the player with points when hit) is customizable. 'Active' sensors are chosen at random on a set interval.
- LEDs are paired with a LDR resistor. When that resistor is 'active', the paired LED should also light up. 


## Use

The intended hardware for configuration for the software is as follows:
- Arduino Uno Rev3
- 74HC959(Out) Shift Register(s)
- 74HC165(In) Shift Register(s)
- LEDs (same amount as LDR resistors)
- LDR Resistor(s) (same amount as LEDs)
- Any laser / redirectable light source.
- Any I2C compatible display. The larger, the better.

See [assets folder](./assets/) for reference pinouts.

## Final Thoughts

**I want to repeat this once more: The code has not been tested on HW.**

Though the project was a "team-project" with ~ 6 members, all soft/hardware design was done by me .... without access to hardware to actually test on :smiley:.

I usually like to post finished, working products, but sometimes, the effort is enough. Learning doesn't always look pretty. With so many unknowns and such little time, I'm proud of what I was able to accomplish. 

I have no intentions of continuing the project, but feel free to leave suggestions or improvements.

If used as "inspiration," please link back to this repo.

Thanks,

\- Kay

