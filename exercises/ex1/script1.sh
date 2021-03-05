#!/bin/zsh
for FN in "$@"
do
    chmod 0750 "$FN"
done
