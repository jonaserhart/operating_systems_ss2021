# Exercise Sheet 1 - Jonas Erhart

## Task 1
Consider the following scripts. Explain what each line means.

### Script 1

```bash
#!/bin/bash 
# the above line instructs the interpreter to use the program /bin/bash to
# run this script
for FN in "$@" # loops over all arguments as string ('$@' is a list of string)
do # do the following in the loop
	chmod 0750 "$FN" # execute chmod on arguments (should probably be filenames)
# '0750' is octal notation for:
#  - owner permissions:
#    - 7: 1 (read permission) + 2 (write persmission) + 4 (execute permission)
#  - group permissions:
#    - 5: 1 (read permission) + 4 (execute persmission)
#  - public permissions:
#    - 0: no permissions
done #loop is done
```
Explain what happens when you execute this script with and without input arguments.

The `for` - loop would not have any arguments to loops through.
The program will exit with success status and do nothing.

### Script 2

#### Infos on `shift`:

When used in a shell script the command `shift <n>` shifts the arguments given to the script `n` positions to the left, throwing out the arguments that are shifted beyond the start of the argument array.
When used with no arguments, the `shift` command assumes `n=1`.

```bash
#!/bin/bash
function usage {
    echo "Usage: $0 <TODO: fill me in>"
    exit 1
}
# the lines below each assign the first argument to the
# corresponding variables
# then the argument array is shifted to the left
# this means: $2 -> $1
# so in the end:
# ARG1 = $1
# ARG2 = $2
# ARG3 = $3
# if either of these shifts fail
# (due to not providing enough arguments for example)
# 'usage' is executed
ARG1="$1"; shift || usage 
ARG2="$1"; shift || usage
ARG3="$1"; shift || usage

# grep: 
#   matches 'ARG1' as regular expression on 'ARG3'
#   -n makes sure the output of grep contains
#   the line number where a match has been found
# the output of 'grep' is redirected to the file ARG2 
grep -n "$ARG3" "$ARG1" > "$ARG2"
```

Explain what happens when you execute this script with and without input arguments. Update the message printed in usage to be more helpful.

```bash
#!/bin/bash
function usage {
    echo "Usage: $0 <string to match> <outputfile> <file to search in>"
    exit 1
}

ARG1="$1"; shift || usage
ARG2="$1"; shift || usage
ARG3="$1"; shift || usage

grep -n "$ARG3" "$ARG1" > "$ARG2"
```

### Script 3

This scipt is probably inteded to create a directory and log the output on an error to the file /dev/null.

```bash
#!/bin/bash
# should be mkdir
makedir "$1" 2> /dev/null
```

Nothing would happen if this script was executed, because `makedir`is not a command recognized by the system (if you have not installed a command named `makedir`).

null - device logged keinen errors

## Task 2

### What does this shell script do?

```bash
#!/bin/bash
# assign arguments to variables
INFILE="$1"
OUTFILE="$2$"
# checks if $INFILE exists
if [ -e "$INFILE" ]
# if exists
then
	# writes contents of INFILE to OUTFILE, appending if present
	cat "$INFILE" >> "$OUTFILE"
fi
```
### Improve its usability by adding error handling for the following cases:

#### Print a help message when the number of provided arguments is not two.

```bash
#!/bin/bash
if [ ! "$#" -eq 2 ]
then
	echo "Please provide exactly two arguments"
	exit 1
fi

INFILE="$1"
OUTFILE="$2$"
if [ -e "$INFILE" ]
then
	cat "$INFILE" >> "$OUTFILE"
fi
```

#### Log an error message to `stderr` if the file `$OUTFILE` is not writable.

```bash
#!/bin/bash
if [ ! "$#" -eq 2 ]
then
	echo "Please provide exactly two arguments" >&2
	exit 1
fi

INFILE="$1"
OUTFILE="$2"
# !: not
# w: write permission are granted
if [ ! -w "$OUTFILE" ]
then
	echo "$OUTFILE can not be written: permission denied" 
	exit 1
fi

if [ -e "$INFILE" ]
then
	cat "$INFILE" >> "$OUTFILE"
fi
```

#### Think about what else might go wrong with this script. Find at least one other edge case and log an appropriate error message.

##### Edge case: `$INFILE` does not exist: no message is logged

```bash
#!/bin/bash
if [ ! "$#" -eq 2 ]
then
	echo "please provide exactly two arguments"
	exit 1
fi

INFILE="$1"
OUTFILE="$2"
# !: not
# w: write permission are granted
if [ ! -w "$OUTFILE" ]
then
	echo "$OUTFILE can not be written: permission denied" 
	exit 1
fi

# if not exists -> log error
if [! -e "$INFILE" ]
then
	echo "$INFILE does not exist"
	exit 1
fi

# execute if passed so far
cat "$INFILE" >> "$OUTFILE"
```

##### Edge case: `$INFILE` can not be read due to missing permissoins

```bash
#!/bin/bash
if [ ! "$#" -eq 2 ]
then
	echo "please provide exactly two arguments"
	exit 1
fi

INFILE="$1"
OUTFILE="$2"
# !: not
# w: write permission are granted
if [ ! -w "$OUTFILE" ]
then
	echo "$OUTFILE can not be written: permission denied" 
	exit 1
fi

# if not exists -> log error
if [ ! -e "$INFILE" ]
then
	echo "$INFILE does not exist"
	exit 1
fi

if [ ! -r "$INFILE" ]
then
	echo "$INFILE can not be read: permission denied"
	exit 1
fi
# execute if passed so far
cat "$INFILE" >> "$OUTFILE"
```

## Task 3

Write a script my_backup.sh to perform a backup of the current directory (containing only files) into a given folder. The script receives a single parameter which is the path of the folder where the backup is to be stored.

The script will be invoked as follows:

```bash
bash my_backup.sh /path/to/backup-folder # creates a backup of . into backup-folder
```
If the backup folder already exists, check for each file whether it has already been backed up. If so, back up a copy of the file by appending a timestamp to its name. For example, a file file1 could become file1-1614624210. You can use date +%s to get the current time as a UNIX timestamp.

```bash
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
```
