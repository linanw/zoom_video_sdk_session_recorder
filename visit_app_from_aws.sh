#!/bin/bash

. set_env.sh

if [ -z "$AWS_ACCESS_KEY_ID" ] ; then echo "Please set your AWS_ACCESS_KEY_ID in set_env.sh. "; exit 1; fi
if [ -z "$AWS_SECRET_ACCESS_KEY" ] ; then echo "Please set your AWS_SECRET_ACCESS_KEY in set_env.sh. "; exit 1; fi
if [ -z "$AWS_DEFAULT_REGION" ] ; then echo "Please set your AWS_DEFAULT_REGION in set_env.sh. "; exit 1; fi

aws elbv2 describe-load-balancers | python3 -c "import sys, json, subprocess; LBs=json.load(sys.stdin)['LoadBalancers'];subprocess.run(['open', 'http://' + LBs[-1]['DNSName'] + ':81']) if len(LBs) > 0 else print('No LoadBalancer found.')" &
