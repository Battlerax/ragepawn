native Kick(playerid, reason[]='\0');
native Ban(playerid, reason[]='\0');
native SendClientMessage(playerid, message[]);
native Notify(playerid, message[]);
native GetPlayerName(playerid, name[], lenth);
native TriggerClientEvent(playerid, const name[], const format[], ...);
native SetPlayerName(playerid, name[]);
native IsPlayerAiming(playerid);
native GetPlayerHeading(playerid, &Float:heading);
native SetPlayerHeading(playerid, Float:heading);
native GetPlayerMoveSpeed(playerid, &Float:moveSpeed);
native GetPlayerHealth(playerid, &Float:health);
native SetPlayerHealth(playerid, Float:value);
native GetPlayerArmour(playerid, &Float:armour);
native SetPlayerArmour(playerid, Float:armour);
native GetPlayerAimPosition(playerid, Float:x, Float:y, Float:z);
native GetPlayerPing(playerid, &ping);
native GetPlayerPacketLoss(playerid, &packetLoss);
native GetPlayerKickReason(playerid, &reason[], length)
native Evaluate(playerid, code[]);


native SetPlayerName(playerid, name[]);
native GetPlayerIP(playerid, &ipAddress[], length);
native IsPlayerJumping(playerid);
native IsPlayerInCover(playerid);
native IsPlayerEnteringVehicle(playerid);
native IsPlayerLeavingVehicle(playerid);
native IsPlayerClimbing(playerid);
native IsPlayerOnLadder(playerid);
native IsPlayerReloading(playerid);
native IsPlayerInMelee(playerid);
native GetPlayerActionString(playerid, action[], length);








