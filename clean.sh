#!/usr/bin/env bash
set -euxo pipefail

rm  ./example-camera/bin/data/yolov2-tiny.weights
rm  ./example-single_image/bin/data/yolov2-tiny.weights
rm  ./example-annotation/bin/data/yolov2-tiny.weights
rm  ./example-single_image/bin/data/yolov2-tiny.cfg
rm  ./example-camera/bin/data/yolov2-tiny.cfg
rm  ./example-annotation/bin/data/yolov2-tiny.cfg
rm -fR ./example-camera/bin/camera*.app
rm -fR ./example-single_image/bin/single_image*.app
rm -fR ./example-annotation/bin/annotation*.app
echo "done"