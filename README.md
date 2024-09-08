```
sudo apt update
sudo apt install -y build-essential gcc cmake
sudo apt install -y libglib2.0-dev liblzma-dev libxcb-image0 libxcb-keysyms1 libxcb-xfixes0 libxcb-xkb1 libxcb-shape0 libxcb-shm0 libxcb-randr0 libxcb-xtest0 libgbm1 libxtst6 libgl1 libcurl4-openssl-dev
git clone https://github.com/linanw/zoom_v-sdk_linux_bot.git
cd zoom_v-sdk_linux_bot/
cmake -B build
cd build/
make
```

# todo
1. stream to rtmp (currently not working, handled in onchatmessagereceived)
call localhost:8888


# Notes
1. libvideosdk.so shall rename to its soname: libvideosdk.so.1 to be found by executable at runtime.