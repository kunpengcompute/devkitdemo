#!/bin/bash
# pip3 install defusedxml
# pip3 install pycryptodome
set -e
function show_help()
{
    echo "usage: bash sign.sh /path/to/pack-App /path/to/application_path /path/to/private_key.pem /path/to/ta_cert.der"
}

if [ "$1" == "-h" ]; then
    show_help
    exit 0
fi
current_dir=`pwd`
pack_util_path=${1%*/}
app_path=$2
key_path=$3
cert_path=$4
if [ -z "$pack_util_path" ] || [ ! -d "$pack_util_path" ]; then
    echo "pack-App path is empty or not exist!"
    show_help
    exit 1
fi
if [ -z "$app_path" ] || [ ! -d "$app_path" ]; then
    echo "application path is empty or not exist!"
    show_help
    exit 1
fi
if [ -z "$key_path" ] || [ ! -f "$key_path" ]; then
    echo "private key file is empty or not exist!"
    show_help
    exit 1
fi
if [ -z "$cert_path" ] || [ ! -f "$cert_path" ]; then
    echo "certificate file is empty or not exist!"
    show_help
    exit 1
fi
cp -f $key_path $pack_util_path/pack_tools/private_key.pem
cp -f $cert_path $pack_util_path/pack_tools/ta_cert.der
cp -rf $app_path $pack_util_path/
cd $pack_util_path
app=$(basename $app_path)
python3 pack.py $app
cd $current_dir
rm -rf $pack_util_path/$app
rm -rf $pack_util_path/$app.tar
mv $pack_util_path/$app.sec ./
