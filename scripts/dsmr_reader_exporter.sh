#!/bin/bash

AUTH_KEY="86KYQBF0XWRBJAVRVZ7EFLDJ7PGYPC2EZUJ1MQ1O7AIDC83C2E20Y481EC9W3BUR"

i=0
while read line; do
  TELEGRAM="$(echo $line | xxd -r -p)"
  curl -H "X-AUTHKEY: $AUTH_KEY" -d "telegram=$TELEGRAM" http://127.0.0.1:8000/api/v1/datalogger/dsmrreading
  echo -n .
  ((i++))
  if ((i % 100 == 0)); then
    echo $i
  fi
done < <(psql prikmeter thomas -c 'select telegram from telegrams;' -A -F, -t | cut -c3-)
