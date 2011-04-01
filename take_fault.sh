#!/bin/bash

echo "0 0 0 0"
./arafed power 0 0 0 0
./arafed monitor 5
echo "1 0 0 0"
./arafed power 1 0 0 0
./arafed monitor 5
echo "0 1 0 0"
./arafed power 0 1 0 0
./arafed monitor 5
echo "1 1 0 0"
./arafed power 1 1 0 0
./arafed monitor 5
echo "0 0 1 0"
./arafed power 0 0 1 0
./arafed monitor 5
echo "1 0 1 0"
./arafed power 1 0 1 0
./arafed monitor 5
echo "0 1 1 0"
./arafed power 0 1 1 0
./arafed monitor 5
echo "1 1 1 0"
./arafed power 1 1 1 0
./arafed monitor 5
echo "0 0 0 1"
./arafed power 0 0 0 1
./arafed monitor 5
echo "1 0 0 1"
./arafed power 1 0 0 1
./arafed monitor 5
echo "0 1 0 1"
./arafed power 0 1 0 1
./arafed monitor 5
echo "1 1 0 1"
./arafed power 1 1 0 1
./arafed monitor 5
echo "0 0 1 1"
./arafed power 0 0 1 1
./arafed monitor 5
echo "1 0 1 1"
./arafed power 1 0 1 1
./arafed monitor 5
echo "0 1 1 1"
./arafed power 0 1 1 1
./arafed monitor 5
echo "1 1 1 1"
./arafed power 1 1 1 1
./arafed monitor 5

