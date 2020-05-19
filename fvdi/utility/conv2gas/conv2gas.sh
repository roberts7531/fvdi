#!/bin/sh

for file in $@; do
case "$file" in
  *.s) out=`dirname $file`/`basename $file .s`.gnu.s ;;
  *) out="$file.gnu" ;;
esac
tmp1=`basename $file`.tm1
tmp2=`basename $file`.tm2

trap "rm -f $tmp1 $tmp2" 0 1 2 3 15

# Comments with * in the first position on the line
sed "s%^\*.*%%" $file >$tmp2
mv $tmp2 $tmp1

# Comments after ; separator
sed "s%;.*$%%" $tmp1 >$tmp2
mv $tmp2 $tmp1

# Conditional compilation
sed "s%^\\([ 	][ 	]*\\)ifnd%\1.ifndef%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%^\\([ 	][ 	]*\\)ifd%\1.ifdef%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%^\\([ 	][ 	]*\\)ifne%\1.ifne%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%^\\([ 	][ 	]*\\)ifeq%\1.ifeq%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%^\\([ 	][ 	]*\\)ifge%\1.ifge%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%^\\([ 	][ 	]*\\)else%\1.else%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%^\\([ 	][ 	]*\\)endc%\1.endif%" $tmp1 >$tmp2
mv $tmp2 $tmp1

# Constants
sed "s%^\\([^ 	]*\\)[ 	][ 	]*equ[ 	][ 	]*%	.equ \\1,%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%^\\([^ 	]*\\)[ 	][ 	]*set[ 	][ 	]*%	.set	\\1,%" $tmp1 >$tmp2
mv $tmp2 $tmp1

# Sections
sed "s%^\\([ 	][ 	]*\\)text%\1.text%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%^\\([ 	][ 	]*\\)data%\1.data%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%^\\([ 	][ 	]*\\)bss%\1.bss%" $tmp1 >$tmp2
mv $tmp2 $tmp1

# Hexadecimal numbers
sed "s%\\$%0x%" $tmp1 >$tmp2
mv $tmp2 $tmp1

sed "s%^.loop1:$%1:%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%^.loop2:$%2:%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%^.loop2_end:$%3:%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%,.loop1$%,1b%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%,.loop2$%,2b%" $tmp1 >$tmp2
mv $tmp2 $tmp1
sed "s%.loop2_end$%3f%" $tmp1 >$tmp2
mv $tmp2 $tmp1

# Things that would be hard do deal with using sed
`dirname $0`/conv2gas $tmp1 >$tmp2

# Remove temporary file
mv $tmp2 $out
rm $tmp1

done
