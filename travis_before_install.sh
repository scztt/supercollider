#!/usr/bin/env bash
if [[ "$OSTYPE" == "linux-gnu" ]]; then
	sudo apt-add-repository -y ppa:ubuntu-sdk-team/ppa
	sudo apt-get update
	#sudo apt-get install qt5-default qt5-qmake qttools5-dev qttools5-dev-tools qtdeclarative5-dev libqt5webkit5-dev qtchooser
	sudo apt-get install libqt4-assistant libqt4-core \
		libqt4-dbg libqt4-dbus libqt4-designer libqt4-dev \
		libqt4-gui libqt4-help libqt4-network libqt4-opengl \
		libqt4-opengl-dev libqt4-qt3support libqt4-script \
		libqt4-sql libqt4-sql-mysql libqt4-sql-odbc \
		libqt4-sql-psql libqt4-sql-sqlite \
		libqt4-svg libqt4-test libqt4-webkit libqt4-webkit-dbg \
		libqt4-xml libqt4-xmlpatterns libqt4-xmlpatterns-dbg \
		libqtcore4 libqtgui4 qt4-demos qt4-designer qt4-dev-tools \
		qt4-doc qt4-doc-html qt4-qtconfig qtcreator

elif [[ "$OSTYPE" == "darwin"* ]]; then
	brew update
 	brew install qt4

fi

