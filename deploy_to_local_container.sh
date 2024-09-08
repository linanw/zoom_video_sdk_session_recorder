#!/bin/bash

. set_env.sh

if [ "$LAUNCH_IN_BROWSER" = true ] ; then \
sleep 45; \
. visit_app_from_local_container.sh; fi & \
if [ "$REBUILD_IMAGE" = false ] ; then \
docker -c desktop-linux compose -f $VSDK_DEMO_DOCKER_COMPOSE_FILE up --no-build; else \
docker -c desktop-linux compose -f $VSDK_DEMO_DOCKER_COMPOSE_FILE up --build; fi
