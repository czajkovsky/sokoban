sokoban
=======

Sokoban 3D in OpenGL.

##Usage

###Bulding
+ building <code>make</code>
+ running <code>./sokoban</code>

###Keyboard
+ <code>arrows</code> moving folk
+ <code>z</code>/<code>x</code>moving camera y-axis
+ <code>c</code>/<code>v</code>moving camera x-axis
+ <code>+</code>/<code>-</code>zooming in/out
+ <code>r</code> restart current level
+ <code>n</code> start next level
+ <code>ESC</code> to quit

##Adding own levels
To add new level you simply have to add <code>n.level</code> file into <code>levels</code> directory (where <code>n</code> is an integer).

Currently maximum level size is <code>10x10</code>.
<code>.level</code> file should start with level size. Then, it should container <code>size * size</code> integers which are linked to fields on the board. Possible values are:
+ <code>9</code> for borders
+ <code>5</code> for folk
+ <code>4</code> for done boxes
+ <code>3</code> for normal boxes
+ <code>2</code> for empty spots
+ <code>1</code> for normal floor
+ <code>0</code> for free space


