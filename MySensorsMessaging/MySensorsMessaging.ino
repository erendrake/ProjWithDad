/***
 *  This is a simple sketch used to demenstrate and test node-to-node MySensor's communication.
 *  To use this sketch, assemble MySensors nodes - they need nothing more than a radio
 *  1.  Flash each node with the same sketch, open the console and type either 0 or 1 to the respective nodes to set thei ID
 *  2.  You only need to set the node id once, and restart the nodes
 *  3.  To being a ping-pong test, simply type T in the console for one of the nodes.
 *
 *  2015-05-25 Bruce Lacey v1.0
 */

// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <MySensors.h>
#include "MYSLog.h"

// Define two generic nodes with a single child
#define MY_NODE_ID 6
#define ME_HEATER 1

#define TARGET_DESTINATION 123456

MyMessage statusUpdate(ME_HEATER, V_STATUS);   //Status Update

uint32_t statusUpdateIntervalMS = 60000;
uint32_t nextStatusUpdateMS = 0;

void setup()
{
}

void presentation()
{
	present(ME_HEATER, S_HEATER);

	sendSketchInfo("Shop Heater", "v0.1");

	LOG(F("\nShop Heater Ready.\n"));
}

void loop()
{
	uint32_t currentMillis = (uint32_t)millis();
	
	if(currentMillis >= nextStatusUpdateMS){
		nextStatusUpdateMS = currentMillis + statusUpdateIntervalMS;
		sendStatusUpdate(TARGET_DESTINATION);
	}
}

void receive(const MyMessage &message)
{
	//TODO: Set the status from the message
}

void sendStatusUpdate(uint8_t target)
{
	LOG(F("Sending Status Update"));

	//TODO: Get the status to send
	statusUpdate.set( (uint32_t)millis() );

	statusUpdate.setDestination(target);
	send(statusUpdate);
}

void setNodeId(byte nodeID)
{
	LOG(F("Setting node id to: %i.\n***Please restart the node for changes to take effect.\n"), nodeID);
	hwWriteConfig(EEPROM_NODE_ID_ADDRESS, (byte)nodeID);
}
