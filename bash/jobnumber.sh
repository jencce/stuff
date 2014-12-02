sed -ie '/J:/!d' /tmp/ls
awk -F[:\ ] '{print $2}' /tmp/ls | xsel -b
