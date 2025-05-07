#!/bin/bash

###################################################################################################
# raw unencoded example
gst-launch-1.0 -v videotestsrc ! video/x-raw,width=320,height=240,framerate=20/1,format=I420 ! rtpvrawpay ! udpsink host="192.168.2.247" port=5000

gst-launch-1.0 -v udpsrc port=5000 caps=application/x-rtp,encoding-name=RAW,width=320,height=240,framerate=20/1,payload=96,format=I420 ! \
    rtpvrawdepay ! capsfilter caps="video/x-raw,width=720,height=1280" ! videoconvert ! videoscale ! kmssink force-modesetting=true

gst-launch-1.0 -v udpsrc port=5000 caps = "application/x-rtp, media=(string)video, clock-rate=(int)90000, \
    encoding-name=(string)RAW, width=(string)320, height=(string)240, \
    payload=(int)96, sampling=(string)RGBA, depth=(string)8, a-framerate=10/1" ! \
    rtpvrawdepay ! videoscale ! videoconvert ! queue ! kmssink force-modesetting=true

egt_video --width 320 --height 240 --pipeline "udpsrc port=5000 caps=application/x-rtp,encoding-name=RAW,payload=96 \
    ! rtpvrawdepay ! capsfilter name=vcaps caps=video/x-raw,width=320,height=240,format=RGBA ! videoconvert ! videoscale ! appsink name=appsink"

###################################################################################################
# jpeg encoded
# on laptop
gst-launch-1.0 -v videotestsrc ! video/x-raw,format=I420,width=320,height=240,framerate=20/1 ! \
    jpegenc ! rtpjpegpay ! udpsink host="192.168.2.248" port=5000

# either of the following 2 work
# on SAMA7
gst-launch-1.0 -v udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 ! \
    rtpjpegdepay ! jpegdec ! videoscale ! videoconvert ! queue ! kmssink force-modesetting=true

# on SAMA7
egt_video --width 320 --height 240 --pipeline "udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 \
    ! rtpjpegdepay ! jpegdec ! capsfilter name=vcaps caps=video/x-raw,width=320,height=240,format=I420 ! videoscale ! \
    videoconvert ! appsink name=appsink"

###################################################################################################
## 320x240 format

# transmitter 
gst-launch-1.0 -v filesrc location=bbb.avi \
! decodebin ! videoconvert ! videoscale \
! video/x-raw,format=I420,width=320,height=240,framerate=24/1 \
! videorate ! video/x-raw,framerate=20/1 \
! jpegenc quality=80 ! rtpjpegpay pt=96 \
! udpsink host="192.168.2.248" port=5000


# receiver this will be full screen
gst-launch-1.0 -v udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 ! \
    rtpjpegdepay ! jpegdec ! videoscale ! videoconvert ! queue ! kmssink force-modesetting=true


egt_video --width 320 --height 240 --pipeline "udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 \
    ! rtpjpegdepay ! jpegdec ! capsfilter name=vcaps caps=video/x-raw,width=320,height=240,format=I420 ! videoscale ! 
    video/x-raw,width=640,height=480 !\
    videoconvert ! appsink name=appsink"

###########################################################################################
# 480x360 format
# transmitter 
gst-launch-1.0 -v filesrc location=bbb.avi \
! decodebin ! videoconvert ! videoscale \
! video/x-raw,format=I420,width=480,height=360,framerate=24/1 \
! videorate ! video/x-raw,framerate=20/1 \
! jpegenc quality=80 ! rtpjpegpay pt=96 \
! udpsink host="192.168.2.248" port=5000

# receiver this will be full screen
gst-launch-1.0 -v udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 ! \
    rtpjpegdepay ! jpegdec ! videoscale ! videoconvert ! queue ! kmssink force-modesetting=true

egt_video --width 480 --height 360 --pipeline "udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 \
    ! rtpjpegdepay ! jpegdec ! capsfilter name=vcaps caps=video/x-raw,width=480,height=360,format=I420 ! videoscale ! 
    videoconvert ! appsink name=appsink"

###########################################################################################
# webcam 320x240 format
# transmitter 
gst-launch-1.0 -v v4l2src device=/dev/video0 \
    ! videoconvert ! videoscale \
    ! video/x-raw,format=I420,width=320,height=240,framerate=20/1 \
    ! jpegenc quality=80 ! rtpjpegpay pt=96 \
    ! udpsink host="192.168.2.248" port=5000

# receiver this will be full screen
gst-launch-1.0 -v udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 ! \
    rtpjpegdepay ! jpegdec ! videoscale ! videoconvert ! queue ! kmssink force-modesetting=true

egt_video --width 320 --height 240 --pipeline "udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 \
    ! rtpjpegdepay ! jpegdec ! capsfilter name=vcaps caps=video/x-raw,width=320,height=240,format=I420 ! videoscale ! 
    videoconvert ! appsink name=appsink"