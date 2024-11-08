Wh is need:
WeAct studio Black pill stm32f411ceu6
Any working pcm5102 shield configured as slave in i2s Phillips standard
Any working pcm1808 shield configured as slave in i2s Phillips standard

Big Steps for implementing
 make test board
 write firmware fo stm32
 write linux device driver for orange pi

Wh implemented as demo
 simple i2s-i2s bridge firmware for stm32 (in+out) in 48k 24bit 256fs
	opi worked as i2s master
	stm32<->opi-mcu bridge as slave with internal clocking
	stm32 i2s master reciever with internal clock and mclock/bclock output
	stm32 i2s master transmitter with internal clock and mclock/bclock utput
	clock dividers for 48/96/192K audio processing and internal i2s pll reconf +- 0.04% clock error

 simple stm32-i2s kernel audio device driver for use ALSA/JACKd (audio in and audio out)
 simple overlay for enabling i2s in Orange PI PC/ONE in 40pin GPIO header. Implement as 48k 24bit 256fs only without bclck change

Complete tracing shield

2024/Nov/08
Received printed PCB
![Printed PCB1](./docs/IMG_0718.jpg)
![Printed PCB2](./docs/IMG_0719.jpg)
There was an error in placing the socket for 3PIN UART socket from orange PI to shield. This will not interfere with functionality, but will be fixed in the next version of the board.
