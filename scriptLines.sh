red='\033[0;31m'
resetColor='\033[0m'
green='\033[0;32m'
blue='\033[0;34m'
if test $# -ne 1 
then 
    echo "Usage: $0 <file>"
    exit 1
fi
lines=0
if test -f $1
then
  lines=$(wc -l < $1)
  echo -e " \n ${green}The file $1 has ${blue} $lines ${green} lines.${resetColor} \n"
  else
  echo -e "File $1 not found."
  exit 1
  fi