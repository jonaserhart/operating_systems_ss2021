#!/bin/bash
# 
# my_backup.sh
#  USAGE: sh my_backup.sh <backup_directory>
#  this script backs up the current directory into 'backup_directory'
#  if a backup of a file already exists a timestamp is added to the backup file
#  this script also preserves file extensions
# 

# print usage and exit
function usage_and_exit {
    echo "USAGE: $0 <backup_directory>"
    exit 1
}

if [ ! "$#" -eq 1 ]
then
    echo "Please provide exacly one argument"
    usage_and_exit
fi

BACKUPDIR="$1"

# check if backup directory exists
if [ ! -d "$BACKUPDIR" ]
then
    echo "'$BACKUPDIR' does not exist"
    usage_and_exit
fi

# check if writing to backup directory is permitted
if [ ! -w "$BACKUPDIR" ]
then
    echo "no permissions to write to '$BACKUPDIR'"
    usage_and_exit
fi

# loop through files in current directory
for FILE in *
do
    # check if $FILE is a file
    if [ -f "$FILE" ]
    then
        # check if reading $FILE is permitted
        if [ ! -r "$FILE" ]
        then
            echo "skipping file '$FILE': no permission to read contents"
        fi
        BACKUPFILE="$BACKUPDIR/$FILE"
        # check if backup file already exists
        if [ -e "$BACKUPFILE" ]
        then
            echo "'$BACKUPFILE' already exists, adding timestamp..."
            # check if filename contains extension
            if [ $BACKUPFILE == *"."* ]
            then
                # preseve extension of file when adding timestamp
                # cut: cut string on delimiter 'd' and take the value at position 'f'
                FILENAME=$(echo $FILE | cut -f 1 -d '.')
                EXTENSION=$(echo $FILE | cut -f 2 -d '.')
                BACKUPFILE="$BACKUPDIR/$FILENAME-$(date +%s).$EXTENSION"
            else
                BACKUPFILE="$BACKUPFILE-$(date +%s)"
            fi
        fi
        # copy contents of $FILE to $BACKUPFILE
        cat $FILE > $BACKUPFILE 
        echo "backed up file '$FILE' to $BACKUPFILE"
    fi
done
