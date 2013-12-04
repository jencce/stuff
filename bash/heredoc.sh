#! /bin/bash

# interactive cmd like vi use heredoc
# though do not work out #ex /tmp/tt <<xasd
#i
#line1
#line2
#
#xasd
#exit

echo -------------------------

# output muilti-lines msg better than echo
cat <<xasd
line1
line2
line3
xasd

echo -------------------------

# with - option in limit-string
# leading tab in scripts wont show while execution
# leading space does show, so does 'embedded' tabs
cat <<-xasd
	line1
	line2
	line3
        line4
   line5
xasd

echo -------------------------

# heredoc supports parameter and cmd substitution
# lots of work can be done by this way, like a tool
# of transfer file via ftp, where server name, prot
# and file name can be passed by params.
if [ $# -ge 1 ]
then
	name="$1"
else
	name=Joe
fi

cat <<xasd
Hi, $name
#shows up
xasd

echo -------------------------

# However, quote or escaping the limit-string will disable
# the parameter and cmd substitution in heredoc.
# so, generating scripts or code is one use of this.
cat <<'asd'
Hi, $name
asd
cat <<"asd"
Hi, $name
`ls`
asd
cat <<\asd
Hi, $name
asd

echo -------------------------

# if heredoc redirect to a dummy cmd, it can comment code block
:<<"CMT"
#! /bin/bash
do stuff
end
CMT
# the ending limit-string must start at the first charactor, leading
# white space or other will prevent it from being recognized, trailing
# white space or other causes unexpected behavior.

echo -------------------------
cat <<!
aaaaa
bbbb!
!
# this is not recommanded, multi-chars limit-string is prefer.
