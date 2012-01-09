/*
FastCraft - Minecraft SMP Server
Copyright (C) 2011 - 2012 Michael Albrecht aka Sprenger120

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#define BUILD_VERSION ""
#include <iostream>
#include <sstream>
#include <Poco/Thread.h>
#include <Poco/Exception.h>
#include "AcceptThread.h"
#include "SettingsHandler.h"
#include "PlayerPool.h"
#include "PackingThread.h"
#include "ServerTime.h"
#include "ItemInfoStorage.h"

using std::cout;
using std::stringstream;
using Poco::Thread;

int main() {
	cout<<"FastCraft Minecraft Server"<<"\n";
	cout<<"Version "<<BUILD_VERSION<<"-"<<SettingsHandler::getFastCraftVersion()<<" for Minecraft "<<SettingsHandler::getSupportedMCVersion()<<std::endl;
	
	try {
		SettingsHandler(); //Load configuration into static variables
	} catch (Poco::RuntimeException) {
		Thread::sleep(3000);
		return 1;
	}
	cout<<"Running on *:"<<SettingsHandler::getPort()<<" with "<<SettingsHandler::getPlayerSlotCount()<<" player slots"<<std::endl;


	//Threads
	Thread threadPackingThread("Chunk Packer");
	Thread threadPlayerPool("PlayerPool");
	Thread threadAcceptThread("Network Acceptor");

	ItemInfoStorage::addBasicIDSet();


	//Create ChunkPacker Thread
	PackingThread Packer;
	threadPackingThread.start(Packer);


	//Create player pool thread
	PlayerPool PPool(Packer);
	threadPlayerPool.start(PPool);


	//Acceptor thread
	AcceptThread Acceptor(PPool);
	threadAcceptThread.start(Acceptor);


	cout<<"Ready."<<"\n"<<std::endl;

	while (1) {		
		Thread::sleep(1000);
		ServerTime::tick();
	}

	return 1;
}