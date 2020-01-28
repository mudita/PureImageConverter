# PureImageConverter

## input
Input format is 3 channel (RGB) or 4 channel (RGBA, A for alpha)

Alpha is internally stored as a colour. It gets compared in Renderer.cpp against regular colours. If there is a match with current stroke, then this stroke is skipped.
Therefore alpha isn't really a channel, but a colour i.e. there is no property of alpha. A pixel can be either black, gray, white or translucent. No gradation here.
You can specify which colour will be treated as alpha for displaying purposes. Pass a number (0-15 are valid colours) as the second argument.


## output
VPI - vector format
MPI - bitmap (?) format.

### VPI format
vector means not vector as in SVG, but strokes.
image is composed from strokes of colour from top left corner, horizontally, to bottom right.

#### header
there is a header:
```
uint16_t width
uint16_t height
uint8_t  alpha colour
```
and then raw data.

#### output

The program outputs given vectors as debug info:

```<no. of unique vectors per line> [<vecOffset> <vecLength> <vecColor>]```

e.g. 

```2 [0,10,8][0,22,15]```

