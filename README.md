Keep Awake

A 'dead mans' button.

If key is not pressed in due time the beeper will sound.

Long press at the key configures the time accepted between keypresses.

A LED will blink according to time being set. Configured by delayTable[]

Default is:
	3 minutes - 1 blink
	6 minutes - 2 blink
	9 minutes - 3 blink


Hardware: 

Connect a beeper to 'beepOut' - default Digital 4
Connect an LED (anode) to 'ledOut' - default Digital 13. LED (katode to gnd)
Connect a push button to 'keyIn' - default Digital 3. 
Also pull high Digital 3 via e.g. 100K resistor 
