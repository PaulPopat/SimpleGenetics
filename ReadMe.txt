-------------------------------------------------------------------------------
--- About Simple Genetics -----------------------------------------------------
-------------------------------------------------------------------------------
Simple Genetics is an Evolutionary Algorithm designed to make interesting
timbres and micro rhythms that slowly evolve over time. It work with target
audio files that can be changed as the algorithm is running. The algorithm will
use these files as a reference but will not create copies so play around and
have fun!

A more detailed description of the process will be given a the bottom of the
read me.

-------------------------------------------------------------------------------
--- Building Simple Genetics --------------------------------------------------
-------------------------------------------------------------------------------
This program requires FFTW to build. The default location is; 

/usr/local/include --Header--
/usr/local/lib --Library--

Other than that it should build happily on Mac OS. It has been tested on Mac Os
El Capitan. Please note that the live build is currently not working because it
cannot work with fftw. I made this before live build was a thing and haven’t
really looked into it. The Xcode project should build fine.


-------------------------------------------------------------------------------
--- Getting something out of Simple Genetics ----------------------------------
-------------------------------------------------------------------------------
The program runs a Evolutionary Algorithm, THIS WILL TAKE A LONG TIME TO
PROCESS. As it calculates it will play back a demo of what is currently being
calculated however it is mono and rough so it isn't exactly what you will get.
The program required you to open or create a save file to run. This is because
it is reliant on a file based audio bin and I don't like writing to temp files
if I can avoid it. After opening or creating a save file from the settings menu
on the menu bar you can just run it although a fresh save won't do much. I have
included a sample save for you to try out.


-------------------------------------------------------------------------------
--- Using the options ---------------------------------------------------------
-------------------------------------------------------------------------------
As this is a Evolutionary Algorithm there are many settings that a standard user
will be unfamiliar with. I will go over each one (including the obvious) here
so that you can be confident with what you are doing.

FFT Frame Size:
	The number of frequency bands in a frame of fft data. There will be 
	multiple frames per Gene/Chromosome.

Frames Per Gene:
	The number of frames of fft data that will be stored in one Gene/Chromosome

Breeding Phases:
	The number of times the algorithm will loop in one run.

Calculation Loops:
	The number of times the algorithm will run with the same population.

Breeding Random Factor:
	The chance of more successful Genes to breed compared to less. Higher
	numbers mean the successful ones have a higher chance.

Frequency Bands:
	How many sections the algorithm will process in. This means that the
	algorithm will split the data into a given number of frequency bands and
	process them complete separately. It will also run them on separate threads
	for better performance.

Targets:
	The target audio files. ctrl click on a target to remove it and drag them
	to move. To add another target double click on it in the audio bin.

Frequency Mutation Weighting:
	Allows you to set how much each frequency will be affected by mutations.
	Most runs will want the high frequencies mutating a lot less. This is for
	that!

Population:
	The population size at any given time.

Mutation Number:
	The number of mutations applied in each breeding phase. These will be added
	randomly across the new population.

Timbre Mutation Amount:
	How much one mutation will affect a timbre Gene. This is will be applied
	before the weighting.

Panning Mutation Amount:
	How much one mutation will affect a panning Gene.

Capture Interval:
	This will affect how often a snap shot will be taken of the best of the
	population. Setting this at one or below will cause each breeding phase to
	take a snapshot (this is not adviced as the result would be notably boring)


-------------------------------------------------------------------------------
--- Detailed Algorithm Description --------------------------------------------
-------------------------------------------------------------------------------
Here is the introduction to Evolutionary Algorithms from my Thesis for this
project:

In this project I am going to use the term Evolutionary Algorithms (EAs) to 
refer to algorithms that simulate survival of the fittest using computers. 
Survival of the fittest is based on the idea of a population of organisms that 
will try to survive as long as they can. Their success on this will be based on
how well suited they are for the environment. The longer they stay alive the 
more likely they are to breed. Breeding will create a new population which is a
mixture of the most successful population members therefore making it better
suited for the environment than the previous population, as it is more focused.
In the breeding process mutations can be formed. This is when a piece of the
genetic material gets corrupted, producing unpredictable results. Sometimes
this mutation can drastically help with survival meaning that it will probably
be introduced into later Generations by the breeding process. Conversely
sometimes a mutation will be detrimental which means that it will be lost as
the population member will die before it can breed.

EAs simulate this with a collections of numbers representing the population.
Each collection of numbers is sometimes referred to as a Gene and this is the
term that I will using in this document. The environment will generally be
another algorithm that defines what to do with each Gene. This can be anything
the programmer wants and is the most open part of this algorithm. Some examples
of what has been done are musical staves where each number in the Gene
represented a Midi note or control data, or a 3d virtual world where the
numbers are collected together to define how a character in this world will
move. In survival of the fittest, the amount of breeding a Gene will do is
based on survival but in a EA this is defined by something called a Metric. A
Metric is a number that represents the success of that Gene and is the primary
definer of how that algorithm evolves which is Generated with an algorithm that
simulates a Gene in the environment to determine how well suited it is. The
breeding is done by combining Genes, normally this is done by picking random
cross over point, cutting each Gene at that point then taking one part from
first Gene and the other from the second. During the breeding process each
number in the new Gene has small chance of being randomly transformed thus
applying mutations. This should hopefully produce an algorithm that
successfully simulates survival of the fittest.

The Metric for this algorithm works on the basis of FFT Data from target audio
files. For each Target an average amplitude will be taken for each frequency 
band. Then the same will be done to each Gene and the two will be compared. For
the panning a location on a 2d plane is worked out by taking each channel of
target and placing it in the space. The average amplitude is taken for each
channel and the difference is taken between them to get a point on the 2d
plane. Each of the panning Genes has a 2d coordinator for each frequency band
the average of those coordinates is taken and compared to the target for the
Metric. To decode the data into audio, channel amplitudes are worked out by
placing the desired number of outputs in a 2d plane and working out amplitudes
for each frequency band. An inverse FFT is taken of Gene with these amplitudes
applied.