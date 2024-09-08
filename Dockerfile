# FROM debian
FROM dorowu/ubuntu-desktop-lxde-vnc:focal-lxqt

RUN wget -q -O - https://dl.google.com/linux/linux_signing_key.pub | apt-key add -
RUN curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | apt-key add -
RUN echo "deb https://dl.yarnpkg.com/debian/ stable main" | tee /etc/apt/sources.list.d/yarn.list
RUN apt update 
RUN apt install -y git gcc build-essential wget  
RUN apt install -y libxcb-xtest0 libxcb-keysyms1 libxcb-image0 qtbase5-dev \
    libqt53dquick5 libqt5x11extras5 libssl-dev liblzma-dev libglib2.0-dev
RUN apt install -y nodejs npm yarn 
RUN npm i -g n 
RUN n stable 
RUN hash -r 
RUN npm install -g npm@latest


WORKDIR /
RUN wget https://github.com/Kitware/CMake/releases/download/v3.24.1/cmake-3.24.1.tar.gz
RUN tar -xzf cmake-3.24.1.tar.gz
WORKDIR /cmake-3.24.1
RUN ./bootstrap
RUN make
RUN make install

# ARG DEBIAN_FRONTEND=noninteractive
# RUN apt install -y xorg

WORKDIR /
RUN mkdir -p /zoom_v-sdk_linux_bot
WORKDIR /zoom_v-sdk_linux_bot
COPY src/ src/
COPY lib/ lib/
COPY include/ include/
COPY config.json .
COPY CMakeLists.txt .
RUN cmake -B build/
RUN cmake --build build --config RelWithDebInfo --target all -j 22

RUN mkdir -p /root/.zoom/logs/

# adding the api and management web
COPY api /zoom_v-sdk_linux_bot/api
WORKDIR /zoom_v-sdk_linux_bot/api
RUN yarn
EXPOSE 81

ENTRYPOINT [ "node", "api.js"]
# RUN apt install -y libasound2
