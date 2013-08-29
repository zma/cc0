cd /root/dc-programming;
st=$(git pull | grep 'up-to-date');
if [ "$st" == "Already up-to-date." ] ; then
  echo "$st at $(date +%y%m%d)" > /root/update_disacc_src.log;
else
  cd src/toolchain/build/;
  make;
  make install; 
  echo "make at $(date +%y%m%d)" > /root/update_disacc_src.log;
fi
