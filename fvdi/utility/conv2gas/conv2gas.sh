#!/bin/sh

for file in $@; do
case "$file" in
  *.s) out=`dirname $file`/`basename $file .s`.gnu.s ;;
  *) out="$file.gnu" ;;
esac
tmp=`basename $file`.tmp

# Comments with * in the first position on the line
sed "s%^\*.*%%" $file >$out
mv $out $tmp

# Comments after ; separator
sed "s%;.*$%%" $tmp >$out
mv $out $tmp

# Conditional compilation
sed "s%^\\([ 	][ 	]*\\)ifnd%\1.ifndef%" $tmp >$out
mv $out $tmp
sed "s%^\\([ 	][ 	]*\\)ifd%\1.ifdef%" $tmp >$out
mv $out $tmp
sed "s%^\\([ 	][ 	]*\\)ifne%\1.ifne%" $tmp >$out
mv $out $tmp
sed "s%^\\([ 	][ 	]*\\)ifeq%\1.ifeq%" $tmp >$out
mv $out $tmp
sed "s%^\\([ 	][ 	]*\\)ifge%\1.ifge%" $tmp >$out
mv $out $tmp
sed "s%^\\([ 	][ 	]*\\)else%\1.else%" $tmp >$out
mv $out $tmp
sed "s%^\\([ 	][ 	]*\\)endc%\1.endif%" $tmp >$out
mv $out $tmp

# Constants
sed "s%^\\([^ 	]*\\)[ 	][ 	]*equ[ 	][ 	]*%	.equ \\1,%" $tmp >$out
mv $out $tmp
sed "s%^\\([^ 	]*\\)[ 	][ 	]*set[ 	][ 	]*%	.set	\\1,%" $tmp >$out
mv $out $tmp

# Sections
sed "s%^\\([ 	][ 	]*\\)text%\1.text%" $tmp >$out
mv $out $tmp
sed "s%^\\([ 	][ 	]*\\)data%\1.data%" $tmp >$out
mv $out $tmp
sed "s%^\\([ 	][ 	]*\\)bss%\1.bss%" $tmp >$out
mv $out $tmp

# Includes
sed "s%\.inc\"%.inc.gnu\"%" $tmp >$out
mv $out $tmp
sed "s%\\([ 	][ 	]*\\)include%\1.include%" $tmp >$out
mv $out $tmp

# Hexadecimal numbers
sed "s%\\$%0x%" $tmp >$out
mv $out $tmp

sed "s%^.loop1:$%1:%" $tmp >$out
mv $out $tmp
sed "s%^.loop2:$%2:%" $tmp >$out
mv $out $tmp
sed "s%^.loop2_end:$%3:%" $tmp >$out
mv $out $tmp
sed "s%,.loop1$%,1b%" $tmp >$out
mv $out $tmp
sed "s%,.loop2$%,2b%" $tmp >$out
mv $out $tmp
sed "s%.loop2_end$%3f%" $tmp >$out
mv $out $tmp

# Things that would be hard do deal with using sed
`dirname $0`/conv2gas $tmp >$out

# Remove temporary file
rm $tmp

done

