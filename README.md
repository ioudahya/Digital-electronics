# electronics-with-psoc

# what to do 

![image](https://user-images.githubusercontent.com/44835548/208648176-f7843be6-94c6-4a3c-b8a1-7575c27f02ab.png)

# Tasks
- SW1: When pressing switch SW1, the LEDs should light up and the audio output should
emit a sound as long as the switch is pressed. When the switch is unpressed, the LEDs should
go out and the audio output should stop emitting a sound.
- SW2: When pressing switch SW2, the LEDs and audio output should emit a short morse
signal. The short morse signal should be 250 ms long.
- SW3: When pressing switch SW3, the LEDs and audio output should emit a long morse
signal. The long morse signal should be 750 ms long.
- SW4 and Pot: When turning the potentiometer (Pot), the flag should be going up according
to the turning of the potentiometer. When pressing SW4, we change the flag that is controlled
with the potentiometer.
- Photoresistor: The photoresistor can detect whether there is ambient light or not. When
there is light, only the audio output and semaphore flags should be active. When there is no
light, only the audio output and the LEDs should be active.
- Keyboard: We can use the keyboard to send a series of pre-recorded message. For example,
pressing “1” and “*” should start sending “SOS” in morse and semaphore code. Pressing “2”
and “*” should send “BEAMS” in morse and semaphore code. Other messages are for you to
chose.
- Serial port: The user computer can send ASCII characters through the serial port, which
are then sent in morse and semaphore code by the distress beacon. The microcontroller also
sends two types of characters to the user computer over the serial ports: “.” and “-” (which
represents short and long morse signals that are sent by the distress beacon).
- LCD screen: The LCD prints the ASCII characters that are sent by the distress beacon (this
feature can only be used when using the keyboard or the serial port). You can also chose to
print additional information on the LCD screen
