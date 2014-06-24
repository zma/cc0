echo "[`date`] "
for i in *.c0 *.c
do
    cc0 -I./libi0 $i -o $i.bin >$i.compile.log
    diff $i.bin correct/$i.bin >$i.diff.log
    if [ "$?" == "0" ]
    then
        echo "$i [OK]"
    else
        echo ">>>> $i [FAIL]"
    fi
done

echo "[`date`] "
echo "All test done."
