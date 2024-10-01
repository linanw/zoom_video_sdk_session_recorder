```
sudo apt update

# Install cMake
sudo apt install -y build-essential gcc gdb cmake

# ZoomVideoSDK dependencies
sudo apt install -y libglib2.0-dev libxcb-image0 libxcb-keysyms1 libxcb-xtest0 libxcb-shape0 libgbm1

# Other dependencies (optional)
# sudo apt install -y liblzma-dev  libxcb-xfixes0 libxcb-xkb1  libxcb-shm0 libxcb-randr0  libgbm1 libxtst6 libgl1 libcurl4-openssl-dev

# Install opencv libs
sudo apt install -y libopencv-dev libavcodec-dev libavformat-dev

# Install gstreamer (optional)
# sudo apt install -y gstreamer1.0* libgstreamer1.0* gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav

git clone https://github.com/linanw/zoom_v-sdk_linux_bot.git
cd zoom_v-sdk_linux_bot/
git checkout zoom_video_sdk_send_raw_data

cp config.json.example config.json

cmake -B build
cd build/
make

# Update config.json with proper jwt token and session infos

cd ../bin
./video_sdk_send_raw_data
```

