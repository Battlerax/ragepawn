/* 
RAGEPAWN
Contains all of the natives and required stuff to get running 
*/

#pragma tabsize 4

#include <core>
#include <float>
#include <string>
#include <file>
#include <time>
#include <datagram>

#define function%0(%1) forward%0(%1); public%0(%1)

// Console
native println(const string[], foreground = -1, background = -1, highlight = -1);
native printfln(const format[], { Float,Fixed,_ }:...);

// String manipulation
native format(output[], const format[], ...);

// Utils
native GetHashKey(const string[]);
native TriggerClientEvent(playerid, const name[], const format[], ...);

// Player Functions
native GetPlayerName(playerid, name[]);
native SendClientMessage(playerid, const message[]);
#define OutputChatBox(%0,%1) SendClientMessage(%0,%1)

// Callbacks
forward OnIncomingConnection(playerid, ip_address[]);
forward OnPlayerConnect(playerid);
forward OnUpdate();
forward OnPlayerDisconnect(playerid, reason, message[]);
forward OnPlayerSpawn(playerid);
forward OnPlayerDeath(playerid, killerid, reason);
forward OnVehicleDeath(vehicleid, killerid);
forward OnVehicleDeathEx(vehicleid, killerid, reason);
forward OnPlayerText(playerid, text[]);
forward OnPlayerCommandText(playerid, cmdtext[]);
forward OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
forward OnPlayerEnterVehicleEx(playerid, vehicleid, seatId);
forward OnPlayerExitVehicle(playerid, vehicleid);
forward OnPlayerDamage(playerid, Float:healthLoss, Float:armorLoss)
forward OnPlayerStartEnterVehicle(playerid, vehicleid, seatid);
forward OnPlayerStartExitVehicle(playerid,vehicleid);