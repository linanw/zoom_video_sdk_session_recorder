#!/bin/bash

. set_env.sh

docker compose -f $VSDK_DEMO_DOCKER_COMPOSE_FILE down
