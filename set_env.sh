#!/bin/bash

# AWS Settings
export AWS_ACCESS_KEY_ID=
export AWS_SECRET_ACCESS_KEY=
export AWS_DEFAULT_REGION=ap-southeast-1

# Settings
export VSDK_DEMO_DOCKER_COMPOSE_FILE=./docker-compose.yml
export DEB_URL=https://linanw.me/demo.deb
export EXEC_PATH=/opt/zoom-video-sdk/electron-video-sdk
export REBUILD_IMAGE=false
export LAUNCH_IN_BROWSER=true
export KEEP_CONTAINER_ON_APP_EXIT=false
export RESOLUTION=1280x720