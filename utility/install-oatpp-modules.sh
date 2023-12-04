#!/bin/sh

rm -rf tmp

mkdir tmp
cd tmp

##########################################################
## install oatpp

OATPP_MODULE_NAME="oatpp"
OATPP_MODULE_VERSION="1.3.0" # specify the version for oatpp

git clone https://github.com/oatpp/$OATPP_MODULE_NAME
cd $OATPP_MODULE_NAME
git checkout tags/$OATPP_MODULE_VERSION -b $OATPP_MODULE_VERSION

mkdir build
cd build

cmake ..
make install

cd ../../

##########################################################
## install oatpp-swagger

OATPP_SWAGGER_MODULE_NAME="oatpp-swagger"
OATPP_SWAGGER_MODULE_VERSION="1.3.0" # specify the version for oatpp-swagger

git clone https://github.com/oatpp/$OATPP_SWAGGER_MODULE_NAME
cd $OATPP_SWAGGER_MODULE_NAME
git checkout tags/$OATPP_SWAGGER_MODULE_VERSION -b $OATPP_SWAGGER_MODULE_VERSION

mkdir build
cd build

cmake ..
make install

cd ../../

##########################################################

cd ../

rm -rf tmp
