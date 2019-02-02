# Wav Visualizer

This program parses all data from file of .wav format and prints it to the screen.
It is also transfer music data into accessory program for showing graph. Accessory program: https://github.com/Erigara/Audio_visualization

## Libs installation

This program uses ncurses libs, so you should have ones. For installing on Ubuntu type:

```bash
sudo apt install libncurses5-dev libncursesw5-dev
```

To install libs for accessory program, please, follow https://github.com/Erigara/Audio_visualization

## Installation

I am recomending this instalation:

```bash
git clone https://github.com/Arjentix/Wav-Visualizer
git clone https://github.com/Erigara/Audio_visualization
cd Wav-Visualizer/
ln -s ../Audio_visualization/Main.py graph_program # Creating sym link
make clean && make
```

## Using

```bash
./wav_visualizer <.wav file> -c 'python3 graph_program'
```

## API

Such a big word, but I'm providing only music data for another program, which should show them on the graph. You can write your own program, doing this, and connect to my program by just passing it in arguments with flag -c (see *Using*).
Only one requirement for your program: it should takes all data from standart input in format "%d %d %d ..." (%d - is usigned short num in range of [0; 2^16).