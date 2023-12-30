# SNAKE APPLICATION/GAME USING C, SDL & TTF
#### Video Demo:  <URL HERE>
#### Description:
Snake game made using C, SDL & TTF, SDL was used for graphics, TTF was for fonts and coded the everything else in C. I used youtube to find out how to create a game loop then designed a main menu, a single player mode, exit button and endgame screen. Originally wanted to create a 2 player mode but really wanted to finish before the end of the year so by selecting 2 player mode it simply exits the application. :( Otherwise I am pretty happy about finishing the application. Handling memory was annoying but in the end I finished it.

Located in this zip-file are a bunch of files some of which I do not really know what they are used for I think they have to do with being able to run it inside of visual studio 2022. I have the font which is "Noto". It looked kind of like the old-school fonts that were on the snake game for old phones. License files for licensing, kept it open-source I believe. main.c is the code file. dll files for librarires. Also have another license for open fonts. constants.h holds the file for all my constants. 

contained within the zipfile is my final project. It is an application for the game snake that I made myself. The game is written in C with the help of SDL library to handle graphics and TTF library to handle font/texts. Originally the game was meant to be just a version of snake to be played inside the terminal. But I thought it would be cooler for it to run as an application/window. To do this I had to learn how to setup such a thing. I tried setting it up in the cs50 web side. but realized that wouldn't open a window on my computer. I had been completing assignments using docker and visual studio code but it was difficult to setup an actual window to open from that side. This lead to me switching to using visual studio 2022. I took a long while to setup everything such that I could code inside of visual studio 2022, it was much more difficult to use compared to visual studio code.

I watched a video to setup visual studio code to run C code and how to add the SDL library to my project. THis is the video https://www.youtube.com/watch?v=XfZ6WrV5Z7Y&t=1174s&pp=ygUNc2RsIGdhbWUgbG9vcA%3D%3D. This taught me how to create a simple game loop. From there I was able to create a simple snake game.

Snake version 1. This was made using an array with a really big size, the snake was struct array that was comprised of snake body elements which contained fields such as that body parts x,y coordinates relative to the window, it also contained a field for the width and height of the snake body part. The issue was that if the snake got too big the game would crash. The google version of the game has the snake die if it hits the edge of the game window but I prefer something like pac-man were you get instantly teleported to the other end

Snake version 2. This version I replaced the snake structure with a linked list. This took some time to setup. It was around this time that I forced myself to learn how to use git and github as I suffered a lose of code work and decided that simply copying and pasting my code into a notepad called "rendition_code_x" where x was a number was just a bad idea. Took me a while to learn how to use git and github but once I got the hang of it I learned that its a really powerful form of source control. I included in this version mostly just the linked list.

Snake version 3. This version  I spent a lot of time handling memory and freeing memory. I wanted to create a main menu but didn't know how to set that up. Instead I just spent most of this version cleaning up code and moving things around.

Snake version 4. This version I setup the score board such that a score would appear each time the snake eat something. I was pretty proud of it. My favorite part was that I setup the snake such that the food the snake ate would be a random color and the snake would adopt the color that it last ate. So if it ate pink food the entire snake would be pink.

Snake version 5. This one was quite difficult. This is where I setup the main menu/endgame menu. game flow was as followed. Setup() then Check_inputs(), Update(), Render(). I realized that different parts of the game where simply just different variations of that loop and I needed some form of control to decide what part of the game to show. I setup a variable for the game state and depending on a switch case different functions would run. Main menu had its own setup(),inputs(),Update() and render() functions. these 4 units functions were a part of every screen in the game.

Snake version 6. I spent the majority of this version cleaning everything up. handling memory issues. Setup some modularity with respect to screen size such that if the screen becomes smaller certain buttons become smaller. Also setup selection windows for 1 player and 2 player mode. Though the 2 player mode was never implemented the code exists in place to quickly get it up to speed.

Snake version 7 / final version. This was the final version of my snake game. There was a memory problem where as the game continued to run it would slowly keep eating more memory in the megabytes, which shocked me as I was pretty certain I free'd all my mallocs. It turns out I had a object that was only being rendered once continously keep re-rendering as the game ran through each frame, and it was being given new memory to render the same exact object. I was able to find the problem and clean it up.

I kind of wish I had more time to setup and finish all the components of the game, I still have a lot more ideas to add in but I realized I would never actually "finish" the game as there is always something mroe that can be added.
