#!/bin/sh

MD2REVIEW_DOCKER_IMG=nuitsjp/md2review:1.12.1
REVIEW_DOCKER_IMG=vvakame/review:5.1
WORK_DIR=work
ARTICLES_DIR=articles
MOUNT_DIR=`pwd`/./$WORK_DIR:/$WORK_DIR

docker run -d -p 80:80 docker/getting-started
docker pull $MD2REVIEW_DOCKER_IMG
docker pull $REVIEW_DOCKER_IMG
mkdir $WORK_DIR
cp $ARTICLES_DIR/*.md $WORK_DIR
docker run --rm -v $MOUNT_DIR $MD2REVIEW_DOCKER_IMG /bin/sh -c "cd $WORK_DIR  && md2reviews.sh"
mv $WORK_DIR/*.re $ARTICLES_DIR
/bin/sh -c ./build-in-docker.sh
rm $ARTICLES_DIR/*.re
mv $WORK_DIR/*.md $ARTICLES_DIR
rm -rf $WORK_DIR
