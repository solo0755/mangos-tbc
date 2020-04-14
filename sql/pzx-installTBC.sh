#!/bin/sh
#mangosSqlName=$1
cmd="mysql -uroot -proot -s --default-character-set=utf8"
cmdimport="mysql -uroot -proot -s --default-character-set=utf8"

#创建库
printf "创建库 \n"
${cmd} < create/db_create_mysql.sql
printf "初始化mangos数据库 \n"
${cmd} < base/mangos.sql

${cmd} -e "use tbcmangos"
printf "初始化dbc数据1 \n"
${cmd} tbcmangos < base/dbc/*.sql
printf "初始化dbc数据2 \n"
for sql_file in $(ls base/dbc/original_data/*.sql); do ${cmd} --database=tbcmangos < $sql_file ; done
printf "初始化dbc数据3 \n"
for sql_file in $(ls base/dbc/cmangos_fixes/*.sql); do ${cmd} --database=tbcmangos < $sql_file ; done

${cmd} -e "use tbccharacters"
printf "初始化characters数据库 \n"
${cmd} tbccharacters < base/characters.sql
${cmd} -e "use tbcrealmd"
printf "初始化realmd数据库 \n"
${cmd} tbcrealmd < base/realmd.sql