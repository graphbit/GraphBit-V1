C# files contains a software allowing anyone to send pictures and gifs via serial port to the LCD controller.
The software also allows users to get an array representing an imported picture.

It works by resizing the picture to settings entered via the user, converting this resized picture in black and white, and then combining
pixels, so that 8 pixels enter 1 byte. 
At this point, you can either send this array via serial port, or copy the array to paste it into another program or code.
