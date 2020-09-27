# LoonixEditor
A mildly temperamental console-based text editor for linux.

**Notice**\
This branch is a quick patch that makes LoonixEditor usable however there are alot of functions / keybinding that don't work.\
Also, don't expect this branch to be updated very often.

<img src="https://i.imgur.com/HP8RcEW.png" width="552px">

**LoonixEditor running with dark header enabled**

## How to use
### Key binds

**F1** Help menu\
**F5** Option Menu\
**Ctrl + N** New file\
**Ctrl + O** Open file\
**Ctrl + S** Save file\
**Ctrl + A** Save as filename\
**Ctrl + Alt + O** Open in current buffer\
**Ctrl + X** Close file\
\
**F6** Diff bar toggle\
**F7** Low CPU toggle\
**F8** Indentation guide toggle\
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
**Ctrl + Up** Move line up\
**Ctrl + Down** Move line down\

### Themes
Themes can be placed in `~/.ledit/themes` directory. By running setup.sh, the directory will be generated and the packaged themes will be copied in.\
To change the current theme you can go into `~/.ledit/main.config` and change the `theme` value.

If you want to create your own theme I would recommend copying the default theme and altering the values. The values are just ansi escape sequences so you can customize pretty deeply.

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
