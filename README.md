# LoonixEditor
A mildly temperamental console-based text editor for linux.

<img src="https://i.imgur.com/DlZIlcF.png" width="41%">

## How to use
### Key binds
**F1** Help menu\
**Ctrl + N** New file\
**Ctrl + O** Open file\
**Ctrl + S** Save file\
**Ctrl + A** Save as filename\
**Ctrl + X** Close file\
\
**F6** Diff bar toggle\
**F7** Low CPU toggle\
\
**Ctrl + Alt + RightArrow** Move to next file\
**Ctrl + Alt + LeftArrow** Move to previous file\
**Alt + RightArrow** Move to next file\
**Alt + LeftArrow** Move to previous file\
\
**Ctrl + F** Find string\
**Ctrl + R** Find and replace\
**Ctrl + U** Jump to line\
**Ctrl + L** Line info\
**Ctrl + T** Todo Manager\
**Ctrl + D** Duplicate line\
\
**Ctrl + K** Cut line\
**Ctrl + /** Comment line\
**Ctrl + Alt + Up** Move line up\
**Ctrl + Alt + Down** Move line down\

## Build
### Windows
You can compile LoonixEditor on windows however you will need tools such as the linux subsystem\
or git bash to compile and run it in.

### Linux
With linux you can compile LoonixEditor using g++\
<code>g++ LEditor.cpp</code>
\
\
If you get any errors that references c++98 try the following command\
<code>g++ -std=c++11 LEditor.cpp</code>
\
\
*Loonix editor doesn't use any third-party libraries so it should be compatible with any modern c++ compilers.*

## Bugs
If you find any bug within LoonixEditor feel free to open a new issue and ill get round to looking at it,\
please try to include a detailed description of the bug and how you got it to trigger.

## Releases
Semi-regular releases will be issued to github.\
If you have any issues with the releases try building from source.

## Authors

* [Satomatic](http://www.github.com/Satomatic) - (Brian Thomson)
