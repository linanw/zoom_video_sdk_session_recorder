```
sudo apt update
sudo apt install -y build-essential gcc cmake
sudo apt install -y libglib2.0-dev liblzma-dev libxcb-image0 libxcb-keysyms1 libxcb-xfixes0 libxcb-xkb1 libxcb-shape0 libxcb-shm0 libxcb-randr0 libxcb-xtest0 libgbm1 libxtst6 libgl1 libcurl4-openssl-dev

# install opencv libs
sudo apt install -y libopencv-dev

git clone https://github.com/linanw/zoom_v-sdk_linux_bot.git
cd zoom_v-sdk_linux_bot/
git checkout zoom_video_sdk_send_raw_data
cmake -B build
cd build/
make
```

