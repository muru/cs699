declare -A count
count["noext"]=0

for i in *
do
	EXT=${i##*.}
	echo "$EXT"
	if [[ "$EXT" == "$i" ]]; then
		count["noext"]=$(( count["noext"] + 1 ))
	else
		let count["$EXT"]=$(( count["$EXT"] + 1 ))
	fi
done

echo ${count["noext"]}
echo ${!count[@]}

