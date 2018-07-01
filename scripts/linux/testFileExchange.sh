#!/bin/bash

outputWanted=`cat -e tests/outputFileExchange.txt`

cd ./build

echo "Generating random test file"
head -c 50M < /dev/urandom > test.txt

echo "Expecting result:"
echo "$outputWanted"

##Launching server in background
./bin/tcpFileServer &> outputServer &
serverPID=$!

echo "Launched server on $serverPID"

echo "Waiting 10secs";
sleep 10;

./bin/tcpFileClient &> outputClient &
clientPID=$!

echo "Launched client on $clientPID"

echo "Waiting 10secs";
sleep 10;

kill -s "SIGTERM" $clientPID;
sleep 1;
kill -s "SIGTERM" $serverPID;

echo "Client and Server terminated, comparing responses"

response=$(cat -e outputServer | grep "SUCCESS");

echo "Got response:"
echo "$response"

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

