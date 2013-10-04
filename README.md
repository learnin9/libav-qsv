This is fork repository of https://github.com/lu-zero/libav/tree/qsv-simple

## Requirements
   * Intel Media SDK 2013 R2 : http://software.intel.com/en-us/vcsource/tools/media-sdk
   * mfx_dispatch : https://github.com/lu-zero/mfx_dispatch

## Build
   * https://github.com/lu-zero/mfx_dispatch/wiki/Libav-integration

## Support
   * Intel Media SDK API version : 1.1
   * Codec : H.264, MPEG-2
   * Rate control method : CBR, VBR, CQP

## Codec name
   * H.264 : `h264_qsv`
   * MPEG-2 : `mpeg2_qsv`

## Rate control option
   * CBR : `-b 2000k -maxrate 2000k`
   * VBR : `-b 4000k`
   * CQP : `-q 20`

## Option
   * AVCodec : `b, maxrate, q, g, bf, refs, slices, coder +vlc, flags -cgop`
   * QSV : `async_depth, timeout, qpi, qpp, qpb, idr_interval, profile, level`

## Example
QSV encode
`avconv -i "input.ts" -b 2000k -maxrate 2000k -c:v h264_qsv "output.mp4"`
`avconv -i "input.ts" -b 4000k -c:v h264_qsv "output.mp4"`
`avconv -i "input.ts" -q 20 -c:v h264_qsv "output.mp4"`
`avconv -i "input.mp4" -q 20 -c:v mpeg2_qsv "output.ts"`
`avconv -vsync cfr -i "input.mpg" -q 25 -i_qfactor 1 -i_qoffset -3 -b_qfactor 1 -b_qoffset 2 -c:v h264_qsv "output.mp4"`

QSV decode/encode
`avconv -c:v mpeg2_qsv -i "input.mpg" -q 20 -c:v h264_qsv "output.mp4"`
`avconv -c:v h264_qsv -i "input.mp4" -b 8000k -maxrate 9800k -c:v mpeg2_qsv "output.mpg"`
