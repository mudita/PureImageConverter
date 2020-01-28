# PureImageConverter

## usage
Just call `./vimgconvert <path_to_file.png>`

## input
Input format is 3 channel (RGB) or 4 channel (RGBA, A for alpha).

Alpha (transparency) works like this: any alpha value other than fully opaque (255) will cause given pixel not to be drawn. Alpha channel is essentially a mask with threshold of 254.

Exapmle images:

- transparent backgroud (32×32, monochrome objects, backgroud alpha=0):

![kształty](https://user-images.githubusercontent.com/56958031/73300262-0260d880-4211-11ea-9f66-9c5b5abeb385.png)

- 16-step grayscale gradient (32×32, 16 gray levels ranging from 0 to 255, no alpha channel)

![grad](https://user-images.githubusercontent.com/56958031/73352007-78f0eb00-4290-11ea-80cf-f616b0b8d88d.png)

## alpha details
Alpha is internally stored as a colour. It gets compared in Renderer.cpp against regular colours. If there is a match with current stroke, then this stroke is skipped.
Therefore alpha isn't really a channel, but a colour i.e. there is no property of alpha. A pixel can be either black, gray, white or translucent. No gradation here.

By default it is a colour number 16 as this doesn't make sense as a colour. There are only 16 colours, namely 0-15.
You can override this defualt behaviour by specifying which colour will be treated as alpha for displaying purposes. To do so, pass a number (0-15 are valid colours) as the second argument e.g. `./vimgconvert image.png 3`


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

