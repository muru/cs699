#! /bin/sed -nf
/^[[:space:]]*<body>[[:space:]]*$/I,/^[[:space:]]*<\/BODY>[[:space:]]*$/I p
