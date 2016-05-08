#!/bin/bash
#
# Details on generating cert and key using openssl
#
# Attempt #1
#openssl req -x509 -newkey rsa:1024 -keyout key1024.pem -out cert1024.pem

# Attempt #2
#openssl genrsa -des3 -passout pass:x -out server.pass.key 2048
#openssl rsa -passin pass:x -in server.pass.key -out server.key
#openssl req -new -key server.key -out server.csr
#openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt

# Attempt #3 - see https://github.com/codequest-eu/grpc-demo/blob/master/keys/generate.sh
# Generate CA key:
openssl genrsa -des3 -out ca.key 4096

# Generate CA certificate:
openssl req -new -x509 -days 365 -key ca.key -out ca.crt

# Generate server key:
openssl genrsa -des3 -out server.key 4096

# Generate server signing request:
openssl req -new -key server.key -out server.csr

# Self-sign server certificate:
openssl x509 -req -days 365 -in server.csr -CA ca.crt -CAkey ca.key -set_serial 01 -out server.crt

# Remove passphrase from the server key:
openssl rsa -in server.key -out server.key

# Generate client key:
openssl genrsa -des3 -out client.key 4096

# Generate client signing request:
openssl req -new -key client.key -out client.csr

# Self-sign client certificate:
openssl x509 -req -days 365 -in client.csr -CA ca.crt -CAkey ca.key -set_serial 01 -out client.crt

# Remove passphrase from the client key:
openssl rsa -in client.key -out client.key
