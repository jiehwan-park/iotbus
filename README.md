# iotbus
============
##Build the iotbus source code

$ git clone https://github.com/tizen-artik/iotbus.git

$ gbs build -A armv7l --include-all


##Install rpm files to target device

root:~> rpm -Uvh --nodeps --force /tmp/capi-iotbus-*.rpm

root:~> rpm -Uvh --nodeps --force /tmp/capi-iotbus-server-*.rpm

root:~> sync

root:~> reboot


