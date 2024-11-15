# wcmp (wcmp Curses Music Player)

A console music player for windows inspired by [cmus](https://github.com/cmus/cmus) and [ncmpcpp](https://github.com/ncmpcpp/ncmpcpp). 

## features 

- Lightweight with minimal RAM and CPU usage 
- Vi-inspired keybindings
- Vi-like editing of play queue with `d` and `p`/`P` pasting
- Support for mp3, wav, ogg, flac, etc files through [raudio](https://github.com/raysan5/raudio). 
- Support for creating and loading playlists.

## keybindings

### UI navigation
- `j` - Move down
- `k` - Move up
- `g` - Jump to top
- `G` - Jump to bottom
- `<Escape>` - Exit Program / Exit text input mode

## menu Navigation 
- `1` - Library view
- `2` - Play queue view
- `s` - Save play queue as playlist 
- `/` - Search for songs (case insensitive)

### song controls 
- `<Space>` - Pause/unpause current song
- `n` - Play next song 
- `<Enter>` - Play song under cursor
- `l` - Seek forward 5 seconds
- `h` - Seek backward 5 seconds

### play queue editing
- `a` - Append current song or playlist to play queue
- `d` - Cut from play queue 
- `p` - Paste below into play queue
- `P` - Paste above into play queue

## build instructions 

Ensure that `gcc` is installed and on the path. Run `make build`

## roadmap

- Refactor
- Support for reading metadata 

## notes 

- Supports a maximum of 400 files. Edit the `maxFiles` value in `wcmp.c` to support more files. This uses more RAM. 
- tcc is not supported.
