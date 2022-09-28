#!/bin/bash

### WARNING !!!!! STUDENTS MUST NOT CHANGE/EDIT THIS FILE !!!
### You might accidentally delete/change your files or change the intention of the test case
###   if you make changes to this sript without understanding the consequences.
### This script is intended to be executed in mimi using the bash shell.

# BEGIN SETUP
umask 077
testcaseprefix=""
testcasenum=0
function printTestCaseNum
{
		testcasenum=`expr $testcasenum + 1`
		echo "[$testcaseprefix] --> Test Case "`printf "%02d" $testcasenum`' <--'
}

#tmpdir=/tmp/__tmp_comp206_${LOGNAME}_$$
tmpdir=$HOME/tmp/__tmp_comp206_${LOGNAME}_$$
mkdir -p $tmpdir
rc=$?

if [[ $rc -ne 0 ]]
then
	echo "FATAL ERROR during setup !!" 1>&2
	exit 1
fi

function cleanup()
{
# BEGIN CLEANUP
	if [[ -d $tmpdir ]]
	then
		find $tmpdir -type d -exec chmod +rwx {} \+ 2>/dev/null
  	rm -rf $tmpdir
	fi
# END CLEANUP
}

scriptdir=$PWD

function showperms()
{
	echo '----------files and perms--------'
	for f in "$@"
	do
		ls -ild "$f" 2>/dev/null
		rc=$?
		if [[ $rc -ne 0 ]]
		then
			echo "properties of $f cannot be determined."
		fi
	done
	echo '---------------------------------'
}

function showfile()
{
	echo '-------- contents of '$1'--------------'
	if [[ -f $1 ]]
	then
		cat $1
	else
		echo "$1 is not a file / does not exist."
	fi
	echo '-----------------------------------------------------'
}

# END SETUP


if [[ ! -f namefix.bash ]]
then
	echo "WARNING !! could not locate namefix.bash in $scriptdir - those test cases will be skipped" 1>&2
else
	cp -p namefix.bash $tmpdir
	chmod u+x $tmpdir/namefix.bash
	cd $tmpdir
	export testcaseprefix="Ex1"

echo '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  namefix.bash Tests  @@@@@@@@@@@@@@@@@@@@@@@@'
echo ''
echo '********************************************************************************'
echo '[[[ Usage tests - all must fail ]]]'
echo ''
printTestCaseNum
# no arguments
echo ./namefix.bash
./namefix.bash
prc=$?
echo "Return code from the program is $prc"
echo ''

printTestCaseNum
# less arguments
touch $tmpdir/file1.txt
showperms $tmpdir/file1.txt
echo ./namefix.bash $tmpdir/file1.txt
./namefix.bash $tmpdir/file1.txt
prc=$?
echo "Return code from the program is $prc"
echo ''

printTestCaseNum
# more arguments
touch $tmpdir/file1.txt $tmpdir/file2.out $tmpdir/file3.out
showperms $tmpdir/file1.txt $tmpdir/file2.out $tmpdir/file3.out
echo ./namefix.bash $tmpdir/file1.txt $tmpdir/file2.out $tmpdir/file3.out
./namefix.bash $tmpdir/file1.txt $tmpdir/file2.out $tmpdif/file3.out
prc=$?
echo "Return code from the program is $prc"
echo ''

printTestCaseNum
# identical input/output (using abs/rel paths)
echo "john lennon" > $tmpdir/file1d.txt
showperms $tmpdir/file1d.txt file1d.txt
echo ./namefix.bash $tmpdir/file1d.txt file1d.txt
./namefix.bash $tmpdir/file1d.txt file1d.txt
prc=$?
echo "Return code from the program is $prc"
echo ''


echo ''
echo '[[[ Permission tests - all must fail ]]]'
echo ''
printTestCaseNum
# no read permission on input
mkdir $tmpdir/testp1
echo 'john lennon' > $tmpdir/testp1/file1.txt
chmod -r $tmpdir/testp1/file1.txt
showperms $tmpdir/testp1/file1.txt $tmpdir/testp1/file1.out
echo ./namefix.bash $tmpdir/testp1/file1.txt $tmpdir/testp1/file1.out
./namefix.bash $tmpdir/testp1/file1.txt $tmpdir/testp1/file1.out
prc=$?
echo "Return code from the program is $prc"
echo ''

printTestCaseNum
# no write permission on existing output file.
mkdir $tmpdir/testp2
echo 'john lennon' > $tmpdir/testp2/file1.txt
touch $tmpdir/testp2/file1.out
chmod -w $tmpdir/testp2/file1.out
showperms $tmpdir/testp2/file1.txt $tmpdir/testp2/file1.out
echo ./namefix.bash $tmpdir/testp2/file1.txt $tmpdir/testp2/file1.out
./namefix.bash $tmpdir/testp2/file1.txt $tmpdir/testp2/file1.out
prc=$?
echo "Return code from the program is $prc"
echo ''

printTestCaseNum
# no write permission on output directory.
mkdir $tmpdir/testp3
echo 'john lennon' > $tmpdir/testp3/file1.txt
chmod -w $tmpdir/testp3
showperms $tmpdir/testp3/file1.txt $tmpdir/testp3 $tmpdir/testp3/file1.out
echo ./namefix.bash $tmpdir/testp3/file1.txt $tmpdir/testp3/file1.out
./namefix.bash $tmpdir/testp3/file1.txt $tmpdir/testp3/file1.out
prc=$?
echo "Return code from the program is $prc"
chmod +w $tmpdir/testp3
echo ''


printTestCaseNum
# input file exist, output is a dir name, but final output will also map to a dir name.
mkdir -p $tmpdir/testp4/out/names1.txt
ifile=$tmpdir/testp4/names1.txt ofile=testp4/out
echo 'brian epstein' > $ifile
echo ''
showperms $ifile $ofile $ofile/names1.txt
echo ./namefix.bash $ifile $ofile
./namefix.bash $ifile $ofile
prc=$?
echo "Return code from the program is $prc"
echo "Check if input and output files exist."
showperms $ifile $ofile $ofile/names1.txt


echo ''
echo '[[[ Correct usages, must succeed ]]]'

printTestCaseNum
# input file exist, output does not.
mkdir $tmpdir/test1
ifile=$tmpdir/test1/names1.txt ofile=$tmpdir/test1/names1.out
echo 'john lennon' > $ifile
echo ''
showperms $ifile $ofile
echo ./namefix.bash $ifile $ofile
./namefix.bash $ifile $ofile
prc=$?
echo "Return code from the program is $prc"
echo "Check if input and output files exist."
showperms $ifile $ofile

printTestCaseNum
# input file exist, output does not. No read permission on out directory.
mkdir -p $tmpdir/test2/out
ifile=$tmpdir/test2/names1.txt ofile=test2/out/names1.out
chmod -r test2/out
echo 'paul mccartney' > $ifile
echo ''
showperms $ifile test2 $ofile
echo ./namefix.bash $ifile $ofile
./namefix.bash $ifile $ofile
prc=$?
echo "Return code from the program is $prc"
echo "Check if input and output files exist."
showperms $ifile $ofile


printTestCaseNum
# input file exist, as does the output.
mkdir $tmpdir/test3
ifile=$tmpdir/test3/names1.txt ofile=test3/names1.out
echo 'george harrison' > $ifile
touch $ofile
echo ''
showperms $ifile $ofile
echo ./namefix.bash $ifile $ofile
./namefix.bash $ifile $ofile
prc=$?
echo "Return code from the program is $prc"
echo "Check if input and output files exist."
showperms $ifile $ofile


printTestCaseNum
# input file exist, as does the output. (but in a dir with no write permission)
mkdir -p $tmpdir/test4/out
ifile=$tmpdir/test4/names1.txt ofile=test4/out/names1.out
echo 'ringo starr' > $ifile
touch $ofile
chmod -w $tmpdir/test4/out
echo ''
showperms $ifile $tmpdir/test4/out $ofile
echo ./namefix.bash $ifile $ofile
./namefix.bash $ifile $ofile
prc=$?
echo "Return code from the program is $prc"
echo "Check if input and output files exist."
showperms $ifile $ofile

printTestCaseNum
# input file exist, output is a dir name
mkdir -p $tmpdir/test5/out
ifile=$tmpdir/test5/names1.txt ofile=test5/out
echo 'brian epstein' > $ifile
echo ''
showperms $ifile $ofile
echo ./namefix.bash $ifile $ofile
./namefix.bash $ifile $ofile
prc=$?
echo "Return code from the program is $prc"
echo "Check if input and output files exist."
showperms $ifile $ofile $ofile/names1.txt


fi #End of namefix.bash test cases.

cd $scriptdir


if [[ ! -f primechk.bash ]]
then
	echo "WARNING !! could not locate primechk.bash in $scriptdir - those test cases will be skipped" 1>&2
else
	cp -p primechk.bash $tmpdir
	chmod u+x $tmpdir/primechk.bash
	cd $tmpdir
	export testcaseprefix="Ex2"
	testcasenum=0

echo '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  primechk.bash Tests  @@@@@@@@@@@@@@@@@@@@@@@@'
echo ''
echo '********************************************************************************'
echo '[[[ Usage tests - all must fail ]]]'
echo ''
printTestCaseNum
# no arguments
echo ./primechk.bash
./primechk.bash
prc=$?
echo "Return code from the program is $prc"
echo ''

echo ''
printTestCaseNum
echo 13 > nums.txt
showfile nums.txt
# Incorrect arguments
echo ./primechk.bash -f nums.txt -l -f
./primechk.bash -f nums.txt -l -f
prc=$?
echo "Return code from the program is $prc"
echo ''

echo ''
printTestCaseNum
# Incorrect arguments
echo ./primechk.bash -f -l
./primechk.bash -f -l
prc=$?
echo "Return code from the program is $prc"
echo ''

echo ''
printTestCaseNum
echo 13 > nums.txt
showfile nums.txt
# Incorrect arguments
echo ./primechk.bash -l nums.txt -f
./primechk.bash -l nums.txt -f
prc=$?
echo "Return code from the program is $prc"
echo ''

echo ''
printTestCaseNum
echo 13 > nums.txt
showfile nums.txt
# Incorrect arguments
echo ./primechk.bash  nums.txt -f
./primechk.bash nums.txt -f
prc=$?
echo "Return code from the program is $prc"
echo ''

echo ''
printTestCaseNum
showfile nosuch.txt
# File does not exist
echo ./primechk.bash  -f nosuch.txt
./primechk.bash  -f nosuch.txt
prc=$?
echo "Return code from the program is $prc"
echo ''


echo ''
echo '[[[ Correct usages, must succeed ]]]'

echo ''
printTestCaseNum
cat <<-END > nums001.txt
13
13.17
17
12As43
122.21
1307
END
showfile nums001.txt
echo ./primechk.bash  -f nums001.txt
./primechk.bash  -f nums001.txt
prc=$?
echo "Return code from the program is $prc"
echo ''

echo ''
printTestCaseNum
cat <<-END > nums002.txt
13
13.17
1307

19
END
showfile nums002.txt
echo ./primechk.bash  -f $tmpdir/nums002.txt
./primechk.bash  -f $tmpdir/nums002.txt
prc=$?
echo "Return code from the program is $prc"
echo ''

echo ''
printTestCaseNum
cat <<-END > nums003.txt
22
10092003300140014003

121234567898765432
23
END
showfile nums003.txt
echo ./primechk.bash  -f nums003.txt
./primechk.bash  -f nums003.txt
prc=$?
echo "Return code from the program is $prc"
echo ''


# print only the largest prime
echo ''
printTestCaseNum
cat <<-END > nums004.txt
13
13.17
1306

19
END
showfile nums004.txt
echo ./primechk.bash  -f $tmpdir/nums004.txt -l
./primechk.bash  -f $tmpdir/nums004.txt -l
prc=$?
echo "Return code from the program is $prc"
echo ''


# print only the largest prime
# no primes
echo ''
printTestCaseNum
cat <<-END > nums005.txt
130
13.17
1306

3192
END
showfile nums005.txt
echo ./primechk.bash  -f $tmpdir/nums005.txt -l
./primechk.bash  -f $tmpdir/nums005.txt -l
prc=$?
echo "Return code from the program is $prc"
echo ''


fi #End of primechk.sh test cases.

cleanup
