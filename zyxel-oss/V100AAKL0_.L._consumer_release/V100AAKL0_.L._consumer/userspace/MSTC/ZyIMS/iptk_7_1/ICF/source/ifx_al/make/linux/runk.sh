make clean
kwinject -o $1.out make $2
kwadmin create-project $1
\rm -rf temp
mkdir temp
kwadmin build-project $1 ./temp --using-build-spec  ./$1.out
cp /home/klocwork/Klocwork/projects_root/projects/$1/builds/build_1/reports/report.txt report_$1.txt
