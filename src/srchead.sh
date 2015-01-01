#!/bin/bash

#
# Source Header Auto Changer Script
# Copyright (C) 2014-2015 Shinichiro Nakamura
# Version 0.1.0
#

DIRNAME="$1"

if [ ! "$#" = "1" ]
then
  echo "srchead.sh [target directory]"
  exit
fi

TARGETS=`find ./$DIRNAME -regex '.*\.[ch]$' -print`
INSFILE="srchead.txt"
TXT_START="/\*\*"
TXT_END=" \*/"

for F in $TARGETS
do
  TMPFILE=`mktemp srchead.XXXX`
  echo '---------------------------------------------------------------------'
  echo $F
  echo '---------------------------------------------------------------------'
  START=`grep -n $TXT_START $F | sed -e "s/[:].*//"`
  END=`grep -n $TXT_END $F | sed -e "s/[:].*//"`
  LINES=`cat $F | wc -l`
  START=`echo $START | awk '{ print $1 }'`
  END=`echo $END | awk '{ print $1 }'`
  echo "START = " $START
  echo "END   = " $END
  echo "LINES = " $LINES
  if [ "$START" != "" ]
  then
    BN=`basename $F`
    A=`expr $START - 1`
    B=`expr $END + 1`
    cat $F | sed -n -i "1,$A p" > $TMPFILE
    cat $INSFILE | sed -e "s/###FILE_NAME###/$BN/g" >> $TMPFILE
    cat $F | sed -n "$B,$LINES p" >> $TMPFILE
    mv $TMPFILE $F
  else
    BN=`basename $F`
    cat $INSFILE | sed -e "s/###FILE_NAME###/$BN/g" > $TMPFILE
    cat $F >> $TMPFILE
    mv $TMPFILE $F
  fi
done

