# LoonixEditor
A mildly temperamental console-based text editor for linux.

## Notice!
The LoonixEditor console-based text editor for linux is currently being rewritten.
Honestly, I didn't know what I was doing when I first wrote LoonixEditor. Because of this,
an editor full of horrible code practices and bugs was formed. I intend on rewriting from the
ground up, only borrowing some of the newer which I think to be "acceptable".\
\
Hopefully, the new editor (which will keep the original name) will run faster, be less buggy,
and be a nicer project to work on as a whole. This rewrite may take a while but hopefully, it will
be worth it in the long run.\
\
As well as being a complete code rewrite, I plan on adding many new features that just wouldn't be possible
with the current state of the project. Such features include:
- Lua plugin support
- Multiple view editing
- Integrated file manager
- Custom keyboard shortcuts
- Better menus

I will move this version of the editor to its own branch where it may rest for the rest of time, I will most likely
call the branch LegacyEditor (a play on LoonixEditor).\
\
Fun fact is, this isn't the first iteration of LoonixEditor. The original working title for this was
LoonixEditor 2 as it was a rewrite of an even worse project.\
\
We can only hope the best for LoonixEditor 3\
See you then :^)

<img src="https://i.imgur.com/ERov6y6.png" width="552px">

**LoonixEditor running with dark header enabled and vim theme**

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
