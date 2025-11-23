# atdx:auto-jtdx(we need auto TX!!)
### how to build
  sudo apt install -y \
            build-essential cmake git pkg-config \
            qtbase5-dev qttools5-dev qttools5-dev-tools qtmultimedia5-dev \
            libqt5serialport5-dev libqt5svg5-dev \
            gfortran libgfortran-14-dev \
            libfftw3-dev libusb-1.0-0-dev libhamlib-dev \
            portaudio19-dev libsamplerate0-dev libasound2-dev \
            libudev-dev libcurl4-openssl-dev \
            libboost-all-dev \
            asciidoctor
  mkdir build
  cd build
  cmake ..
  make
  cpack

After that, you will see **.deb then use `sudo apt install **.deb` to install
