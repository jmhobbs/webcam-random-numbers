# What is it?

1. Scrape the glass off of a webcam.
2. Rip apart a smoke alarm.
3. Smush them together.
4. Use it to generate random numbers with the radiation.
5. Make friends and share your data!

# No really, what is it?

I saw this video [The Webcam Alpha Scintillator](http://www.youtube.com/watch?v=Lvm9gL4KVQg) then [built my own](http://brizzly.com/pic/1P3J) and decided that this might be a good RNG.

So, this git repo is a piece of software that does the following:

1. Loops over the pixels in a frame from a webcam.
2. When it finds a bright enough pixel (from the radiation) it gets the index number.
3. It then shifts the least significant bit of the index int onto a byte.
4. When it has shifted a full bytes worth, it saves the byte to a file.
5. The file should be random data.

# So, is it working?

Short answer, I think so. Long answer, I don't know.

I need to build up a big enough sample of data before I'll know for sure. At that point I'll run a statistical analysis and if it all looks good I'll build something like random.org.

# Hey, ur doin it wrong.

Am I? I know very little about all of the things I am doing here.  If I have a misplaced assumption and you know better, PLEASE let me know so I can fix it or abandon the project.

Thanks, you're a peach.