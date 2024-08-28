This app will join a zoom video sdk session and record each user's video to a separate video file. 
The code demostrate how to use Zoom Video SDK's Raw Data feature, and how to use FFMPEG lib to encode the Raw Data to a video file. 

## Download & Build

```
sudo apt update
sudo apt install -y build-essential gcc cmake
sudo apt install -y libglib2.0-dev liblzma-dev libxcb-image0 libxcb-keysyms1 libxcb-xfixes0 libxcb-xkb1 libxcb-shape0 libxcb-shm0 libxcb-randr0 libxcb-xtest0 libgbm1 libxtst6 libgl1 libnss3  libasound2 libpulse0
mkdir -p ~/.zoom/logs
git clone https://github.com/linanw/zoom_v-sdk_linux_bot.git
cd zoom_v-sdk_linux_bot/
cmake -B build
cd build/
make
cd ../bin/
./zoom_v-sdk_linux_bot
```

## Generate You JWT
You much have an account [subscribed](https://marketplace.zoom.us/docs/sdk/video/developer-accounts/) Zoom Video SDK. 

Find your Video SDK [key and secret](https://marketplace.zoom.us/docs/sdk/video/auth/#get-video-sdk-key-and-secret). 

Generate your JWT with [this script](https://gist.github.com/linanw/9144d9bfc84da4076ab5beb04d4f9db1).

## Run 
Add your JWT to bin/config.json.

Run the app from bin folder:
```
./zoom_v-sdk_linux_bot
```
