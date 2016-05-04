#!/bin/bash
#
# Details on generating cert and key using openssl
#
openssl req -x509 -newkey rsa:1024 -keyout key1024.pem -out cert1024.pem
