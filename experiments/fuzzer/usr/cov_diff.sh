#! /bin/bash

#lcov -q -c -d . -o cov.info
#lcov -r cov.info -o cov_out.info



cd /root;
python2 /usr/local/bin/xencov_split $1 --output-dir=/;
cd xen;
rm -rf cov1.info cov2.info cov1_sort.info cov2_sort.info;
geninfo -q . -o cov1.info;

cd /root;
python2 /usr/local/bin/xencov_split $2 --output-dir=/;
cd xen;
geninfo -q . -o cov2.info;


gawk -v RS='SF:' -v ORS='\n\0' 1 /root/xen/cov1.info | sort -z | tr -d '\0' > cov1_sort.info
gawk -v RS='SF:' -v ORS='\n\0' 1 /root/xen/cov2.info | sort -z | tr -d '\0' > cov2_sort.info

diff /root/xen/cov1_sort.info /root/xen/cov2_sort.info > diff

