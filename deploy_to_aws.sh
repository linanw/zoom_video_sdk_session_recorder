#!/bin/bash

. set_env.sh

if [ -z "$AWS_ACCESS_KEY_ID" ] ; then echo "Please set your AWS_ACCESS_KEY_ID in set_env.sh. "; exit 1; fi
if [ -z "$AWS_SECRET_ACCESS_KEY" ] ; then echo "Please set your AWS_SECRET_ACCESS_KEY in set_env.sh. "; exit 1; fi
if [ -z "$AWS_DEFAULT_REGION" ] ; then echo "Please set your AWS_DEFAULT_REGION in set_env.sh. "; exit 1; fi

if [[ "$(docker context list --format json | python3 -c "import sys, json; print(len([x for x in json.load(sys.stdin) if x['Name'] == 'vsdkdemo']))")" == 0 ]]; then \
docker context create ecs vsdkdemo --from-env; fi && \
docker -c vsdkdemo compose -f $VSDK_DEMO_DOCKER_COMPOSE_FILE up && \
if [ "$LAUNCH_IN_BROWSER" = true ] ; then \
sleep 10; \
. visit_app_from_aws.sh; fi
