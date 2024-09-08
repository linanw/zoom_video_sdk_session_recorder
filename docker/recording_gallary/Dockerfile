FROM node

RUN apt update ; apt install -y exiftool graphicsmagick ffmpeg gifsicle dcraw zip
RUN npm install -g thumbsup

COPY server /server
RUN mkdir -p /server/recording
RUN mkdir -p /server/static/gallary

WORKDIR /server/
ENTRYPOINT [ "node", "api/api.js" ]
#CMD [ "node", "api/api.js" ]