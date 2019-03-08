#!/bin/bash
sed -n '8p;32p;128p;512p;1024p' $1 > $2
