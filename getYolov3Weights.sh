#!/usr/bin/env bash
set -euxo pipefail

CFG_FILE="yolov3-tiny.cfg"
WEIGHTS_FILE="yolov3-tiny.weights"
# CFG_FILE="yolov3-tiny.cfg"
# WEIGHTS_FILE="yolov3-tiny.weights"
# CFG_FILE="yolov3-tiny.cfg"
# WEIGHTS_FILE="yolov3-tiny.weights"
PATH_TO_CFG="https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/${CFG_FILE}"
PATH_TO_WEIGHTS="https://pjreddie.com/media/files/${WEIGHTS_FILE}"


	echo "downloading weights and cfg"
if hash wget 2>/dev/null; then
	wget "${PATH_TO_WEIGHTS}"
	wget "${PATH_TO_CFG}"
else
	echo "wget is not installed. Falling back to curl"
	curl -O "${PATH_TO_WEIGHTS}"
	curl -O "${PATH_TO_CFG}"
fi


cp "./${WEIGHTS_FILE}" "./example-camera/bin/data/${WEIGHTS_FILE}"
cp "./${WEIGHTS_FILE}" "./example-single_image/bin/data"
cp "./${WEIGHTS_FILE}" "./example-annotation/bin/data"
cp "./${CFG_FILE}" "./example-single_image/bin/data/"
cp "./${CFG_FILE}" "./example-camera/bin/data/"
cp "./${CFG_FILE}" "./example-annotation/bin/data/"
rm -f "./${CFG_FILE}"
rm -f "./${WEIGHTS_FILE}"
echo "done"
