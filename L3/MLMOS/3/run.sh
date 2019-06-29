#! /usr/bin/env bash

mkdir -p /srv/gitlab/{config,logs,data}
mkdir -p /srv/gitlab-runner/config

docker stack deploy --compose-file docker-compose.yml tema3
docker exec -ti tema3_gitlab.1.$(docker service ps -f 'name=tema3_gitlab.1' tema3_gitlab -q --no-trunc | head -n1) gitlab-ctl reconfigure
