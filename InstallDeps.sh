#!/bin/bash
 set -e 
opencv_deps="
build-essential cmake libgtkglext1-dev libvtk6-dev zlib1g-dev libjpeg-dev libwebp-dev libpng-dev libtiff5-dev libjasper-dev libopenexr-dev libgdal-dev libgphoto2-dev libdc1394-22-dev libavcodec-dev libavformat-dev libswscale-dev libtheora-dev libvorbis-dev libxvidcore-dev libx264-dev yasm libopencore-amrnb-dev libopencore-amrwb-dev libv4l-dev libxine2-dev v4l-utils libtbb-dev libeigen3-dev python-dev python-tk python-numpy python3-dev python3-tk python3-numpy"

install_festival="
festival festival-dev"

pocketsphinx_deps="
automake autoconf libtool bison swig python-dev libpulse-dev"

#set(BUILD_SHARED_LIBS true)
export dlib_version="19.15"
export guile_version="2.2"
export raspicam_version="0.1.6"
export opencv_version="3.2.0"
export opencv_contrib_version="3.2.0"

# Opencv

echo "Installing OpenCV"
sudo apt-get install -y $opencv_deps
wget https://github.com/opencv/opencv/archive/$opencv_version.zip -O opencv_$opencv_version.zip
unzip opencv_$opencv_version.zip
rm opencv_$opencv_version.zip
mv opencv-$opencv_version OpenCV
cd OpenCV


#OpenCV_contrib library
wget https://github.com/opencv/opencv_contrib/archive/$opencv_contrib_version.zip -O opencv_contrib_$opencv_contrib_version.zip
unzip opencv_contrib_$opencv_contrib_version.zip
rm opencv_contrib_$opencv_contrib_version.zip
mv opencv_contrib-$opencv_contrib_version OpenCV_contrib

mkdir build
cd build
cmake -DOPENCV_EXTRA_MODULES_PATH=../OpenCV_contrib/modules -DWITH_QT=OFF -DWITH_OPENGL=ON -DFORCE_VTK=ON -DWITH_TBB=ON -DINSTALL_C_EXAMPLES=OFF -DWITH_GDAL=ON -DWITH_XINE=ON -DBUILD_EXAMPLES=OFF -DENABLE_PRECOMPILED_HEADERS=OFF ..
make -j $(($(nproc) + 1))
sudo make install
cd ../..
sudo rm -r OpenCV 


#dlib
echo "Installing dlib version $dlib_version"
 wget https://github.com/davisking/dlib/archive/v$dlib_version.tar.gz
 tar -xf  v$dlib_version.tar.gz
 rm v$dlib_version.tar.gz
 cd dlib-$dlib_version
 
 mkdir build
 cd build
 cmake -DBUILD_SHARED_LIBS=ON ..
 sudo make install
 cd ../..
 sudo rm -r dlib-$dlib_version


#guile
echo "Installing guile version $guile_version"
sudo apt-get install guile-$guile_version

#festival
echo "Installing festival"

sudo apt-get install -y $install_festival

#pocketSphinx
echo "Installing pocketsphinx"
sudo apt-get install -y $install_pocketsphinx_deps

mkdir sphinx
cd sphinx/

wget https://github.com/cmusphinx/sphinxbase/archive/master.zip -O sphinxbase.zip
unzip sphinxbase.zip
cd sphinxbase-master
./autogen.sh
make
sudo make install
sudo sed -i "$ a /usr/local/lib" /etc/ld.so.conf
sudo ldconfig
cd ..
sudo rm sphinxbase.zip

wget https://github.com/cmusphinx/pocketsphinx/archive/master.zip -O pocketsphinx.zip
unzip pocketsphinx.zip
cd pocketsphinx-master
./autogen.sh
make
sudo make install
cd ../..
sudo rm -r sphinx

#raspicam
echo "Installing raspicam version $raspicam_version"
wget https://sourceforge.net/projects/raspicam/files/raspicam-$raspicam_version.zip/download -O raspicam.zip
unzip raspicam.zip
rm raspicam.zip
cd raspicam-$raspicam_version
mkdir build
cd build
cmake ..
make
sudo make install
cd ../..
sudo rm -r raspicam-$raspicam_version

#wiringpi
echo "Installing wiringpi"
wget https://github.com/WiringPi/WiringPi/archive/master.zip -O wiringpi.zip
unzip wiringpi.zip
rm wiringpi.zip
cd WiringPi-master
./build
cd ..
sudo rm -r WiringPi-master






