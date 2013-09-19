#! /bin/bash

sed '0,/^[[:space:]]*<body>[[:space:]]*$/Id;/^[[:space:]]*<\/BODY>[[:space:]]*$/I,$d' $1 | wc -w
