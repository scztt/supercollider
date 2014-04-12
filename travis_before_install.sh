#!/usr/bin/env bash
if [[ "$OSTYPE" == "linux-gnu" ]]; then
	sudo apt-add-repository -y ppa:ubuntu-sdk-team/ppa
	sudo apt-get update
	#sudo apt-get install qt5-default qt5-qmake qttools5-dev qttools5-dev-tools qtdeclarative5-dev libqt5webkit5-dev qtchooser
	sudo apt-get install build-essential libqt4-dev libqtwebkit-dev libjack-dev libsndfile1-dev libasound2-dev libavahi-client-dev libicu-dev libreadline6-dev libfftw3-dev libxt-dev pkg-config cmake subversion

elif [[ "$OSTYPE" == "darwin"* ]]; then
	brew update
 	brew install qt4

fi

