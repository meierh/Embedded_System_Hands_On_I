# Audio files for DFPlayer

The ringtones and mp3 player files are located in the `mp3` and `ringtones` subfolders. The `mp3` folder contains the audio files for the mp3 player, while the `ringtones` folder contains the ringtones for the alarm clock.

Use 
```bash
./convert.sh
```
to convert and rename the audio files s.t. the DFPlayer recognizes them. The script requires `lame` to be installed.

The script will create a folder `out` and two subfolders `01` and `02`. Copy `01` and `02` to the root of a FAT32-formatted SD card and insert it into the DFPlayer Mini.