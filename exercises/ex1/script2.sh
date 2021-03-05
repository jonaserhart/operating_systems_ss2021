#!/bin/zsh
function usage {
    echo "Usage: $0 <string to match> <outputfile> <file to search in>"
    exit 1
}

ARG1="$1"; shift || usage
ARG2="$1"; shift || usage
ARG3="$1"; shift || usage

grep -n "$ARG3" "$ARG1" > "$ARG2"