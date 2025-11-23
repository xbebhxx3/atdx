# atdx:auto-jtdx (we need auto TX!!!)
### how to Compile
    sudo apt update
    sudo apt install -y \
        build-essential cmake git pkg-config \
        qtbase5-dev qttools5-dev qttools5-dev-tools qtmultimedia5-dev \
        libqt5serialport5-dev libqt5svg5-dev \
        gfortran libgfortran-14-dev \
        libfftw3-dev libusb-1.0-0-dev libhamlib-dev \
        portaudio19-dev libsamplerate0-dev libasound2-dev \
        libudev-dev libcurl4-openssl-dev \
        libboost-all-dev

    mkdir build
    cd build
    cmake ..
    make clean
    make
    ./atdx

Copyright (C) 2001-2020 by Joe Taylor, K1JT.
Copyright (C) 2016-2020 by Igor Chernikov UA3DJY and Arvo Järve ES1JA.

atdx is forked from  WSJT-X, a computer  program  dedicated to amateur
radio communication using very weak signals. 

The program runs equally well on Windows, Macintosh and Linux systems,
and installation packages are available for all three platforms.

atdx  is  an  open-source project released under the  GPL license (See
COPYING). If  you have  programming or  documentation skills  or would
like to  contribute to  the project  in other  ways, please  make your
interests known  to the  development team.


Project Web site:

http://www.atdx.tech

Project mailing  list:

https://atdx.groups.io/g/main
