# wcmp (Windows Curses Music Player)

A console music player for windows inspired by (cmus)[https://github.com/cmus/cmus] and (ncmpcpp)[https://github.com/ncmpcpp/ncmpcpp]. 

# features 

- Lightweight with minimal RAM and CPU usage 
- Vi-inspired keybindings
- Vi-like editing of play queue with `d` and `p`/`P` pasting
- Support for mp3, wav, ogg, flac, etc files through (raudio)[https://github.com/raysan5/raudio]. 
- Support for creating and loading playlists.


# keybindings

### UI navigation
- `j` - Move down
- `k` - Move up

### song eontrols 
- `<Space>` - Pause/unpause current song
- `n` - Play next song 
- `<Enter>` - Play song under cursor

### play queue editing
- `a` - Append song or playlist to play queue
- `d` - Cut from play queue 
- `p` - Paste below into play queue
- `P` - Paste above into play queue

# build instructions 

Ensure that `gcc` is installed and on the path. Run `make build`

# roadmap

- Seeking back and forth
- Searching via file names 
- Refactor
- Support for reading metadata 

# notes 

- Supports a maximum of 400 files. Edit the `maxFiles` value in `wcmp.c` to support more files. This uses more RAM. 
- tcc is not supported.
