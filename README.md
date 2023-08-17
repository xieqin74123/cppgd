# cppgd
CPP version of Gdata programme. Machine-learning-oriented data management and convertion for Gaussian output and Chargemol output

## Build guide
    make cppgd

## Usage guide
    Usage: cppgd -i<input format> [<input option>] <input path> -o<output format> <output path> [options]
    
    You can also directly run this programme without any argument
    
    Options:
    --help       show this help
    
    Supported input format:
    -imol2       .mol2 
    -iglog       .log    gaussian output log. Can specific charge type with --mulliken (default), --hirshfeld or --cm5
    
    Supported output format:
    -oxyz        .xyz 
    -onpy        .npy    numpy data series files (output path should be folder)

## Acknowledgement
npy.hpp is written by github user llohse (Leon Merten Lohse). Original page can be access [here](https://github.com/llohse/libnpy/tree/master).
