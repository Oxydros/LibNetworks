#!/bin/bash

outputWanted=`cat -e tests/outputTCP.txt`

cd ./build

##Launching server in background
./bin/tcpServer &> outputServer &
serverPID=$!

echo "Launched server on $serverPID"

echo "Waiting 5secs";
sleep 5;

./bin/tcpClient &> outputClient &
clientPID=$!

echo "Launched client on $clientPID"

echo "Waiting 5secs";
sleep 5;

kill -s "SIGTERM" $clientPID;
sleep 1;
kill -s "SIGTERM" $serverPID;

echo "Client and Server terminated, comparing responses"

response=$(cat -e outputServer | grep "logged");

if [ "$response" == "$outputWanted" ]
then
    rm outputServer;
    rm outputClient;
    echo "Success!";
    exit 0;
fi

echo "Failed!";

echo "=========================";
echo "Output SERVER";
echo "=========================";
cat outputServer;
echo "=========================";
echo "Output CLIENT";
echo "=========================";
cat outputClient;
echo "=========================";

rm outputServer;
rm outputClient;

exit 42;

