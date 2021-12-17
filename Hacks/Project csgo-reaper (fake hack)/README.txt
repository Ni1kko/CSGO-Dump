Hi! :)

Read this document if you want to understand how everything works.

Disclaimer:
This project is written in C# and is 100% external without memory writing. 
The main reason for me to share the source code is to serve as evidence 
that my videos are not faked. If you have come here to learn programming 
this is not a good place to start and you should google real tutorials 
instead. This project is poorly documented, poorly written with a lot of 
hacky solutions or copy pasted snippets.

MemoryReading
This project reads the memory to get information from the game. 
Current memory offsets are grabed from my repo on program start.
I try keeping the offsets up to date with every game update but feel free to load
your own offsets by creating file /offsets/offsets.txt

MemoryWriting
This project _do not_ rely on memory writing at all. There was simply no need
to write memory to accomplish what was needed for the purpose of this bait software.

How do I send console commands without writing memory?
This is a bit hacky but gets the job done: In csgo you can bind keys to execute console commands from
cfg files stored in the csgo directory. The fake cheat manipulates the cheaters config file
and adds a keybinding to execute a cfg file called cheater.cfg. Whatever console command I want
to send I just write it to that file then programatically simulate pressing the key bound to 
execute the cheater.cfg
 
Debugging
In program.cs I have prepared debugging settings and turned off running the process in background
when you close the main window. Play around with the debugging settings or remove all of them to
make it run like it would for a real cheater.

Third party stuff
This project uses third party references like google drive api, sharpdx, costura and more.
I have also tried to credit the authors in the classes where I have used copy paste code or
based code on someone else code or tutorial.

How do I create tripwires?
In the helper class I have a method that reads the coordinates in the correct
format that is used when adding tripwires. Step 1: Start the fake cheat, Step 2: Go to the location
in-game where you want to setup a tripwire. Step 3: Press 'p' 4 times (one for each corner of the
tripwire in-game, remember to move to each corner in the gameworld before each keypress).
Step 4: copy the coordinates from visual studio output (generated every 4th press of 'p' key)
Step 5: create new TripWire instance and use the coordinats you collected from output.
You can press 'c' key to reset the 'p' press counter (to manually start over).
The z value can be used to make a tripwire only trigger on a certain height of the map or leave
it at 0 to make it trigger no matter how high or low the player enters the tripwire on the map.

// ScriptKid
